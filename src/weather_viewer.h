#pragma once

#include <nlohmann/json.hpp>

#include "area_code_selector.h"

using json = nlohmann::json;

namespace Console
{
    using AreaInfo = AreaCodeSelector::AreaInfo;

    class WeatherViewer
    {
    private:
        /* data */
    public:
        WeatherViewer();
        ~WeatherViewer();

    public:
        void Display(AreaInfo info, json json);

    private:
        void DisplayHeader(AreaInfo info);
        void DisplayContens(json json);
    };
}
