#include <Geode/utils/string.hpp>

USE_GEODE_NAMESPACE();

std::string expandKnownLink(std::string const& link) {
    switch (hash(utils::string::toLower(link).c_str())) {
        case hash("github"):
            if (!utils::string::contains(link, "/")) {
                return "https://github.com/" + link;
            } break;

        case hash("youtube"):
            if (!utils::string::contains(link, "/")) {
                return "https://youtube.com/channel/" + link;
            } break;

        case hash("twitter"):
            if (!utils::string::contains(link, "/")) {
                return "https://twitter.com/" + link;
            } break;

        case hash("newgrounds"):
            if (!utils::string::contains(link, "/")) {
                return "https://" + link + ".newgrounds.com";
            } break;
    }
    return link;
}
