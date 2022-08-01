#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

std::string expandKnownLink(std::string const& link) {
    switch (hash(string_utils::toLower(link).c_str())) {
        case hash("github"):
            if (!string_utils::contains(link, "/")) {
                return "https://github.com/" + link;
            } break;

        case hash("youtube"):
            if (!string_utils::contains(link, "/")) {
                return "https://youtube.com/channel/" + link;
            } break;

        case hash("twitter"):
            if (!string_utils::contains(link, "/")) {
                return "https://twitter.com/" + link;
            } break;

        case hash("newgrounds"):
            if (!string_utils::contains(link, "/")) {
                return "https://" + link + ".newgrounds.com";
            } break;
    }
    return link;
}
