#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
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

std::vector<std::function<void(Mod*)>>& Log::scheduled() {
    static std::vector<std::function<void(Mod*)>> ret;
    return ret;
}

void log::releaseSchedules(Mod* m) {
    for (auto& func : Log::scheduled()) {
        func(m);
    }
    Log::scheduled().clear();
}

std::string log::parse(Mod* mod) {
    if (mod) {
        return fmt::format("{{ Mod, {} }}", mod->getName());;
    } else {
        return "{ Mod, null }";
    }
}

std::string log::parse(CCObject* obj) {
    if (obj) {
        return fmt::format("{{ {}, {} }}", typeid(*obj).name(), utils::intToHex(obj));
    } else {
        return "{ CCObject, null }";
    }
}

std::string log::parse(CCNode* obj) {
    if (obj) {
        auto bb = obj->boundingBox();
        return fmt::format("{{ {}, {}, ({}, {} | {} : {}) }}", 
            typeid(*obj).name(), utils::intToHex(obj),
            bb.origin.x, bb.origin.y, bb.size.width, bb.size.height
        );
    } else {
        return "{ CCNode, null }";
    }
}

std::string log::parse(CCArray* arr) {
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
        res += fmt::format("{:%H:%M:%S}", m_time);
    }

    res += fmt::format(" [{}]: ", m_sender ? m_sender->getName() : "?");

    for (auto& i : m_components) {
        res += i->_toString();
    }

    return res;
}

void Log::pushToLoader() {
    Loader::get()->pushLog(std::move(*this));
}

std::string geode::log::generateLogName() {
    return fmt::format("Geode {:%d %b %H.%M.%S}.log", log_clock::now());
}

void geode::log::vlogImpl(Severity severity, Mod* mod, std::string_view formatStr, std::function<void(Log&)>* components, size_t componentsSize) {
    Log log(mod, severity);

    const auto pushSomething = [](Log& log, auto something) {
        // i think this line of code is very sad
        log.getComponents().push_back(new ComponentBase(something));
    };

    size_t compIndex = 0;
    std::string current;
    for (size_t i = 0; i < formatStr.size(); ++i) {
        if (formatStr[i] == '{') {
            if (i == formatStr.size() - 1)
                throw std::runtime_error("Unescaped { at the end of format string");
            const auto next = formatStr[i + 1];
            if (next == '{') {
                current.push_back('{');
                ++i;
                continue;
            }
            if (next == '}') {
                if (compIndex >= componentsSize)
                    throw std::runtime_error("Not enough arguments for format string");
                pushSomething(log, current);
                components[compIndex++](log);
                current.clear();
                ++i;
                continue;
            }
            throw std::runtime_error("You put something in between {} silly head");
        }
        if (formatStr[i] == '}') {
            if (i == formatStr.size() - 1)
                throw std::runtime_error("Unescaped } at the end of format string");
            if (formatStr[i + 1] == '}') {
                current.push_back('}');
                ++i;
                continue;
            }
            throw std::runtime_error("You have an unescaped }");
        }

        current.push_back(formatStr[i]);
    }

    if (!current.empty())
        pushSomething(log, current);

    if (compIndex != componentsSize) {
        throw std::runtime_error("You have left over arguments.. silly head");
        // show_silly_error(formatStr);
    }

    // (l.getComponents().push_back(new ComponentBase(args)), ...);

    log.pushToLoader();
}

