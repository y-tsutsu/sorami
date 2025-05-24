#pragma once

#include <set>
#include <vector>

#include <nlohmann/json.hpp>

using std::set;
using std::string;
using std::vector;

using json = nlohmann::json;

namespace Console
{
    class InputConsole
    {
    private:
        struct AreaInfo
        {
            string name;
            int number;
            string code;
        };

    private:
        json config_;
        set<int> area_nums_;
        vector<AreaInfo> area_infos_;
        static constexpr int QUIT_NUM = 0;

    public:
        InputConsole(json config);
        ~InputConsole();

    public:
        void PrintAreaSelectMessage();
        void PrintSubAreaSelectMessage(int area_num);
        string GetSubAreaCode(int area_num);
        bool IsQuitNumber(int area_num);
        bool IsInvalidAreaNumber(int area_num);
        bool IsInvalidSubAreaNumber(int sub_area_num);

    private:
        vector<AreaInfo> GetAreaInfos(int number, json config);
    };
}
