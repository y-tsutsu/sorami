#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "area_code_selector.h"
#include "weather_json_downloader.h"

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::make_unique;
using std::map;
using std::pair;
using std::string;
using std::vector;

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
        string code = selector->SelectAreaCode();

        if (selector->IsQuitCode(code))
        {
            break;
        }

        if (selector->IsInvalidCode(code))
        {
            continue;
        }

        json json = downloader->Download(code);
    }

    return 0;
}
