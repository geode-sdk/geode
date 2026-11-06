#pragma once

#include <Geode/utils/crypto.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <asp/iter.hpp>
#include <string>
#include <asp/fs/Path.hpp>

inline geode::Sha256 sha256Text(asp::fs::PathView path) {
    std::string input = geode::utils::file::readString(path).unwrapOrDefault();
    geode::utils::StringBuffer<> buf;

    // remove all newlines
    for (auto line : asp::iter::lines(input)) {
        buf.append(line);
    }

    return geode::sha256(buf.view());
}
