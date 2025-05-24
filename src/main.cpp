#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "input_console.h"

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::pair;
using std::string;
using std::vector;

using json = nlohmann::json;

using Console::InputConsole;

json read_configfile(string filename)
{
    std::ifstream f(filename.c_str());
    json config = json::parse(f);
    return config;
}

int main(int argc, char const *argv[])
{
    auto config = read_configfile("./config/area_config.json");
    auto console = InputConsole(config);

    while (true)
    {
        console.PrintAreaSelectMessage();

        int select_num;
        cin >> select_num;

        if (console.IsQuitNumber(select_num))
            break;
        if (console.IsInvalidAreaNumber(select_num))
        {
            cout << "[Error]: Invalid Area Number..." << endl;
            cout << endl;
            continue;
        }

        console.PrintSubAreaSelectMessage(select_num);

        int select_sub_num;
        cin >> select_sub_num;

        if (console.IsInvalidSubAreaNumber(select_sub_num))
        {
            cout << "[Error]: Invalid Sub Area Number..." << endl;
            cout << endl;
            continue;
        }

        string code = console.GetSubAreaCode(select_sub_num);
        cout << code << " !!!!!" << endl;
        cout << endl;
    }

    return 0;
}
