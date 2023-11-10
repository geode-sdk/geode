#include "LoaderImpl.hpp"

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/general.hpp>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <iomanip>

using namespace geode::prelude;
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

std::string log::parse(gd::string const& str) {
    return fmt::format("{}", std::string(str));
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
    return toString(logTime, 0);
}
std::string Log::toString(bool logTime, uint32_t nestLevel) const {
    std::string res;

    if (logTime) {
        res += fmt::format("{:%H:%M:%S}", m_time);
    }

    res += fmt::format(" [{}]: ", m_sender ? m_sender->getName() : "Geode?");

    for (uint32_t i = 0; i < nestLevel; i++) {
        res += "  ";
    }

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
    auto res = this->addFormatNew(formatStr, components);
    if (res.isErr()) {
        throw std::runtime_error(res.unwrapErr());
    }
}

Result<> Log::addFormatNew(std::string_view formatStr, std::span<ComponentTrait*> components) {
    size_t compIndex = 0;
    std::string current;
    for (size_t i = 0; i < formatStr.size(); ++i) {
        if (formatStr[i] == '{') {
            if (i == formatStr.size() - 1) {
                return Err("Unescaped { at the end of format string");
            }
            auto const next = formatStr[i + 1];
            if (next == '{') {
                current.push_back('{');
                ++i;
                continue;
            }
            if (next == '}') {
                if (compIndex >= components.size()) {
                    return Err("Not enough arguments for format string");
                }
                
                m_components.push_back(new ComponentBase(current));
                m_components.push_back(components[compIndex++]);

                current.clear();
                ++i;
                continue;
            }
            return Err("You put something in between {} silly head");
        }
        if (formatStr[i] == '}') {
            if (i == formatStr.size() - 1) {
                return Err("Unescaped } at the end of format string");
            }
            if (formatStr[i + 1] == '}') {
                current.push_back('}');
                ++i;
                continue;
            }
            return Err("You have an unescaped }");
        }

        current.push_back(formatStr[i]);
    }

    if (!current.empty())
        m_components.push_back(new ComponentBase(current));

    if (compIndex != components.size()) {
        return Err("You have left over arguments.. silly head");
    }

    return Ok();
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
uint32_t& Logger::nestLevel() {
    static std::uint32_t nestLevel = 0;
    return nestLevel;
}

void Logger::setup() {
    logStream() = std::ofstream(dirs::getGeodeLogDir() / log::generateLogName());
}

void Logger::push(Log&& log) {
    if (log.getSender()->isLoggingEnabled()) {
        std::string logStr = log.toString(true, nestLevel());

        LoaderImpl::get()->logConsoleMessageWithSeverity(logStr, log.getSeverity());
        logStream() << logStr << std::endl;

        logs().emplace_back(std::forward<Log>(log));
    }
}

void Logger::pop(Log* log) {
    geode::utils::ranges::remove(Logger::logs(), *log);
}

void Logger::pushNest() {
    if (nestLevel() == std::numeric_limits<uint32_t>::max())
        return;
    nestLevel()++;
}
void Logger::popNest() {
    if (nestLevel() == 0)
        return;
    nestLevel()--;
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
