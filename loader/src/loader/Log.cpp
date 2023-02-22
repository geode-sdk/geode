#include "LoaderImpl.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/general.hpp>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iomanip>

USE_GEODE_NAMESPACE();
using namespace geode::log;
using namespace cocos2d;

// Parse overloads

std::string log::parse(Mod* mod) {
    if (mod) {
        return fmt::format("{{ Mod, {} }}", mod->getName());
    }
    else {
        return "{ Mod, null }";
    }
}

std::string log::parse(CCObject* obj) {
    if (obj) {
        return fmt::format("{{ {}, {} }}", typeid(*obj).name(), utils::intToHex(obj));
    }
    else {
        return "{ CCObject, null }";
    }
}

std::string log::parse(CCNode* obj) {
    if (obj) {
        auto bb = obj->boundingBox();
        return fmt::format(
            "{{ {}, {}, ({}, {} | {} : {}) }}",
            typeid(*obj).name(),
            utils::intToHex(obj),
            bb.origin.x,
            bb.origin.y,
            bb.size.width,
            bb.size.height
        );
    }
    else {
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
    }
    else out += "empty";

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

// Log

Log::Log(Mod* mod, Severity sev) : m_sender(mod), m_time(log_clock::now()), m_severity(sev) {}

Log::~Log() {
    for (auto comp : m_components) {
        delete comp;
    }
}

bool Log::operator==(Log const& l) {
    return this == &l;
}

std::string Log::toString(bool logTime) const {
    std::string res;

    if (logTime) {
        res += fmt::format("{:%H:%M:%S}", m_time);
    }

    res += fmt::format(" [{}]: ", m_sender ? m_sender->getName() : "Geode?");

    for (auto& i : m_components) {
        res += i->_toString();
    }

    return res;
}

std::vector<ComponentTrait*>& Log::getComponents() {
    return m_components;
}

log_clock::time_point Log::getTime() const {
    return m_time;
}

Mod* Log::getSender() const {
    return m_sender;
}

Severity Log::getSeverity() const {
    return m_severity;
}

void Log::addFormat(std::string_view formatStr, std::span<ComponentTrait*> components) {

    size_t compIndex = 0;
    std::string current;
    for (size_t i = 0; i < formatStr.size(); ++i) {
        if (formatStr[i] == '{') {
            if (i == formatStr.size() - 1)
                throw std::runtime_error("Unescaped { at the end of format string");
            auto const next = formatStr[i + 1];
            if (next == '{') {
                current.push_back('{');
                ++i;
                continue;
            }
            if (next == '}') {
                if (compIndex >= components.size())
                    throw std::runtime_error("Not enough arguments for format string");
                
                m_components.push_back(new ComponentBase(current));
                m_components.push_back(components[compIndex++]);

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
        m_components.push_back(new ComponentBase(current));

    if (compIndex != components.size()) {
        throw std::runtime_error("You have left over arguments.. silly head");
        // show_silly_error(formatStr);
    }
}

// Logger

std::vector<Log>& Logger::logs() {
    static std::vector<Log> logs;
    return logs;
}
std::ofstream& Logger::logStream() {
    static std::ofstream logStream;
    return logStream;
}

void Logger::setup() {
    logStream() = std::ofstream(dirs::getGeodeLogDir() / log::generateLogName());
}

void Logger::push(Log&& log) {
    std::string logStr = log.toString(true);

    LoaderImpl::get()->logConsoleMessage(logStr);
    logStream() << logStr << std::endl;

    logs().emplace_back(std::forward<Log>(log));
}

void Logger::pop(Log* log) {
    geode::utils::ranges::remove(Logger::logs(), *log);
}

std::vector<Log*> Logger::list() {
    std::vector<Log*> logs_;
    logs_.reserve(logs().size());
    for (auto& log : logs()) {
        logs_.push_back(&log);
    }
    return logs_;
}

void Logger::clear() {
    logs().clear();
}

// Misc

std::string geode::log::generateLogName() {
    return fmt::format("Geode {:%d %b %H.%M.%S}.log", log_clock::now());
}
