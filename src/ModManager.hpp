#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

struct Asset {
    std::string uploader;
    std::string assetName;
    std::string downloadUrl;

};

inline void to_json(nlohmann::json& j, const Asset& a) {
    j = nlohmann::json{
        {"uploader", a.uploader},
        {"assetName", a.assetName},
        {"downloadUrl", a.downloadUrl},
    };
}

inline void from_json(const nlohmann::json& j, Asset& a) {
    j.at("uploader").get_to(a.uploader);
    j.at("assetName").get_to(a.assetName);
    j.at("downloadUrl").get_to(a.downloadUrl);
}

struct ModData {
    std::string modName;
    std::string tagName;
    std::string publishTime;
    std::vector<Asset> assets;

};

inline void to_json(nlohmann::json& j, const ModData& m) {
    j = nlohmann::json{
        {"modName", m.modName},
        {"tagName", m.tagName},
        {"publishTime", m.publishTime},
        {"assets", m.assets}
    };
}

inline void from_json(const nlohmann::json& j, ModData& m) {
    j.at("modName").get_to(m.modName);
    j.at("tagName").get_to(m.tagName);
    j.at("publishTime").get_to(m.publishTime);
    j.at("assets").get_to(m.assets);
}

class ModManager {
public:
    ModManager();
    ~ModManager() = default;

    inline void SetOnDataRetrieving(std::function<void()> onDataRetrieving) {
        m_OnDataRetrieving = onDataRetrieving;
    }

    inline void SetOnDataRetrieved(std::function<void()> onDataRetrieved) {
        m_OnDataRetrieved = onDataRetrieved;
    }

    [[nodiscard]]
    ModData GetReleaseData(const std::string& repo) const;

    void Download(const std::string& file, fs::path destination);


private:
    httplib::Headers m_headers{};
    const char* m_host = "api.github.com";

    std::function<void()> m_OnDataRetrieving;
    std::function<void()> m_OnDataRetrieved;
};
