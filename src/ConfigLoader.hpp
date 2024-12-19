#include "nlohmann/json.hpp"
#include <map>
#include <string>

class ConfigLoader
{
    public:
        static ConfigLoader& getInstance();
        const nlohmann::json& getConfig(const char* nkey);

    private:
        ConfigLoader();
        ~ConfigLoader() = default;
        ConfigLoader(const ConfigLoader&) = delete;
        ConfigLoader& operator=(const ConfigLoader&) = delete;
        void loadConfig(const char* key, const char* path);

    private:
        std::map<std::string, nlohmann::json> configList;
};