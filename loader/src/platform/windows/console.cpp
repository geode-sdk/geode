#include <loader/console.hpp>
#include <loader/LogImpl.hpp>
#include <io.h>

using namespace geode::prelude;

HANDLE s_outHandle = nullptr;
bool s_hasAnsiColorSupport = false;
bool s_shouldAlloc = false;

void setupConsole(HANDLE out) {
    SetConsoleCP(CP_UTF8);

    DWORD consoleMode = 0;

    // Set output mode to handle ansi color sequences
    s_hasAnsiColorSupport = GetConsoleMode(out, &consoleMode) &&
        ((consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) > 0 ||
            SetConsoleMode(out, consoleMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING));

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

    s_outHandle = out;
}

void finishSetup() {
    for (auto const& log : log::Logger::get()->list()) {
        console::log(log.toString(), log.getSeverity());
    }
}

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
    // if the game launched from a console or with a console already attached,
    // this is where we find that out and save its handle
    auto outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!outHandle) {
        if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
            s_shouldAlloc = true;
        }
        else {
            outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            if (!outHandle) {
                s_shouldAlloc = true;
            }
        }
    }
    else {
        // explicitly ignore some stupid handles
        char buf[MAX_PATH + 1];
        auto count = GetFinalPathNameByHandleA(outHandle, buf, MAX_PATH + 1, FILE_NAME_OPENED | VOLUME_NAME_NT);
        if (count != 0) {
            std::string path(buf, count - 1);
            // proton redirects stdout to /dev/null for some reason??
            // vscode redirects output to a named pipe and then doesn't use it
            if (path.ends_with("\\dev\\null") || string::contains(path, "\\uv\\0000")) {
                outHandle = nullptr;
                s_shouldAlloc = true;
                CloseHandle(GetStdHandle(STD_OUTPUT_HANDLE));
                CloseHandle(GetStdHandle(STD_INPUT_HANDLE));
                CloseHandle(GetStdHandle(STD_ERROR_HANDLE));
                FreeConsole();
                SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
                SetStdHandle(STD_INPUT_HANDLE, nullptr);
                SetStdHandle(STD_ERROR_HANDLE, nullptr);
            }
        }
    }

    // setup the already attached console
    if (!s_shouldAlloc)
        setupConsole(outHandle);

    auto oldStdout = _dup(_fileno(stdout));

    redirectStd(stdout, "stdout", Severity::Info);
    redirectStd(stderr, "stderr", Severity::Debug);

    // re-open the file from the handle we just stole..
    if (oldStdout >= 0) {
        _fdopen(oldStdout, "w");
        s_outHandle = reinterpret_cast<HANDLE>(_get_osfhandle(oldStdout));
    }

    if (!s_shouldAlloc)
        finishSetup();
}

void console::openIfClosed() {
    if (!s_shouldAlloc)
        return;
    s_shouldAlloc = false;
    AllocConsole();
    // reopen conin$/conout$ if they're closed
    if (!GetStdHandle(STD_OUTPUT_HANDLE)) {
        SetStdHandle(STD_OUTPUT_HANDLE, CreateFileA("CONOUT$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr));
        SetStdHandle(STD_INPUT_HANDLE, CreateFileA("CONIN$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr));
        SetStdHandle(STD_ERROR_HANDLE, GetStdHandle(STD_OUTPUT_HANDLE));
    }
    setupConsole(GetStdHandle(STD_OUTPUT_HANDLE));
    finishSetup();
}

void console::log(std::string const& msg, Severity severity) {
    if (!s_outHandle)
        return;
    DWORD written;

    if (!s_hasAnsiColorSupport) {
        WriteFile(s_outHandle, (msg + "\n").c_str(), msg.size() + 1, &written, nullptr);
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

    WriteFile(s_outHandle, newMsg.c_str(), newMsg.size(), &written, nullptr);
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
