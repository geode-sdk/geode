#include "LogImpl.hpp"

#include "console.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Types.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/general.hpp>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <memory>
#include <ostream>
#include <utility>

using namespace geode::prelude;
using namespace geode::log;
using namespace cocos2d;

static std::atomic<bool> g_logMillis{false};
static constexpr size_t LOG_BUFFER_LIMIT = 32768;

auto convertTime(auto timePoint) {
    // std::chrono::current_zone() isnt available on clang (android),
    // so do this instead to get the local time for logging.
    // By accident this also gets rid of the decimal places in the seconds
    auto timeEpoch = std::chrono::system_clock::to_time_t(timePoint);
    return fmt::localtime(timeEpoch);
}

// Like Log, but doesn't own any content, and is cheap to construct and copy.
// Can be easily converted into and from Log.
struct geode::log::BorrowedLog final {
    log_clock::time_point m_time;
    Severity m_severity;
    int32_t m_nestCount;
    std::string_view m_content;
    std::string_view m_thread;
    std::string_view m_source;
    Mod* m_mod = nullptr;
    
    BorrowedLog(Severity severity, int32_t nestCount, std::string_view content, std::string_view thread, std::string_view source, Mod* mod)
        : m_time(log_clock::now())
        , m_severity(severity)
        , m_thread(thread)
        , m_source(source)
        , m_nestCount(nestCount)
        , m_content(content)
        , m_mod(mod)
    {}

    BorrowedLog(Log const& log) 
        : m_time(log.m_time)
        , m_severity(log.m_severity)
        , m_thread(log.m_thread)
        , m_source(log.m_source)
        , m_nestCount(log.m_nestCount)
        , m_content(log.m_content)
        , m_mod(nullptr)
    {}

    Log intoLog() const {
        return Log{
            m_time,
            m_severity,
            m_nestCount,
            std::string(m_content),
            std::string(m_thread),
            std::string(m_source),
            m_mod
        };
    }

    template <size_t N>
    void formatTo(StringBuffer<N>& buf, bool millis = false) const {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_time.time_since_epoch()) % 1000;

        if (millis) {
            buf.append("{:%H:%M:%S}.{:03}", convertTime(m_time), ms.count());
        } else {
            buf.append("{:%H:%M:%S}", convertTime(m_time));
        }

        buf.append(' ');
    
        switch (m_severity.m_value) {
            case Severity::Debug: buf.append("DEBUG"); break;
            case Severity::Info: buf.append("INFO "); break;
            case Severity::Warning: buf.append("WARN "); break;
            case Severity::Error: buf.append("ERROR"); break;
            default: buf.append("?????"); break;
        }

        buf.append(' ');

        auto [source, thread, nestCount] = this->truncateWithNest();
        bool sourceTrunc = source.size() != m_source.size();
        bool threadTrunc = thread.size() != m_thread.size();

        if (!thread.empty()) {
            buf.append('[');
            buf.append(thread);
            if (threadTrunc) {
                buf.append('>');
            }
            buf.append("] ");
        }

        buf.append('[');
        buf.append(source);
        if (sourceTrunc) {
            buf.append('>');
        }
        buf.append("]: ");

        for (int32_t i = 0; i < nestCount; i++) {
            buf.append(' ');
        }
    
        buf.append(m_content);
    }

    std::tuple<std::string_view, std::string_view, int32_t> truncateWithNest() const {
        int32_t nestCount = m_nestCount;
        auto source = m_source;
        auto thread = m_thread;
        auto initSourceLen = static_cast<int32_t>(source.size());
        auto initThreadLen = static_cast<int32_t>(thread.size());
    
        if (nestCount != 0) {
            nestCount -= initSourceLen + initThreadLen;
        }

        if (nestCount >= 0) {
            return { source, thread, nestCount };
        }

        int32_t needsCollapse = -nestCount;

        if (initThreadLen == 0) { // no thread
            auto sourceLen = std::max(initSourceLen - needsCollapse, 2);
            if (sourceLen < source.size())
                source = source.substr(0, sourceLen - 1);
        }
        else {
            int32_t sourceCollapse = needsCollapse / 2;
            int32_t threadCollapse = needsCollapse - sourceCollapse;

            int32_t sourceLen = std::max(initSourceLen - sourceCollapse, 2);
            int32_t threadLen = std::max(initThreadLen - threadCollapse, 2);

            sourceCollapse = initSourceLen - sourceLen;
            threadCollapse = initThreadLen - threadLen;
            int32_t remainder = needsCollapse - sourceCollapse - threadCollapse;
            if (remainder > 0) {
                sourceLen = std::max(sourceLen - remainder, 2);
            }

            if (sourceLen < source.size())
                source = source.substr(0, sourceLen - 1);
            if (threadLen < thread.size())
                thread = thread.substr(0, threadLen - 1);
        }

        return { source, thread, nestCount };
    }
};

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
        return fmt::format("{{ {}, {} }}", getObjectName(obj), fmt::ptr(obj));
    } else {
        return "{ CCObject, null }";
    }
}

