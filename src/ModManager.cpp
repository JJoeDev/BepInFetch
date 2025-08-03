#include "ModManager.hpp"
#include "httplib.h"
#include <fstream>
#include <ios>
#include <regex>
#include <stdexcept>

ModManager::ModManager() {
    m_headers = {{"User-Agent", "BepInFetch"}};
}

/// This function does not expect to recieve https://github.com/ only /username/reponame
ModData ModManager::GetReleaseData(const std::string& repo) const {
    m_OnDataRetrieving();

    httplib::SSLClient cli(m_host);

    ModData modData{};

    auto res = cli.Get("/repos/" + repo + "/releases/latest", m_headers);
    if(res && res->status == 200) {
        auto json = nlohmann::json::parse(res->body);

        modData.modName = json["name"].get<std::string>();
        modData.tagName = json["tag_name"].get<std::string>();
        modData.publishTime = json["published_at"].get<std::string>();

        if(json.contains("assets")) {
            for(const auto& asset : json["assets"]) {
                Asset a {
                    .uploader = asset["uploader"]["login"].get<std::string>(),
                    .assetName= asset["name"].get<std::string>(),
                    .downloadUrl = asset["browser_download_url"].get<std::string>(),
                };

                modData.assets.push_back(a);
            }
        }
        else {
            std::cerr << "Request failed: " << (res ? res->status : -1) << std::endl;
        }

    }

    m_OnDataRetrieved();
    return modData;
}

struct UrlParts {
    std::string host;
    std::string path;
};
 
UrlParts ParseURL(const std::string& url) {
    std::regex urlRegex(R"(https?://([^/]+)(/.*))");
    std::smatch match;
    if(std::regex_match(url, match, urlRegex)) {
        std::cout << "REGEX MATCHES\n";
        return {match[1], match[2]};
    }

    std::cerr << "REGEX MATCH ERROR\n";

    throw std::invalid_argument("Invalid URL: " + url);
}

void ModManager::Download(const std::string& url, fs::path destination) {
    m_OnDataRetrieving();

    std::string currentUrl = url;
    int maxRedirects = 5;

    while(maxRedirects-- > 0) {
        auto [host, path] = ParseURL(currentUrl);

        httplib::SSLClient cli(host);
        cli.set_follow_location(false);

        std::ofstream ofs(destination, std::ios::binary | std::ios::trunc);
        if(!ofs) {
            std::cerr << "Failed to open file: " << destination << std::endl;
            m_OnDataRetrieved();
            return;
        }

        auto res = cli.Get(path, m_headers, [&](const char* data, std::streamsize dataLen) {
            ofs.write(data, dataLen);
            return true;
        });

        if(!res) {
            std::cerr << "REQUEST FAILED\n";
            ofs.close();
            m_OnDataRetrieved();
            return;
        }

        if(res->status == 200) {
            std::cout << "DOWNLOAD SUCCESS\n";
            ofs.close();
            m_OnDataRetrieved();
            return;
        }
        else if(res->status == 302 || res->status == 301 || res->status == 303 || res->status == 307 || res->status == 308) {
            // Redirect - Get location header and update currentUrl

            auto it = res->headers.find("Location");
            if(it != res->headers.end()) {
                currentUrl = it->second;
                std::cout << "REDIRECT TO: " << currentUrl << '\n';
                ofs.close();
                continue;
            }
            else {
                std::cerr << "REDIRECT STATUS BUT NO LOCATION HEADER\n";
                ofs.close();
                m_OnDataRetrieved();
                return;
            }
        }
        else {
            std::cerr << "DOWNLOAD FAILED: HTTP STATUS " << res->status << '\n';
            ofs.close();
            m_OnDataRetrieved();
            return;
        }
    }

    std::cerr << "Too many redirects!\n";
    m_OnDataRetrieved();
}
