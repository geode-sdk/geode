#include <loader/console.hpp>
#include <loader/LogImpl.hpp>
#include <io.h>

using namespace geode::prelude;

HANDLE s_outHandle = nullptr;
FILE* s_origStdOut = nullptr;
bool s_hasAnsiColorSupport = false;
bool s_shouldAlloc = false;

bool redirectStd(FILE* which, std::string const& name, const Severity sev) {
    auto pipeName = fmt::format(R"(\\.\pipe\geode-{}-{})", name, GetCurrentProcessId());
    auto pipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
        1, 0, 1024, 0, nullptr
    );
    if (!pipe) {
        log::warn("Failed to create pipe, {} will be unavailable", name);
        return false;
    }
    std::thread([pipe, name, sev]() {
        thread::setName(fmt::format("{} Read Thread", name));
        std::string cur;
        while (true) {
            char buf[1024];
            DWORD read = 0;
            if (!ReadFile(pipe, buf, 1024, &read, nullptr))
                continue;
            for (auto i = 0; i < read; i++) {
                if (buf[i] != '\n') {
                    if (buf[i] != '\r')
                        cur += buf[i];
                    continue;
                }
                log::Logger::get()->push(sev, "", std::string(name), 0, std::string(cur));
                cur.clear();
            }
        }
    }).detach();
    FILE* yum;
    if (freopen_s(&yum, pipeName.c_str(), "w", which)) {
        log::warn("Failed to reopen file, {} will be unavailable", name);
        return false;
    }
    return true;
}

void console::setup() {
    bool hasStdout = _fileno(stdout) >= 0;
    if (!s_shouldAlloc) {
        s_origStdOut = hasStdout ? _fdopen(_dup(_fileno(stdout)), "w") : nullptr;
        if (!redirectStd(stdout, "stdout", Severity::Info) && s_origStdOut) {
            fclose(s_origStdOut);
            s_origStdOut = nullptr;
        }
        redirectStd(stderr, "stderr", Severity::Debug);
    }

    s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!s_outHandle || !hasStdout) {
        if (s_shouldAlloc)
            return;
        if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
            s_shouldAlloc = true;
            return;
        }
        s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!s_outHandle || !hasStdout) {
            s_shouldAlloc = true;
            return;
        }
    }
    s_shouldAlloc = false;
    SetConsoleCP(CP_UTF8);

    DWORD consoleMode = 0;

    // Set output mode to handle ansi color sequences
    s_hasAnsiColorSupport = GetConsoleMode(s_outHandle, &consoleMode) &&
        (
            (consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) > 0 ||
            SetConsoleMode(s_outHandle, consoleMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING)
        );

    // copied from pwsh when gd is in console mode
    SetConsoleMode(
        GetStdHandle(STD_INPUT_HANDLE),
        ENABLE_PROCESSED_INPUT |
        ENABLE_LINE_INPUT |
        ENABLE_ECHO_INPUT |
        ENABLE_MOUSE_INPUT |
        ENABLE_INSERT_MODE |
        ENABLE_QUICK_EDIT_MODE |
        ENABLE_EXTENDED_FLAGS |
        ENABLE_AUTO_POSITION
    );

    for (auto const& log : log::Logger::get()->list()) {
        console::log(log.toString(), log.getSeverity());
    }
}

void console::openIfClosed() {
    if (!s_shouldAlloc)
        return;
    AllocConsole();
    setup();
}

void console::log(std::string const& msg, Severity severity) {
    if (!s_outHandle)
        return;
    DWORD written;

    if (!s_hasAnsiColorSupport) {
        if (s_origStdOut) {
            fwrite(msg.c_str(), 1, msg.size(), s_origStdOut);
            fwrite("\n", 1, 1, s_origStdOut);
            fflush(s_origStdOut);
        }
        WriteConsoleA(s_outHandle, msg.c_str(), msg.size(), &written, nullptr);
        WriteConsoleA(s_outHandle, "\n", 1, &written, nullptr);
        return;
    }

    int color = 0;
    int color2 = -1;
    switch (severity) {
        case Severity::Debug:
            color = 243;
            color2 = 250;
            break;
        case Severity::Info:
            color = 33;
            color2 = 254;
            break;
        case Severity::Warning:
            color = 229;
            color2 = 230;
            break;
        case Severity::Error:
            color = 9;
            color2 = 224;
            break;
        default:
            color = 7;
            break;
    }
    auto const colorStr = fmt::format("\x1b[38;5;{}m", color);
    auto const color2Str = color2 == -1 ? "\x1b[0m" : fmt::format("\x1b[38;5;{}m", color2);
    auto const newMsg = fmt::format(
        "{}{}{}{}\x1b[0m\n",
        colorStr, msg.substr(0, 14), color2Str, msg.substr(14)
    );

    if (s_origStdOut) {
        fwrite(newMsg.c_str(), 1, newMsg.size(), s_origStdOut);
        fflush(s_origStdOut);
    }
    WriteConsoleA(s_outHandle, newMsg.c_str(), newMsg.size(), &written, nullptr);
}

void console::messageBox(char const* title, std::string const& info, Severity severity) {
    unsigned int icon;
    switch (severity) {
        case Severity::Debug:
        case Severity::Info:
            icon = MB_ICONINFORMATION;
            break;
        case Severity::Warning:
            icon = MB_ICONWARNING;
            break;
        default:
            icon = MB_ICONERROR;
            break;
    }
    MessageBoxA(nullptr, info.c_str(), title, icon);
}
