#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "area_code_selector.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

using json = nlohmann::json;

using Console::AreaCodeSelector;

json read_configfile(string filename)
{
    std::ifstream f(filename.c_str());
    json config = json::parse(f);
    return config;
}

int main(int argc, char const *argv[])
{
    auto config = read_configfile("./config/area_config.json");
    auto console = AreaCodeSelector(config);

    while (true)
    {
        string code = console.SelectAreaCode();

        if (console.IsQuitCode(code))
        {
            break;
        }

        if (console.IsInvalidCode(code))
        {
            continue;
        }

        cout << code << " !!!!!" << endl;
        cout << endl;
    }

    return 0;
}
