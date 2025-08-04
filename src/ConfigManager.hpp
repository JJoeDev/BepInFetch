#pragma once

#include <nlohmann/json.hpp>

class ConfigManager {
public:
    explicit ConfigManager();
    ~ConfigManager() = default;

    void WriteJson(const nlohmann::json& j, const std::string& filename);
    bool ReadJson(nlohmann::json& j, const std::string& filename);

private:

};
