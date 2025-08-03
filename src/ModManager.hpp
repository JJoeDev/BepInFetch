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

struct ModData {
    std::string modName;
    std::string tagName;
    std::string publishTime;
    std::vector<Asset> assets;
};

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

    std::vector<std::string> m_trackedMods{};

private:
    httplib::Headers m_headers{};
    const char* m_host = "api.github.com";

    std::function<void()> m_OnDataRetrieving;
    std::function<void()> m_OnDataRetrieved;
};
