#include "ModManager.hpp"
#include "httplib.h"

#include <exception>
#include <iostream>

void ModManager::TestLibs() {
    std::cout << " === Testing cpp-httplib (HTTPS request) ===\n";

    httplib::SSLClient cli("www.google.com", 443);
    cli.enable_server_certificate_verification(true);

    auto res = cli.Get("/");

    if(res) {
        std::cout << "HTTP Status: " << res->status << "\n";
        std::cout << "Response length: " << res->body.size() << "\n";
        std::cout << "Sample body (First 200 chars): " << res->body.substr(0, 200) << "\n";
    }
    else {
        std::cout << "Request failed: " << res.error() << "\n";
    }

    std::cout << "\n === Testing nlohmann::json ===\n";

    const char* jsonTxt = R"({"name": "ModManager", "Version": 1, "enabled": true})";

    try {
        nlohmann::json j = nlohmann::json::parse(jsonTxt);

        std::cout << "JSON parsed successfully!\n";
        std::cout << "Name: " << j.at("name").get<std::string>() << "\n";
        std::cout << "Version: " << j.at("Version").get<int>() << '\n';
        std::cout << "Enabled: " << j.at("enabled").get<bool>() << '\n';
    } catch(const std::exception& e) {
        std::cout << "JSON parsing error: " << e.what() << '\n';
    }
}
