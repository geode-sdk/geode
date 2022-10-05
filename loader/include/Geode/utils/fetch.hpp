#pragma once

#include "../DefaultInclude.hpp"
#include <fs/filesystem.hpp>
#include "Result.hpp"
#include "json.hpp"

namespace geode::utils::web {
    using FileProgressCallback = std::function<bool(double, double)>;

    /**
     * Synchronously fetch data from the internet
     * @param url URL to fetch
     * @returns Returned data as string, or error on error
     */
    GEODE_DLL Result<std::string> fetch(std::string const& url);

    /**
     * Syncronously download a file from the internet
     * @param url URL to fetch
     * @param into Path to download file into
     * @param prog Progress function; first parameter is bytes downloaded so 
     * far, and second is total bytes to download. Return true to continue 
     * downloading, and false to interrupt. Note that interrupting does not 
     * automatically remove the file that was being downloaded
     * @returns Returned data as JSON, or error on error
     */
    GEODE_DLL Result<> fetchFile(
        std::string const& url,
        ghc::filesystem::path const& into,
        FileProgressCallback prog = nullptr
    );

    /**
     * Synchronously fetch data from the internet and parse it as JSON
     * @param url URL to fetch
     * @returns Returned data as JSON, or error on error
     */
    template<class Json = nlohmann::json>
    Result<Json> fetchJSON(std::string const& url) {
        auto res = fetch(url);
        if (!res) return Err(res.error());
        try {
            return Ok(Json::parse(res.value()));
        } catch(std::exception& e) {
            return Err(e.what());
        }
    }
}

