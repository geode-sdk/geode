#include <Geode/Geode.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Interface.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/casts.hpp>
#include <InternalLoader.hpp>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <iomanip>

USE_GEODE_NAMESPACE();
using namespace geode::log;
using namespace cocos2d;

std::string log::parse(Mod* const& mod) {
    if (mod) {
        return fmt::format("{{ Mod, {} }}", mod->getName());;
    } else {
        return "{ Mod, null }";
    }
}

std::string log::parse(CCObject* const& obj) {
    if (obj) {
        return fmt::format("{{ {}, {} }}", typeid(*obj).name(), utils::intToHex(obj));
    } else {
        return "{ CCObject, null }";
    }
}

std::string log::parse(CCArray* const& arr) {
    std::string out = "[";

    if (arr && arr->count()) {
        for (int i = 0; i < arr->count(); ++i) {
            out += parse(arr->objectAtIndex(i));
            if (i < arr->count() - 1) out += ", ";
        }
    } else out += "empty";

    return out + "]";
}

std::string log::parse(CCPoint const& pt) {
    return fmt::format("{}, {}", pt.x, pt.y);
}

std::string log::parse(CCSize const& sz) {
    return fmt::format("{} : {}", sz.width, sz.height);
}

std::string log::parse(CCRect const& rect) {
    return parse(rect.origin) + " | " + parse(rect.size); 
}

std::string log::parse(cocos2d::ccColor3B const& col) {
    return fmt::format("rgb({}, {}, {})", col.r, col.g, col.b);
}

std::string log::parse(cocos2d::ccColor4B const& col) {
    return fmt::format("rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
}

Log::Log(Mod* mod, Severity sev) : m_sender(mod), m_time(log_clock::now()), m_severity(sev) {}

bool Log::operator==(Log const& l) {
    return this == &l;
}

std::string Log::toString(bool logTime) const {
    std::string res;

    if (logTime) {
        const auto t = std::chrono::system_clock::to_time_t(m_time);
        tm obj;
        #ifdef _MSC_VER
        localtime_s(&obj, &t);
        #else
        obj = *std::localtime(&t);
        #endif
        res += fmt::format("{:%H:%M:%S} ", m_time);
    }

    res += fmt::format("[{}]:", m_sender ? m_sender->getName() : "?");

    for (auto& i : m_components) {
        res += i->_toString();
    }

    return res;
}

void Log::pushToLoader() {
    Loader::get()->pushLog(std::move(*this));
}

std::string geode::log::generateLogName() {
    std::stringstream tmp;
    tmp << "Geode_" 
        << std::chrono::duration_cast<std::chrono::seconds>(log_clock::now().time_since_epoch()).count()
        << ".log";
    return tmp.str();
}