std::string geode::format_as(CCNode* obj) {
    if (obj) {
        return fmt::format(
            "{{ {}, {}, ({}) }}",
            getObjectName(obj),
            fmt::ptr(obj),
            obj->boundingBox()
        );
    } else {
        return "{ CCNode, null }";
    }
}

std::string geode::format_as(CCArray* arr) {
    if (arr && arr->count()) {
        fmt::memory_buffer buffer;
        buffer.push_back('[');

        for (int i = 0; i < arr->count(); ++i) {
            auto* obj = arr->objectAtIndex(i);
            buffer.append(format_as(obj));
            if (i + 1 < arr->count()) {
                buffer.append(std::string_view(", "));
            }
        }

        buffer.push_back(']');
        return fmt::to_string(buffer);
    } else {
        return "[empty]";
    }
}

// Log

Log::Log(log_clock::time_point time, Severity severity, int32_t nestCount,
    std::string content, std::string thread, std::string source, Mod* mod)
    : m_time(time), m_severity(severity),
      m_thread(std::move(thread)), m_source(std::move(source)),
      m_nestCount(nestCount), m_content(std::move(content)), m_mod(mod)
{}

std::string Log::toString(bool millis) const {
    StringBuffer<> buf;
    BorrowedLog(*this).formatTo(buf, millis);
    return buf.str();
}

inline static thread_local int32_t s_nestLevel = 0;
inline static thread_local int32_t s_nestCountOffset = 0;
inline static thread_local bool s_insideLogImpl = false;

struct LogImplGuard {
    LogImplGuard() { s_insideLogImpl = true; }
    ~LogImplGuard() { s_insideLogImpl = false; }
};

void log::vlogImpl(Severity sev, Mod* mod, fmt::string_view format, fmt::format_args args) {
    // prevent recursion
    if (s_insideLogImpl) return;

    if (!mod->isLoggingEnabled() || sev < mod->getLogLevel()) return;

    auto nestCount = s_nestLevel * 2;
    if (nestCount != 0) {
        nestCount += s_nestCountOffset;
    }

    LogImplGuard _guard;

    Logger::get()->push(sev, nestCount, fmt::vformat(format, args),
        thread::getName(), mod->getName(), mod);
}

std::filesystem::path const& log::getCurrentLogPath() {
    return Logger::get()->getLogPath();
}

void log::addLogCallback(LogCallback callback) {
    Logger::get()->addLogCallback(std::move(callback));
}

Severity Log::getSeverity() const {
    return m_severity;
}

// Logger

Logger* Logger::get() {
    static Logger inst;
    return &inst;
}

Logger::~Logger() {
    m_terminating.store(true, std::memory_order::release);

    if (m_usingThread && m_logThread.joinable()) {
        m_logCv.notify_one();
        m_logThread.join();
    }

    // dont flush as it could be problematic during shutdown
}

std::mutex& getLogMutex() {
    static std::mutex mutex;
    return mutex;
}

void Logger::setup() {
    if (m_initialized.load(std::memory_order::acquire)) {
        return;
    }

    std::lock_guard g(getLogMutex());

    g_logMillis = Mod::get()->getSettingValue<bool>("log-milliseconds");
    listenForSettingChanges("log-milliseconds", [](bool val) {
        g_logMillis.store(val, std::memory_order::release);
    });

    auto logDir = dirs::getGeodeLogDir();

    // on the first launch, this doesn't exist yet..
    if (!std::filesystem::exists(logDir)) {
        std::error_code ec;
        std::filesystem::create_directories(logDir, ec);
    }

    m_logPath = logDir / log::generateLogName();
    m_logStream = std::ofstream(m_logPath);

    // Logs can and will probably be added before setup() is called, so we'll write them now
    for (Log const& log : m_logs) {
        this->outputLog(BorrowedLog(log), true);
    }
    m_logs.clear();

    this->flushLocked();
    m_initialized.store(true, std::memory_order::release);

    // setup log thread
    m_usingThread = Mod::get()->getSettingValue<bool>("log-thread");
    if (m_usingThread) {
        m_logThread = std::thread(&Logger::workerThread, this);
    }
}

void Logger::workerThread() {
    thread::setName("Geode Log Thread");

    // keep a local vector to avoid reallocations every time
    std::vector<Log> processing;

    while (!m_terminating.load(std::memory_order::acquire)) {
        std::unique_lock g(getLogMutex());
        m_logCv.wait(g, [this]() {
            return !m_logs.empty() || m_terminating.load(std::memory_order::acquire);
        });

        // move all logs from the queue into our vector
        while (!m_logs.empty()) {
            processing.push_back(std::move(m_logs.front()));
            m_logs.pop_front();
        }

        // unlock mutex and print all the logs
        g.unlock();

        for (auto& log : processing) {
            this->outputLog(BorrowedLog(log));
        }
        processing.clear();
    }
}

