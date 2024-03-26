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
#include <memory>
#include <utility>

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

inline static thread_local int32_t s_nestLevel = 0;
inline static thread_local int32_t s_nestCountOffset = 0;

void log::vlogImpl(Severity sev, Mod* mod, fmt::string_view format, fmt::format_args args) {
    if (!mod->isLoggingEnabled()) return;

    auto nestCount = s_nestLevel * 2;
    if (nestCount != 0) {
        nestCount += s_nestCountOffset;
    }

    Logger::get()->push(sev, thread::getName(), mod->getName(), nestCount,
        fmt::vformat(format, args));
}


Log::Log(Severity sev, std::string&& thread, std::string&& source, int32_t nestCount,
    std::string&& content) :
    m_time(log_clock::now()),
    m_severity(sev),
    m_thread(thread),
    m_source(source),
    m_nestCount(nestCount),
    m_content(content) {}

Log::~Log() = default;

auto convertTime(auto timePoint) {
    // std::chrono::current_zone() isnt available on clang (android),
    // so do this instead to get the local time for logging.
    // By accident this also gets rid of the decimal places in the seconds
    auto timeEpoch = std::chrono::system_clock::to_time_t(timePoint);
    return fmt::localtime(timeEpoch);
}

std::string Log::toString() const {
    std::string res = fmt::format("{:%H:%M:%S}", convertTime(m_time));

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

    auto nestCount = m_nestCount;
    auto source = m_source;
    auto thread = m_thread;

    if (nestCount != 0) {
        nestCount -= static_cast<int32_t>(source.size() + thread.size());
    }

    if (nestCount < 0) {
        auto initSourceLength = static_cast<int32_t>(source.size());
        auto initThreadLength = static_cast<int32_t>(thread.size());

        auto needsCollapse = -nestCount;

        if (initThreadLength == 0) {
            auto sourceCollapse = needsCollapse;
            auto sourceLength = std::max(initSourceLength - sourceCollapse, 2);
            if (sourceLength < source.size())
                source = fmt::format("{}>", source.substr(0, sourceLength - 1));
        }
        else {
            auto sourceCollapse = needsCollapse / 2;
            auto sourceLength = std::max(initSourceLength - sourceCollapse, 2);
            sourceCollapse = initSourceLength - sourceLength;

            auto threadCollapse = needsCollapse - sourceCollapse;
            auto threadLength = std::max(initThreadLength - threadCollapse, 2);
            threadCollapse = initThreadLength - threadLength;

            sourceCollapse = needsCollapse - threadCollapse;
            sourceLength = std::max(initSourceLength - sourceCollapse, 2);

            if (sourceLength < source.size())
                source = fmt::format("{}>", source.substr(0, sourceLength - 1));
            if (threadLength < thread.size())
                thread = fmt::format("{}>", thread.substr(0, threadLength - 1));
        }
    }

    if (thread.empty())
        res += fmt::format(" [{}]: ", source);
    else
        res += fmt::format(" [{}] [{}]: ", thread, source);

    for (int32_t i = 0; i < nestCount; i++) {
        res += " ";
    }

    res += m_content;

    return res;
}

Severity Log::getSeverity() const {
    return m_severity;
}

// Logger

Logger* Logger::get() {
    static Logger inst;
    return &inst;
}

void Logger::setup() {
    m_logStream = std::ofstream(dirs::getGeodeLogDir() / log::generateLogName());
}

std::mutex& getLogMutex() {
    static std::mutex mutex;
    return mutex;
}

void Logger::push(Severity sev, std::string&& thread, std::string&& source, int32_t nestCount,
    std::string&& content) {
    std::lock_guard g(getLogMutex());

    Log& log = m_logs.emplace_back(sev, std::move(thread), std::move(source), nestCount,
            std::move(content));

    auto const logStr = log.toString();
    console::log(logStr, log.getSeverity());
    m_logStream << logStr << std::endl;
}

Nest::Nest(std::shared_ptr<Nest::Impl> impl) : m_impl(std::move(impl)) { }
Nest::Impl::Impl(int32_t nestLevel, int32_t nestCountOffset) :
    m_nestLevel(nestLevel), m_nestCountOffset(nestCountOffset) { }

std::vector<Log> const& Logger::list() {
    return m_logs;
}

void Logger::clear() {
    std::lock_guard g(getLogMutex());
    m_logs.clear();
}

// Misc

std::string geode::log::generateLogName() {
    return fmt::format("Geode {:%F %H.%M.%S}.log", convertTime(log_clock::now()));
}

void log::pushNest(Mod* mod) {
    if (s_nestLevel == 0)
        s_nestCountOffset = static_cast<int32_t>(mod->getName().size() + thread::getName().size());
    s_nestLevel++;
}

void log::popNest(Mod* mod) {
    s_nestLevel--;
    if (s_nestLevel == 0)
        s_nestCountOffset = 0;
}

std::shared_ptr<Nest> log::saveNest() {
    return std::make_shared<Nest>(std::make_shared<Nest::Impl>(s_nestLevel, s_nestCountOffset));
}

void log::loadNest(std::shared_ptr<Nest> const& nest) {
    s_nestLevel = nest->m_impl->m_nestLevel;
    s_nestCountOffset = nest->m_impl->m_nestCountOffset;
}
