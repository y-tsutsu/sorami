#include <weather_json_downloader.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

#include <curl/curl.h>

using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;

namespace chrono = std::chrono;
namespace fs = std::filesystem;

namespace Json
{
    WeatherJsonDownloader::WeatherJsonDownloader()
    {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    WeatherJsonDownloader::~WeatherJsonDownloader()
    {
        curl_global_cleanup();
    }

    json WeatherJsonDownloader::Download(string area_code)
    {
        if (!CreateDownloadDirectory())
        {
            return json();
        }

        string filepath = GetJsonFilepath(area_code);
        if (!ExistsTodayFileAndCleanup(filepath))
        {
            DownloadByCurl(area_code, filepath);
        }

        ifstream f(filepath.c_str());
        return json::parse(f);
    }

    bool WeatherJsonDownloader::CreateDownloadDirectory()
    {
        fs::path dir = DOWNLOAD_DIR;
        if (fs::exists(dir))
        {
            return true;
        }
        std::error_code ec;
        if (!fs::create_directories(dir, ec))
        {
            cerr << "Failed to create directory: " << ec.message() << endl;
            return false;
        }
        return true;
    }

    string WeatherJsonDownloader::GetJsonFilepath(string area_code)
    {
        return string(DOWNLOAD_DIR) + "/" + area_code + ".json";
    }

    string WeatherJsonDownloader::GetJsonUrl(string area_code)
    {
        return "https://www.jma.go.jp/bosai/forecast/data/forecast/" + area_code + ".json";
    }

    bool WeatherJsonDownloader::ExistsTodayFileAndCleanup(string filepath)
    {
        if (!fs::exists(filepath))
        {
            return false;
        }

        auto ftime = fs::last_write_time(filepath);
        auto sctp = chrono::time_point_cast<chrono::system_clock::duration>(
            ftime - fs::file_time_type::clock::now() + chrono::system_clock::now());
        std::time_t cftime = chrono::system_clock::to_time_t(sctp);
        std::tm *file_time = std::localtime(&cftime);

        std::time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
        std::tm *now_time = std::localtime(&now);

        if (file_time->tm_year == now_time->tm_year && file_time->tm_yday == now_time->tm_yday)
        {
            return true;
        }
        else
        {
            fs::remove(filepath);
            return false;
        }
    }

    bool WeatherJsonDownloader::DownloadByCurl(string area_code, string filepath)
    {
        CURL *curl = curl_easy_init();
        if (!curl)
        {
            cerr << "Failed to curl_easy_init() failed" << endl;
            return false;
        }

        ofstream ofs(filepath, std::ios::binary);
        if (!ofs)
        {
            cerr << "Failed to open file for writing: " << filepath << endl;
            return false;
        }

        string url = GetJsonUrl(area_code);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WeatherJsonDownloader::WriteToFile);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ofs);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
            ofs.close();
            fs::remove(filepath);
            return false;
        }

        ofs.close();
        curl_easy_cleanup(curl);
        return true;
    }

    size_t WeatherJsonDownloader::WriteToFile(void *contents, size_t size, size_t nmemb, void *userp)
    {
        ofstream *ofs = static_cast<std::ofstream *>(userp);
        size_t totalSize = size * nmemb;
        ofs->write(static_cast<char *>(contents), totalSize);
        return totalSize;
    }
}