void Logger::deleteOldLogs(size_t maxAgeHours) {
    auto logDir = dirs::getGeodeLogDir();

    auto now = std::chrono::file_clock::now();

    std::error_code ec;
    auto iterator = std::filesystem::directory_iterator(logDir, ec);
    if (ec != std::error_code{}) {
        log::error("Failed to delete old logs: {}", ec.message());
        return;
    }

    for (auto const& entry : iterator) {
        if (entry.is_regular_file() && entry.path().extension() == ".log") {
            auto time = std::filesystem::last_write_time(entry, ec);
            if (ec != std::error_code{}) {
                continue;
            }

            auto diff = now - time;
            if (diff > std::chrono::hours(maxAgeHours)) {
                std::filesystem::remove(entry, ec);
            }
        }
    }
}

static Severity logLevelFor(std::string_view level) {
    if (level == "debug") {
        return Severity::Debug;
    } else if (level == "info") {
        return Severity::Info;
    } else if (level == "warn") {
        return Severity::Warning;
    } else if (level == "error") {
        return Severity::Error;
    } else {
        return Severity::Info;
    }
}

Severity Logger::getConsoleLogLevel() {
    auto level = Mod::get()->getSettingValue<std::string_view>("console-log-level");
    return logLevelFor(level);
}

Severity Logger::getFileLogLevel() {
    auto level = Mod::get()->getSettingValue<std::string_view>("file-log-level");
    return logLevelFor(level);
}

void Logger::push(Severity sev, int32_t nestCount, std::string content,
    std::string_view thread, std::string_view source, Mod* mod)
{
    // if thread is enabled or logging isn't initialized, push into the queue; otherwise print right now
    if (!m_initialized.load(std::memory_order::acquire) || m_usingThread) {
        std::lock_guard g(getLogMutex());

        if (m_logs.size() >= LOG_BUFFER_LIMIT) {
            // drop oldest log
            m_logs.pop_front();
        }

        m_logs.emplace_back(
            log_clock::now(),
            sev,
            nestCount,
            std::move(content),
            std::string(thread),
            std::string(source),
            mod
        );
        m_logCv.notify_one();
        return;
    }

    std::lock_guard g(getLogMutex());
    this->outputLog(BorrowedLog(
        sev,
        nestCount,
        content,
        thread,
        source,
        mod
    ));
}

void Logger::outputLog(BorrowedLog const& log, bool dontFlush) {
    auto sev = log.m_severity;

    // should we log this at all?
    bool logConsole = sev >= this->getConsoleLogLevel();
    bool logFile = sev >= this->getFileLogLevel();
    bool logCallbacks = !m_callbacks.empty();
    if (!logConsole && !logFile && !logCallbacks) return;

    StringBuffer<> buf;
    bool millis = g_logMillis.load(std::memory_order::relaxed);
    log.formatTo(buf, millis);

    if (logConsole) {
        console::log(buf.c_str(), sev);
    }
    if (logFile) {
        m_logStream << buf.view() << '\n';
        // don't flush stream for every log as that's super slow
        if (!dontFlush) {
            this->maybeFlushStream();
        }
    }
    if (logCallbacks) {
        for (auto& cb : m_callbacks) {
            cb(log.m_content, log.m_severity, log.m_mod, log.m_source, log.m_thread);
        }
    }
}

void Logger::maybeFlushStream() {
    m_unflushedLogs++;
    if (m_unflushedLogs >= 64) {
        this->flushLocked();
        return;
    }

    auto now = log_clock::now();
    if (now - m_lastFlushTime >= std::chrono::seconds(1)) {
        this->flushLocked();
        return;
    }
}

void Logger::flush() {
    std::lock_guard g(getLogMutex());
    this->flushLocked();
}

void Logger::flushLocked() {
    m_logStream << std::flush;
    m_unflushedLogs = 0;
    m_lastFlushTime = log_clock::now();
}

void Logger::clear() {
    std::lock_guard g(getLogMutex());
    m_logs.clear();
}

void Logger::addLogCallback(LogCallback callback) {
    std::lock_guard g(getLogMutex());
    m_callbacks.push_back(std::move(callback));
}

Nest::Nest(std::shared_ptr<Nest::Impl> impl) : m_impl(std::move(impl)) { }
Nest::Impl::Impl(int32_t nestLevel, int32_t nestCountOffset) :
    m_nestLevel(nestLevel), m_nestCountOffset(nestCountOffset) { }

std::filesystem::path const& Logger::getLogPath() const {
    return m_logPath;
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
