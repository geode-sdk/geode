#include <Geode/utils/general.hpp>

#ifdef GEODE_IS_WINDOWS
    #define GEODE_CTIME() ctime_s(buf, sizeof buf, &t);
#else
    #define GEODE_CTIME() strcpy(buf, ctime(&t));
#endif

std::string geode::utils::timePointAsString(std::chrono::system_clock::time_point const& tp) {
    auto t = std::chrono::system_clock::to_time_t(tp);
    char buf[128];
    GEODE_CTIME();
    std::string res = buf;
    res.pop_back();
    return res;
}
