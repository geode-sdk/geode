#include <Geode/utils/general.hpp>

std::string geode::utils::timePointAsString(std::chrono::system_clock::time_point const& tp) {
    return fmt::format("{:%a %b %e %H:%M:%S %Y}", tp);
}

static std::string timeToAgoStringImpl(size_t durationMinutes, bool abbreviated, auto&& formatDate) {
    auto const fmtPlural = [](auto count, auto unit) {
        return fmt::format("{} {}{} ago", count, unit, (count != 1 ? "s" : ""));
    };
    if (durationMinutes < 1) {
        return abbreviated ? "Now" : "Just now";
    }
    if (durationMinutes < 60) {
        return abbreviated ? fmt::format("{}M", durationMinutes) : fmtPlural(durationMinutes, "minute");
    }
    durationMinutes /= 60;
    if (durationMinutes < 24) {
        return abbreviated ? fmt::format("{}H", durationMinutes) : fmtPlural(durationMinutes, "hour");
    }
    durationMinutes /= 24;
    if (durationMinutes < 31) {
        return abbreviated ? fmt::format("{}D", durationMinutes) : fmtPlural(durationMinutes, "day");
    }
    return formatDate();
}

std::string geode::utils::timeToAgoString(std::chrono::system_clock::time_point tp, bool abbreviated) {
    using namespace std::chrono;
    return timeToAgoStringImpl(
        duration_cast<minutes>(system_clock::now() - tp).count(),
        abbreviated,
        [tp] { return fmt::format("{:%b %d %Y}", tp); }
    );
}
std::string geode::utils::timeToAgoString(asp::SystemTime tp, bool abbreviated) {
    auto dur = asp::SystemTime::now().durationSince(tp);
    return timeToAgoStringImpl(
        (dur ? dur->minutes() : 0),
        abbreviated,
        [tp] { return tp.format("{:%b %d %Y}"); }
    );
}
