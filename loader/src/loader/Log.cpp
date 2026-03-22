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
#include <Geode/utils/async.hpp>
#include <asp/time/SystemTime.hpp>
#include <arc/future/Select.hpp>
#include <arc/time/Sleep.hpp>
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

BorrowedLog::BorrowedLog(Severity severity, int32_t nestCount, std::string_view content, std::string_view thread, std::string_view source, Mod* mod)
    : m_time(asp::SystemTime::now())
    , m_severity(severity)
    , m_thread(thread)
    , m_source(source)
    , m_nestCount(nestCount)
    , m_content(content)
    , m_mod(mod)
{}

BorrowedLog::BorrowedLog(Log const& log)
    : m_time(log.m_time)
    , m_severity(log.m_severity)
    , m_thread(log.m_thread)
    , m_source(log.m_source)
    , m_nestCount(log.m_nestCount)
    , m_content(log.m_content)
    , m_mod(log.m_mod)
{}

Log BorrowedLog::intoLog() const {
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

std::tuple<std::string_view, std::string_view, int32_t> BorrowedLog::truncateWithNest() const {
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

Log::Log(asp::SystemTime time, Severity severity, int32_t nestCount,
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

Severity Log::getSeverity() const {
    return m_severity;
}

// Logger

Logger::Logger() {
    auto [tx, rx] = arc::mpsc::channel<Log>(LOG_BUFFER_LIMIT);
    m_logTx = std::move(tx);
    m_logRx = std::move(rx);
}

Logger* Logger::get() {
    static Logger inst;
    return &inst;
}

Logger::~Logger() {
}

void Logger::shutdownThread() {
    auto runtime = m_runtime.upgrade();

    if (m_usingThread.exchange(false, std::memory_order::relaxed) && m_logThread && runtime) {
        m_cancel.cancel();
        m_logThread.blockOn();
        for (auto& msg : m_logRx->drain()) {
            this->outputLog(BorrowedLog(msg), true);
        }
    }

    m_runtime = {};
    this->flush();
}

std::mutex& getLogMutex() {
    static std::mutex mutex;
    return mutex;
}

static Severity logLevelFor(std::string_view level) {
    if (level == "trace") {
        return Severity::Trace;
    } else if (level == "debug") {
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

void Logger::setup() {
    if (m_initialized.load(std::memory_order::acquire)) {
        return;
    }

    std::lock_guard g(getLogMutex());

    g_logMillis = Mod::get()->getSettingValue<bool>("log-milliseconds");
    m_consoleLevel = logLevelFor(
        Mod::get()->getSettingValue<std::string_view>("console-log-level")
    );
    m_fileLevel = logLevelFor(
        Mod::get()->getSettingValue<std::string_view>("file-log-level")
    );

    listenForSettingChanges<bool>("log-milliseconds", [](bool val) {
        g_logMillis.store(val, std::memory_order::release);
    });
    listenForSettingChanges<std::string_view>("console-log-level", [this](std::string_view val) {
        m_consoleLevel.store(logLevelFor(val), std::memory_order::relaxed);
    });
    listenForSettingChanges<std::string_view>("file-log-level", [this](std::string_view val) {
        m_fileLevel.store(logLevelFor(val), std::memory_order::relaxed);
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
    for (Log const& log : m_logRx->drain()) {
        this->outputLog(BorrowedLog(log), true);
    }

    this->flushLocked();
    m_initialized.store(true, std::memory_order::release);

    // setup log thread
    m_usingThread = Mod::get()->getSettingValue<bool>("log-thread");
    if (m_usingThread) {
        m_logThread = async::runtime().spawn(this->workerThread());
        m_runtime = async::runtime().weakFromThis();
        m_logThread.setName("Geode Log Worker");
    }
}

arc::Future<> Logger::workerThread() {
    bool running = true;
    asp::Duration flushInterval = asp::Duration::fromSecs(1);
    asp::Instant nextFlush = asp::Instant::now();
    size_t unflushed = 0;

    auto doFlush = [&] {
        std::lock_guard g(getLogMutex());
        this->flushLocked();
        nextFlush = asp::Instant::now() + flushInterval;
        unflushed = 0;
    };

    size_t flushRequests = 0;

    while (running) {
        auto now = asp::Instant::now();

        if (now >= nextFlush || unflushed >= 64) {
            doFlush();
        }

        // if we have a flush request, only fulfill it once all logs are printed
        if (flushRequests && m_logRx->empty()) {
            doFlush();
            m_syncFlushSemaphore.release(flushRequests);
            flushRequests = 0;
        }

        co_await arc::select(
            arc::selectee(m_logRx->recv(), [&](auto res) {
                if (!res) return;
                Log log = std::move(res).unwrap();

                std::lock_guard g(getLogMutex());
                this->outputLog(BorrowedLog(log), true);
            }),

            arc::selectee(m_cancel.waitCancelled(), [&] { running = false; }),

            arc::selectee(
                m_syncFlushNotify.notified(),
                [&] { flushRequests++; }
            ),

            // periodically flush
            arc::selectee(arc::sleepUntil(nextFlush))
        );
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

Severity Logger::getConsoleLogLevel() {
    return m_consoleLevel.load(std::memory_order::relaxed);
}

Severity Logger::getFileLogLevel() {
    return m_fileLevel.load(std::memory_order::relaxed);
}

void Logger::push(Severity sev, int32_t nestCount, std::string content,
    std::string_view thread, std::string_view source, Mod* mod)
{
    // check if we should log at all, before acquiring any locks,
    // since this check is much cheaper than locking or pushing to queue
    if (!this->shouldOutputLog(sev, mod)) return;

    // if thread is enabled or logging isn't initialized, push into the queue; otherwise print right now
    if (!m_initialized.load(std::memory_order::relaxed) || m_usingThread.load(std::memory_order::relaxed)) {
        (void) m_logTx->trySend(Log{
            asp::SystemTime::now(),
            sev,
            nestCount,
            std::move(content),
            std::string(thread),
            std::string(source),
            mod
        });
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
    bool logConsole, logFile, logListeners, logModLevel;
    if (!this->shouldOutputLog(sev, log.m_mod, logConsole, logFile, logListeners, logModLevel)) {
        return;
    }

    StringBuffer<> buf;
    bool millis = g_logMillis.load(std::memory_order::relaxed);
    log.formatTo(buf, millis);

    if (logConsole && logModLevel) {
        console::log(buf.c_str(), sev);
    }
    if (logFile && logModLevel) {
        m_logStream << buf.view() << '\n';
        // don't flush stream for every log as that's super slow
        if (!dontFlush) {
            this->flushLocked();
        }
    }
    if (logListeners) {
        LogImplGuard _guard;
        LogEvent().send(log);
    }
}

bool Logger::shouldOutputLog(Severity sev, Mod* mod) {
    bool console, file, listeners, modLevel;
    return this->shouldOutputLog(sev, mod, console, file, listeners, modLevel);
}

bool Logger::shouldOutputLog(Severity sev, Mod* mod, bool& console, bool& file, bool& listeners, bool& modLevel) {
    console = sev >= this->getConsoleLogLevel();
    file = sev >= this->getFileLogLevel();
    listeners = LogEvent().getReceiverCount() > 0;
    modLevel = !mod || (mod->isLoggingEnabled() && sev >= mod->getLogLevel());

    // always output the log if there are registered listeners, let them handle filtering
    // otherwise, only output if the log is above a certain level
    return listeners || ((console || file) && modLevel);
}

void Logger::flush() {
    std::lock_guard g(getLogMutex());
    this->flushLocked();
}

void Logger::flushLocked() {
    m_logStream << std::flush;
}

void Logger::flushExternal() {
    // if not using a log thread, then queue is always empty, so simply flush
    if (!m_usingThread) {
        this->flush();
        return;
    }

    // synchronize with the log thread
    m_syncFlushNotify.notifyOne();
    m_syncFlushSemaphore.acquire();
}

void Logger::clear() {
    m_logRx->drain();
}

Nest::Nest(std::shared_ptr<Nest::Impl> impl) : m_impl(std::move(impl)) { }
Nest::Impl::Impl(int32_t nestLevel, int32_t nestCountOffset) :
    m_nestLevel(nestLevel), m_nestCountOffset(nestCountOffset) { }

std::filesystem::path const& Logger::getLogPath() const {
    return m_logPath;
}

// Misc

std::string geode::log::generateLogName() {
    auto now = asp::SystemTime::now();
    return fmt::format("Geode {:%F %H.%M.%S}.log", asp::localtime(now.to_time_t()));
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

void log::flush() {
    Logger::get()->flushExternal();
}

std::shared_ptr<Nest> log::saveNest() {
    return std::make_shared<Nest>(std::make_shared<Nest::Impl>(s_nestLevel, s_nestCountOffset));
}

void log::loadNest(std::shared_ptr<Nest> const& nest) {
    s_nestLevel = nest->m_impl->m_nestLevel;
    s_nestCountOffset = nest->m_impl->m_nestCountOffset;
}
