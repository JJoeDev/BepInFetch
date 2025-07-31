#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

class ModManager {
public:
    ModManager() = default;
    ~ModManager() = default;

    void TestLibs();

private:
    std::vector<std::string> m_trackedMods{};
};
