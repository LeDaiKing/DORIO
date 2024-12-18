#include "ConfigLoader.hpp"
#include <fstream>
#include <iostream>
ConfigLoader::ConfigLoader()
{
    loadConfig("Dough", "file/Dough/abilities.json");
    loadConfig("Enemy", "file/Enemy/abilities.json");
    loadConfig("Map", "file/Map/loadmap.json");
}

ConfigLoader& ConfigLoader::getInstance()
{
    static ConfigLoader instance;
    return instance;
}

const nlohmann::json& ConfigLoader::getConfig(const char* nkey)
{
    //format a/b/c
    std::string delimiter = "/";
    size_t pos = 0;
    std::string token;
    std::string key = nkey;
    nlohmann::json* current = nullptr;
    while ((pos = key.find(delimiter)) != std::string::npos) {
        token = key.substr(0, pos);
        if (current == nullptr)
        {
            current = &configList[token];
        }
        else
            current = &(*current)[token];
        key.erase(0, pos + delimiter.length());
    }
        

    return (*current)[key];
}

void ConfigLoader::loadConfig(const char* key, const char* path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cerr << "Cannot open file " << path << std::endl;
        return;
    }
    nlohmann::json a;
    file >> a;
    configList[key] = a;
    file.close();
}
