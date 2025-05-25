#include "area_code_selector.h"

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;

namespace Console
{
    AreaCodeSelector::AreaCodeSelector(const json &config) : config_(config)
    {
        for (auto &element : config_)
        {
            auto number = element["number"].template get<int>();
            area_nums_.insert(number);
        }
    }

    AreaCodeSelector::~AreaCodeSelector()
    {
    }

    string AreaCodeSelector::SelectAreaCode()
    {
        PrintAreaSelectMessage();

        int select_num;
        cin >> select_num;

        if (IsQuitNumber(select_num))
        {
            return string(QUIT_CODE);
        }

        if (IsInvalidAreaNumber(select_num))
        {
            cout << "[Error]: Invalid Area Number..." << endl;
            cout << endl;
            return string(INVALID_CODE);
        }

        PrintSubAreaSelectMessage(select_num);

        int select_sub_num;
        cin >> select_sub_num;

        if (IsInvalidSubAreaNumber(select_sub_num))
        {
            cout << "[Error]: Invalid Sub Area Number..." << endl;
            cout << endl;
            return string(INVALID_CODE);
        }

        string code = GetSubAreaCode(select_sub_num);
        return code;
    }

    bool AreaCodeSelector::IsQuitCode(string_view code)
    {
        return code == QUIT_CODE;
    }

    bool AreaCodeSelector::IsInvalidCode(string_view code)
    {
        return code == INVALID_CODE;
    }

    void AreaCodeSelector::PrintAreaSelectMessage()
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

    void AreaCodeSelector::PrintSubAreaSelectMessage(int area_num)
    {
        cout << endl;
        area_infos_ = GetAreaInfos(area_num);
        for (auto &&info : area_infos_)
        {
            cout << info.number << ": " << info.name << endl;
        }
        cout << endl;
    }

    string AreaCodeSelector::GetSubAreaCode(int sub_area_num)
    {
        for (auto &&info : area_infos_)
        {
            if (info.number == sub_area_num)
                return info.code;
        }
        return "";
    }

    bool AreaCodeSelector::IsQuitNumber(int area_num)
    {
        return area_num == QUIT_NUM;
    }

    bool AreaCodeSelector::IsInvalidAreaNumber(int area_num)
    {
        return !area_nums_.contains(area_num);
    }

    bool AreaCodeSelector::IsInvalidSubAreaNumber(int sub_area_num)
    {
        for (auto &&info : area_infos_)
        {
            if (info.number == sub_area_num)
                return false;
        }
        return true;
    }

    vector<AreaCodeSelector::AreaInfo> AreaCodeSelector::GetAreaInfos(int number)
    {
        vector<AreaInfo> ret;
        for (auto &element : config_)
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