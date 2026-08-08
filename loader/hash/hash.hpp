#pragma once

#include <Geode/utils/hash.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <asp/iter.hpp>
#include <string>
#include <filesystem>

inline geode::Sha256 sha256Text(std::filesystem::path const& path) {
    std::string input = geode::utils::file::readString(path).unwrapOrDefault();
    geode::utils::StringBuffer<> buf;

    // remove all newlines
    for (auto line : asp::iter::lines(input)) {
        buf.append(line);
    }

    return geode::sha256(buf.view());
}
