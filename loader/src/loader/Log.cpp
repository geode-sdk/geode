#include "console.hpp"
#include "LogImpl.hpp"

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

std::string geode::format_as(Mod* mod) {
    if (mod) {
        return fmt::format("{{ Mod, {} }}", mod->getName());
    }
    else {
        return "{ Mod, null }";
    }
}

std::string geode::format_as(CCObject const* obj) {
    if (obj) {
        // TODO: try catch incase typeid fails
        return fmt::format("{{ {}, {} }}", typeid(*obj).name(), fmt::ptr(obj));
    }
    else {
        return "{ CCObject, null }";
    }
}

std::string geode::format_as(CCNode* obj) {
    if (obj) {
        auto bb = obj->boundingBox();
        return fmt::format(
            "{{ {}, {}, ({}, {} | {} : {}) }}",
            typeid(*obj).name(),
            fmt::ptr(obj),
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

std::string geode::format_as(CCArray* arr) {
    std::string out = "[";

    if (arr && arr->count()) {
        for (int i = 0; i < arr->count(); ++i) {
            out += format_as(arr->objectAtIndex(i));
            if (i < arr->count() - 1) out += ", ";
        }
    }
    else out += "empty";

    return out + "]";
}

std::string cocos2d::format_as(CCPoint const& pt) {
    return fmt::format("{}, {}", pt.x, pt.y);
}

std::string cocos2d::format_as(CCSize const& sz) {
    return fmt::format("{} : {}", sz.width, sz.height);
}

std::string cocos2d::format_as(CCRect const& rect) {
    return fmt::format("{} | {}", rect.origin, rect.size);
}

std::string cocos2d::format_as(cocos2d::ccColor3B const& col) {
    return fmt::format("rgb({}, {}, {})", col.r, col.g, col.b);
}

std::string cocos2d::format_as(cocos2d::ccColor4B const& col) {
    return fmt::format("rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
}

// Log

void log::vlogImpl(Severity sev, Mod* mod, fmt::string_view format, fmt::format_args args) {
    Logger::get()->push(
        sev,
        mod,
        fmt::vformat(format, args)
    );
}


Log::Log(Severity sev, std::string&& thread, Mod* mod, std::string&& content) :
    m_sender(mod),
    m_time(log_clock::now()),
    m_severity(sev),
    m_thread(thread),
    m_content(content) {}

Log::~Log() {}

auto convertTime(auto timePoint) {
    // std::chrono::current_zone() isnt available on clang (android),
    // so do this instead to get the local time for logging.
    // By accident this also gets rid of the decimal places in the seconds
    auto timeEpoch = std::chrono::system_clock::to_time_t(timePoint);
    return fmt::localtime(timeEpoch);
}

std::string Log::toString(bool logTime, int32_t nestCount) const {
    std::string res;

    if (logTime) {
        res += fmt::format("{:%H:%M:%S}", convertTime(m_time));
    }

    switch (m_severity.m_value) {
        case Severity::Debug:
            res += " DEBUG";
            break;
        case Severity::Info:
            res += " INFO ";
            break;
        case Severity::Warning:
            res += " WARN ";
            break;
        case Severity::Error:
            res += " ERROR";
            break;
        default:
            res += " ?????";
            break;
    }

    auto senderName = m_sender ? m_sender->getName() : "Geode?";
    auto threadName = m_thread;

    if (nestCount != 0) {
        nestCount -= static_cast<int32_t>(senderName.size() + threadName.size());
    }

    if (nestCount < 0) {
        auto initSenderLength = static_cast<int32_t>(senderName.size());
        auto initThreadLength = static_cast<int32_t>(threadName.size());

        auto needsCollapse = -nestCount;

        auto senderCollapse = needsCollapse / 2;
        auto senderLength = std::max(initSenderLength - senderCollapse, 2);
        senderCollapse = initSenderLength - senderLength;

        auto threadCollapse = needsCollapse - senderCollapse;
        auto threadLength = std::max(initThreadLength - threadCollapse, 2);

        if (senderLength < senderName.size())
            senderName = fmt::format("{}>", senderName.substr(0, senderLength - 1));
        if (threadLength < threadName.size())
            threadName = fmt::format("{}>", threadName.substr(0, threadLength - 1));
    }

    res += fmt::format(" [{}] [{}]: ", threadName, senderName);

    for (int32_t i = 0; i < nestCount; i++) {
        res += " ";
    }

    res += m_content;

    return res;
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

std::string_view Log::getContent() const {
    return m_content;
}

// Logger

Logger* Logger::get() {
    static Logger inst;
    return &inst;
}

void Logger::setup() {
    m_logStream = std::ofstream(dirs::getGeodeLogDir() / log::generateLogName());
}

std::mutex g_logMutex;
void Logger::push(Severity sev, Mod* mod, std::string&& content) {
    if (!mod->isLoggingEnabled()) return;

    Log* log = nullptr;
    {
        std::lock_guard g(g_logMutex);
        log = &m_logs.emplace_back(sev, thread::getName(), mod, std::move(content));
    }

    auto nestCount = s_nestLevel * 2;
    if (nestCount != 0) {
        nestCount += s_nestCountOffset;
    }

    auto const logStr = log->toString(true, nestCount);

    console::log(logStr, log->getSeverity());
    m_logStream << logStr << std::endl;
}

void Logger::pushNest() {
    if (s_nestLevel == 0)
        s_nestCountOffset = static_cast<int32_t>(Mod::get()->getName().size() + thread::getName().size());
    s_nestLevel++;
}

void Logger::popNest() {
    s_nestLevel--;
}

std::vector<Log> const& Logger::list() {
    return m_logs;
}

void Logger::clear() {
    std::lock_guard g(g_logMutex);
    m_logs.clear();
}

// Misc

std::string geode::log::generateLogName() {
    return fmt::format("Geode {:%d %b %H.%M.%S}.log", convertTime(log_clock::now()));
}

void log::pushNest() {
    Logger::pushNest();
}

void log::popNest() {
    Logger::popNest();
}
