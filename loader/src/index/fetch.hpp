#pragma once

#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

Result<std::string> fetch(std::string const& url);
Result<> fetchFile(
    std::string const& url,
    ghc::filesystem::path const& into,
    std::function<int(double, double)> prog = nullptr
);

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


