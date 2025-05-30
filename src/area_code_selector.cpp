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
            auto number = element["number"].get<int>();
            area_nums_.insert(number);
        }
    }

    AreaCodeSelector::~AreaCodeSelector()
    {
    }

    AreaCodeSelector::AreaInfo AreaCodeSelector::SelectAreaInfo()
    {
        PrintAreaSelectMessage();

        int select_num;
        cin >> select_num;

        if (IsQuitNumber(select_num))
        {
            return AreaInfo::NewQuitAreaInfo();
        }

        if (IsInvalidAreaNumber(select_num))
        {
            cout << "[Error]: Invalid Area Number..." << endl;
            cout << endl;
            return AreaInfo::NewInvalidAreaInfo();
        }

        PrintSubAreaSelectMessage(select_num);

        int select_sub_num;
        cin >> select_sub_num;

        if (IsInvalidSubAreaNumber(select_sub_num))
        {
            cout << "[Error]: Invalid Sub Area Number..." << endl;
            cout << endl;
            return AreaInfo::NewInvalidAreaInfo();
        }

        return GetAreaInfo(select_sub_num);
    }

    void AreaCodeSelector::PrintAreaSelectMessage()
    {
        cout << "input area number." << endl;
        cout << endl;

        for (auto &element : config_)
        {
            auto name = element["name"].get<string>();
            auto number = element["number"].get<int>();
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

    AreaCodeSelector::AreaInfo AreaCodeSelector::GetAreaInfo(int sub_area_num)
    {
        for (auto &&info : area_infos_)
        {
            if (info.number == sub_area_num)
                return info;
        }
        return AreaInfo();
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

    vector<AreaCodeSelector::AreaInfo> AreaCodeSelector::GetAreaInfos(int area_num)
    {
        vector<AreaInfo> ret;
        for (auto &&element : config_)
        {
            auto num = element["number"].get<int>();
            if (num != area_num)
                continue;
            for (auto &ele : element["areas"])
            {
                ret.emplace_back(ele["name"].get<string>(),
                                 ele["number"].get<int>(),
                                 ele["code"].get<string>());
            }
            break;
        }
        return ret;
    }
}