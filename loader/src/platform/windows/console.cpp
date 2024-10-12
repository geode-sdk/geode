#include <loader/console.hpp>
#include <loader/LogImpl.hpp>
#include <io.h>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;

HANDLE s_outHandle = nullptr;
bool s_useEscapeCodes = false;

void setupConsole(bool forceUseEscapeCodes = false) {
    SetConsoleCP(CP_UTF8);

    // set output mode to handle ansi color sequences
    DWORD consoleMode = 0;
    s_useEscapeCodes = forceUseEscapeCodes || GetConsoleMode(s_outHandle, &consoleMode) &&
        SetConsoleMode(s_outHandle, consoleMode | ENABLE_PROCESSED_OUTPUT |
            ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    if (s_useEscapeCodes && !forceUseEscapeCodes) {
        // test if the console *actually* supports escape codes (thanks wine)
        s_useEscapeCodes = false;
        DWORD written;
        CONSOLE_SCREEN_BUFFER_INFO preInfo;
        CONSOLE_SCREEN_BUFFER_INFO postInfo;
        if (GetConsoleScreenBufferInfo(s_outHandle, &preInfo) &&
            WriteConsoleA(s_outHandle, "\x1b[0m", 4, &written, nullptr) &&
            GetConsoleScreenBufferInfo(s_outHandle, &postInfo)) {
            s_useEscapeCodes = preInfo.dwCursorPosition.X == postInfo.dwCursorPosition.X &&
                preInfo.dwCursorPosition.Y == postInfo.dwCursorPosition.Y;
            SetConsoleCursorPosition(s_outHandle, preInfo.dwCursorPosition);
        }
    }

    for (auto const& log : log::Logger::get()->list()) {
        console::log(log.toString(), log.getSeverity());
    }
}

struct stdData {
    OVERLAPPED m_overlap{};
    std::string const& m_name;
    const Severity m_sev;
    std::string& m_cur;
    char* m_buf;
    stdData(std::string const& name, const Severity sev, std::string& cur, char* buf) :
        m_name(name), m_sev(sev), m_cur(cur), m_buf(buf) { }
};
void WINAPI CompletedReadRoutine(DWORD error, DWORD read, LPOVERLAPPED overlap) {
    auto* o = reinterpret_cast<stdData*>(overlap);
    for (auto i = 0; i < read && !error; i++) {
        if (o->m_buf[i] != '\n') {
            if (o->m_buf[i] != '\r')
                o->m_cur += o->m_buf[i];
            continue;
        }
        log::Logger::get()->push(o->m_sev, "", std::string(o->m_name), 0, std::string(o->m_cur));
        o->m_cur.clear();
    }
    delete o;
}

bool redirectStd(FILE* which, std::string const& name, const Severity sev) {
    auto pipeName = fmt::format(R"(\\.\pipe\geode-{}-{})", name, GetCurrentProcessId());
    auto pipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_REJECT_REMOTE_CLIENTS,
        1, 0, 1024, 0, nullptr
    );
    if (!pipe) {
        log::warn("Failed to create pipe, {} will be unavailable", name);
        return false;
    }
    std::thread([pipe, name, sev]() {
        thread::setName(fmt::format("{} Read Thread", name));
        auto event = CreateEventA(nullptr, false, false, nullptr);
        std::string cur;
        while (true) {
            char buf[1024];
            auto* data = new stdData(name, sev, cur, buf);
            data->m_overlap.hEvent = event;
            ReadFileEx(pipe, buf, 1024, &data->m_overlap, &CompletedReadRoutine);
            auto res = WaitForSingleObjectEx(event, INFINITE, true);
            if (!res)
                continue;
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
    s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!s_outHandle && AttachConsole(ATTACH_PARENT_PROCESS)) {
        s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    if (s_outHandle) {
        std::string path;
        DWORD dummy;
        // use GetConsoleMode to check if the handle is a console
        if (!GetConsoleMode(s_outHandle, &dummy)) {
            // explicitly ignore some stupid handles
            char buf[MAX_PATH + 1];
            auto count = GetFinalPathNameByHandleA(s_outHandle, buf, MAX_PATH + 1,
                FILE_NAME_OPENED | VOLUME_NAME_NT);
            if (count != 0) {
                path = std::string(buf, count - 1);
            }

            // TODO: this code causes a crash when piping game's output somewhere (and in some other cases), so it's removed for now
            // // count == 0 => not a console and not a file, assume it's closed
            // // wine does something weird with /dev/null? not sure tbh but it's definitely up to no good
            // if ((count == 0 || path.ends_with("\\dev\\null"))) {
            //     s_outHandle = nullptr;
            //     CloseHandle(GetStdHandle(STD_OUTPUT_HANDLE));
            //     CloseHandle(GetStdHandle(STD_INPUT_HANDLE));
            //     CloseHandle(GetStdHandle(STD_ERROR_HANDLE));
            //     FreeConsole();
            //     SetStdHandle(STD_OUTPUT_HANDLE, nullptr);
            //     SetStdHandle(STD_INPUT_HANDLE, nullptr);
            //     SetStdHandle(STD_ERROR_HANDLE, nullptr);
            // }
        }

        // clion console supports escape codes but we can't query that because it's a named pipe
        setupConsole(string::contains(path, "cidr-"));
    }

    auto oldStdout = _dup(_fileno(stdout));

    redirectStd(stdout, "stdout", Severity::Info);
    redirectStd(stderr, "stderr", Severity::Debug);

    // re-open the file from the handle we just stole..
    if (oldStdout >= 0) {
        _fdopen(oldStdout, "w");
        s_outHandle = reinterpret_cast<HANDLE>(_get_osfhandle(oldStdout));
    }
}

void console::openIfClosed() {
    if (s_outHandle)
        return;
    AllocConsole();
    s_outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    // reopen conin$/conout$ if they're closed
    if (!s_outHandle) {
        s_outHandle = CreateFileA("CONOUT$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr);
        SetStdHandle(STD_OUTPUT_HANDLE, s_outHandle);
        SetStdHandle(STD_INPUT_HANDLE, CreateFileA("CONIN$", GENERIC_READ, 0, nullptr, 0, 0, nullptr));
        SetStdHandle(STD_ERROR_HANDLE, s_outHandle);
    }
    setupConsole();
}

void console::log(std::string const& msg, Severity severity) {
    if (!s_outHandle)
        return;
    DWORD written;

    if (!s_useEscapeCodes || msg.size() <= 14) {
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
