#pragma once

#include <set>
#include <string>
#include <string_view>
#include <vector>

#include <nlohmann/json.hpp>

using std::set;
using std::string;
using std::string_view;
using std::vector;

using json = nlohmann::json;

namespace Console
{
    class AreaCodeSelector
    {
    public:
        struct AreaInfo
        {
            string name;
            int number;
            string code;
            bool IsQuitCode() { return code == QUIT_CODE; }
            bool IsInvalidCode() { return code == INVALID_CODE; }
            static AreaInfo NewQuitAreaInfo() { return {"", 0, string(QUIT_CODE)}; }
            static AreaInfo NewInvalidAreaInfo() { return {"", 0, string(INVALID_CODE)}; }
        };

    private:
        const json &config_;
        set<int> area_nums_;
        vector<AreaInfo> area_infos_;
        static constexpr int QUIT_NUM = 0;
        static constexpr string_view QUIT_CODE = "QUIT_CODE";
        static constexpr string_view INVALID_CODE = "INVALID_CODE";

    public:
        AreaCodeSelector(const json &config);
        ~AreaCodeSelector();

    public:
        AreaInfo SelectAreaCode();

    private:
        void PrintAreaSelectMessage();
        void PrintSubAreaSelectMessage(int area_num);
        AreaInfo GetAreaInfo(int sub_area_num);
        bool IsQuitNumber(int area_num);
        bool IsInvalidAreaNumber(int area_num);
        bool IsInvalidSubAreaNumber(int sub_area_num);
        vector<AreaInfo> GetAreaInfos(int area_num);
    };
}
