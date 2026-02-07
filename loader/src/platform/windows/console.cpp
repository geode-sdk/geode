#include <loader/console.hpp>
#include <loader/LogImpl.hpp>
#include <io.h>
#include <Geode/utils/string.hpp>
#include <Geode/utils/general.hpp>
#include <arc/iocp/IocpPipe.hpp>

using namespace geode::prelude;

HANDLE s_outHandle = nullptr;
bool s_useEscapeCodes = false;

void setupConsole(bool forceUseEscapeCodes = false) {
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

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
            WriteFile(s_outHandle, "\x1b[0m", 4, &written, nullptr) &&
            GetConsoleScreenBufferInfo(s_outHandle, &postInfo)) {
            s_useEscapeCodes = preInfo.dwCursorPosition.X == postInfo.dwCursorPosition.X &&
                preInfo.dwCursorPosition.Y == postInfo.dwCursorPosition.Y;
            SetConsoleCursorPosition(s_outHandle, preInfo.dwCursorPosition);
        }
    }
}

bool redirectStd(FILE* which, ZStringView name, const Severity sev) {
    auto pipeName = utils::string::utf8ToWide(fmt::format(R"(\\.\pipe\geode-{}-{})", name, GetCurrentProcessId()));
    auto pipe = CreateNamedPipeW(
        pipeName.c_str(),
        PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED,
        PIPE_REJECT_REMOTE_CLIENTS,
        1, 0, 1024, 0, nullptr
    );
    if (!pipe) {
        log::warn("Failed to create pipe, {} will be unavailable", name);
        return false;
    }

    FILE* yum;
    if (freopen_s(&yum, utils::string::wideToUtf8(pipeName).c_str(), "w", which)) {
        log::warn("Failed to reopen file, {} will be unavailable", name);
        return false;
    }

    async::spawn([sev, name, handle = pipe] mutable -> arc::Future<> {
        auto piperes = arc::IocpPipe::open(handle);
        if (!piperes) {
            log::warn("Failed to open IocpPipe, {} will be unavailable: {}", name, piperes.unwrapErr());
            co_return;
        }
        auto pipe = std::move(piperes).unwrap();

        std::string line;

        while (true) {
            char buf[1024];
            auto res = co_await pipe.read(buf, 1024);

            if (!res) {
                log::warn("Error reading from {} pipe: {}", name, res.unwrapErr());
                break;
            }

            // often we get tiny reads that may not be a whole line yet, so buffer until a full line
            auto readBytes = res.unwrap();
            std::string_view view{buf, readBytes};

            for (char c : view) {
                if (c == '\r') continue;
                if (c == '\n') {
                    // complete line
                    log::Logger::get()->push(sev, 0, line, "", name, nullptr);
                    line.clear();
                    continue;
                }
                
                line.push_back(c);
            }
        }
    }).setName(fmt::format("{} Read Task", name));

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
            std::array<wchar_t, MAX_PATH + 1> buf;
            auto count = GetFinalPathNameByHandleW(s_outHandle, buf.data(), buf.size(),
                FILE_NAME_OPENED | VOLUME_NAME_NT);
            if (count != 0) {
                path = utils::string::wideToUtf8(std::wstring{buf.data(), count - 1});
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
        // allow the user to forcefully enable colors via an environment variable too

        setupConsole(
            string::contains(path, "cidr-")
            || geode::utils::getEnvironmentVariable("GEODE_FORCE_ENABLE_TERMINAL_COLORS") != "0" // prefer to use FORCE_COLOR=1
            || geode::utils::getEnvironmentVariable("FORCE_COLOR") != "0"
        );
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
        s_outHandle = CreateFileW(L"CONOUT$", GENERIC_WRITE, 0, nullptr, 0, 0, nullptr);
        SetStdHandle(STD_OUTPUT_HANDLE, s_outHandle);
        SetStdHandle(STD_INPUT_HANDLE, CreateFileW(L"CONIN$", GENERIC_READ, 0, nullptr, 0, 0, nullptr));
        SetStdHandle(STD_ERROR_HANDLE, s_outHandle);
    }
    setupConsole();
}

void console::log(ZStringView msg, Severity severity) {
    if (!s_outHandle)
        return;
    DWORD written;

    if (!s_useEscapeCodes || msg.size() <= 14) {
        WriteFile(s_outHandle, msg.c_str(), msg.size(), &written, nullptr);
        WriteFile(s_outHandle, "\n", 1, &written, nullptr);
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
    
    std::string_view sv{msg};

    // this is suboptimal but slightly better than hardcoding '14' which is what it was before
    size_t colorEnd = sv.find_first_of('[') - 1;

    auto str = fmt::format("\x1b[38;5;{}m{}\x1b[0m{}\n", color, sv.substr(0, colorEnd), sv.substr(colorEnd));
    WriteFile(s_outHandle, str.c_str(), str.size(), &written, nullptr);
}

void console::messageBox(ZStringView title, ZStringView info, Severity severity) {
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
    auto winfo = string::utf8ToWide(info);
    auto wtitle = string::utf8ToWide(title);

    MessageBoxW(nullptr, winfo.c_str(), wtitle.c_str(), icon);
}
