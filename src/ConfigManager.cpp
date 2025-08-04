#include "ConfigManager.hpp"
#include "StaticInterface.hpp"

#include <iostream>
#include <fstream>

ConfigManager::ConfigManager() {
    GlobalInstance<ConfigManager>::instance = this;
}

void ConfigManager::WriteJson(const nlohmann::json& j, const std::string& filename) {
    std::ofstream file(filename, std::ios::trunc);
    if(file) {
        file << j.dump(4);
    }
    else {
        std::cerr << "[ CONFIG MANAGER ]: Could not create file\n";
    }
}

bool ConfigManager::ReadJson(nlohmann::json& j, const std::string& filename) {
    std::ifstream file(filename);
    if(!file) return false;

    file >> j;

    return true;
}
