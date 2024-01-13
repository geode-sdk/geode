#include <loader/console.hpp>
#include <loader/LogImpl.hpp>
#include <iostream>

using namespace geode::prelude;

bool s_isOpen = false;
bool s_hasAnsiColorSupport = false;

void console::open() {
    if (s_isOpen) return;
    if (AllocConsole() == 0) return;
    SetConsoleCP(CP_UTF8);
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    // Set output mode to handle ansi color sequences
    auto handleStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD consoleMode = 0;
    if (GetConsoleMode(handleStdout, &consoleMode)) {
        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (SetConsoleMode(handleStdout, consoleMode)) {
            s_hasAnsiColorSupport = true;
        }
    }

    s_isOpen = true;

    for (auto const& log : log::Logger::get()->list()) {
        console::log(log.toString(true), log.getSeverity());
    }
}

void console::close() {
    if (!s_isOpen) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();

    s_isOpen = false;
}

void console::log(std::string const& msg, Severity severity) {
    if (!s_isOpen)
        return;

    if (!s_hasAnsiColorSupport) {
        std::cout << msg << "\n" << std::flush;
        return;
    }

    int color = 0;
    switch (severity) {
        case Severity::Debug:
            color = 243;
            break;
        case Severity::Info:
            color = 33;
            break;
        case Severity::Warning:
            color = 229;
            break;
        case Severity::Error:
            color = 9;
            break;
        default:
            color = 7;
            break;
    }
    auto const colorStr = fmt::format("\x1b[38;5;{}m", color);
    auto const newMsg = fmt::format("{}{}\x1b[0m{}", colorStr, msg.substr(0, 12),
        msg.substr(12));

    std::cout << newMsg << "\n" << std::flush;
}

void console::messageBox(char const* title, std::string const& info, Severity severity) {
    unsigned int icon;
    switch (severity) {
        case Severity::Debug:
        case Severity::Info:
        case Severity::Notice:
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
