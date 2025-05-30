#include "weather_viewer.h"

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace Console
{
    WeatherViewer::WeatherViewer()
    {
    }

    WeatherViewer::~WeatherViewer()
    {
    }

    void WeatherViewer::Display(AreaInfo info, json json)
    {
        cout << endl;
        DisplayHeader(info);
        for (auto &&element : json)
        {
            for (auto &&ele : element["timeSeries"])
            {
                if (ele["timeDefines"].size() <= 3)
                {
                    DisplayContens(ele);
                }
            }
        }
        cout << endl;
    }

    void WeatherViewer::DisplayHeader(AreaInfo info)
    {
        cout << info.name << "の天気予報" << endl;
        cout << "====================" << endl;
        cout << endl;
    }

    void WeatherViewer::DisplayContens(json json)
    {
        vector<string> dates;
        for (auto &&element : json["timeDefines"])
        {
            string date = element.template get<string>().substr(0, 10);
            dates.push_back(date);
        }

        auto format = [](string &str)
        {
            const std::string fullWidthSpace = "　";
            const std::string halfWidthSpace = " ";
            size_t pos = 0;
            while ((pos = str.find(fullWidthSpace, pos)) != std::string::npos)
            {
                str.replace(pos, fullWidthSpace.length(), halfWidthSpace);
                pos += halfWidthSpace.length();
            }

            str.erase(str.find_last_not_of(' ') + 1);
        };

        for (auto &&element : json["areas"])
        {
            string area = element["area"]["name"].template get<string>();
            cout << "■ " << area << endl;
            int index = 0;
            for (auto &&ele : element["weathers"])
            {
                string weather = ele.template get<string>();
                format(weather);
                cout << dates[index] << "  " << weather << endl;
                index++;
            }
            cout << endl;
        }
    }
}
