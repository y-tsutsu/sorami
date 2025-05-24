#include "input_console.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

namespace Console
{
    InputConsole::InputConsole(json config) : config_(config)
    {
        for (auto &element : config_)
        {
            auto number = element["number"].template get<int>();
            area_nums_.insert(number);
        }
    }

    InputConsole::~InputConsole()
    {
    }

    void InputConsole::PrintAreaSelectMessage()
    {
        cout << "input area number." << endl;
        cout << endl;

        for (auto &element : config_)
        {
            auto name = element["name"].template get<string>();
            auto number = element["number"].template get<int>();
            cout << number << ": " << name << endl;
        }

        cout << QUIT_NUM << ": 終了" << endl;
        cout << endl;
    }

    void InputConsole::PrintSubAreaSelectMessage(int area_num)
    {
        cout << endl;
        area_infos_ = GetAreaInfos(area_num, config_);
        for (auto &&info : area_infos_)
        {
            cout << info.number << ": " << info.name << endl;
        }
        cout << endl;
    }

    string InputConsole::GetSubAreaCode(int area_num)
    {
        for (auto &&info : area_infos_)
        {
            if (info.number == area_num)
                return info.code;
        }
        return "";
    }

    bool InputConsole::IsQuitNumber(int area_num)
    {
        return area_num == QUIT_NUM;
    }

    bool InputConsole::IsInvalidAreaNumber(int area_num)
    {
        return !area_nums_.contains(area_num);
    }

    bool InputConsole::IsInvalidSubAreaNumber(int sub_area_num)
    {
        for (auto &&info : area_infos_)
        {
            if (info.number == sub_area_num)
                return false;
        }
        return true;
    }

    vector<InputConsole::AreaInfo> InputConsole::GetAreaInfos(int number, json config)
    {
        vector<AreaInfo> ret;
        for (auto &element : config)
        {
            auto num = element["number"].template get<int>();
            if (num != number)
                continue;
            for (auto &ele : element["areas"])
            {
                ret.emplace_back(ele["name"].template get<string>(),
                                 ele["number"].template get<int>(),
                                 ele["code"].template get<string>());
            }
            break;
        }
        return ret;
    }
}