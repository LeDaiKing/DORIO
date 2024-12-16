#include "ConfigLoader.hpp"
#include <fstream>
ConfigLoader::ConfigLoader()
{
    std::ifstream file("file/Dough/abilities.json");
    nlohmann::json a;
    file >> a;
    configList["Dough"] = a;
    file.close();
    file.open("file/Enemy/abilities.json");
    file >> a;
    configList["Enemy"] = a;
    file.close();
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