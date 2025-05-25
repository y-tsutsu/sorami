#include <fstream>
#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "area_code_selector.h"
#include "weather_json_downloader.h"

using std::ifstream;
using std::make_unique;
using std::string;

using json = nlohmann::json;

using Console::AreaCodeSelector;
using Json::WeatherJsonDownloader;

json read_configfile(string filename)
{
    ifstream f(filename.c_str());
    json config = json::parse(f);
    return config;
}

int main(int argc, char const *argv[])
{
    auto config = read_configfile("./config/area_config.json");
    auto selector = make_unique<AreaCodeSelector>(config);
    auto downloader = make_unique<WeatherJsonDownloader>();

    while (true)
    {
        auto info = selector->SelectAreaCode();

        if (info.IsQuitCode())
        {
            break;
        }

        if (info.IsInvalidCode())
        {
            continue;
        }

        auto json = downloader->Download(info.code);
        if (!json.has_value())
        {
            continue;
        }
    }

    return 0;
}
