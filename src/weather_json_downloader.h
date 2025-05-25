#pragma once

#include <optional>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

using std::optional;
using std::string;
using std::string_view;

using json = nlohmann::json;

namespace Json
{
    class WeatherJsonDownloader
    {
    private:
        static constexpr string_view DOWNLOAD_DIR = "json";

    public:
        WeatherJsonDownloader();
        ~WeatherJsonDownloader();

    public:
        optional<json> Download(string area_code);

    private:
        bool CreateDownloadDirectory();
        string GetJsonFilepath(string area_code);
        string GetJsonUrl(string area_code);
        bool ExistsTodayFileAndCleanup(string filepath);
        bool DownloadByCurl(string area_code, string filepath);

        static size_t WriteToFile(void *contents, size_t size, size_t nmemb, void *userp);
    };
}
