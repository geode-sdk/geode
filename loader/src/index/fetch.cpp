#include "fetch.hpp"
#include <curl/curl.h>

namespace fetch_utils {
    static size_t writeData(char* data, size_t size, size_t nmemb, void* str) {
        as<std::string*>(str)->append(data, size * nmemb);
        return size * nmemb;
    }

    static size_t writeBinaryData(char* data, size_t size, size_t nmemb, void* file) {
        as<std::ofstream*>(file)->write(data, size * nmemb);
        return size * nmemb;
    }

    static int progress(void* ptr, double total, double now, double, double) {
        return (*as<std::function<int(double, double)>*>(ptr))(now, total);
    }
}

Result<> fetchFile(
    std::string const& url,
    ghc::filesystem::path const& into,
    std::function<int(double, double)> prog
) {
    auto curl = curl_easy_init();
    
    if (!curl) return Err("Curl not initialized!");

    std::ofstream file(into, std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        return Err("Unable to open output file");
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_utils::writeBinaryData);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
    if (prog) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, fetch_utils::progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &prog);
    }
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return Err("Fetch failed");
    }

    char* ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if ((res == CURLE_OK) && ct) {
        curl_easy_cleanup(curl);
        return Ok();
    }
    curl_easy_cleanup(curl);
    return Err("Error getting info: " + std::string(curl_easy_strerror(res)));
}

Result<std::string> fetch(std::string const& url) {
    auto curl = curl_easy_init();
    
    if (!curl) return Err("Curl not initialized!");

    std::string ret;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fetch_utils::writeData);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github_api/1.0");
    auto res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        return Err("Fetch failed");
    }

    char* ct;
    res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
    if ((res == CURLE_OK) && ct) {
        curl_easy_cleanup(curl);
        return Ok(ret);
    }
    curl_easy_cleanup(curl);
    return Err("Error getting info: " + std::string(curl_easy_strerror(res)));
}


