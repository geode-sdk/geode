#define ISOLATION_AWARE_ENABLED 1

#include <Windows.h>

#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/general.hpp>

#include <string>
#include <filesystem>

// comctl32 v6
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

enum {
    ID_CRASHLOG_TEXT = 101,
    ID_BUTTON_CLOSE = 102,
    ID_BUTTON_OPEN_FOLDER = 103,
    ID_BUTTON_COPY_CLIPBOARD = 104,
};
#define TO_HMENU(x) reinterpret_cast<HMENU>(static_cast<size_t>(x))

namespace layout {
    static constexpr int CRASHLOG_FONT_SIZE = 16;
    static constexpr int BUTTON_HEIGHT = 30;
    static constexpr int BUTTON_WIDTH = 120;
    static constexpr int BUTTON_SPACING = 10;

    static constexpr int PADDING = 10;
}

// dont judge
std::filesystem::path g_crashlogPath;
std::string g_crashlogText;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_CREATE: {
        {
            // center the window
            RECT desktopRect;
            GetClientRect(GetDesktopWindow(), &desktopRect);

            RECT windowRect;
            GetWindowRect(hwnd, &windowRect);

            auto x = desktopRect.right / 2 - (windowRect.right - windowRect.left) / 2;
            auto y = desktopRect.bottom / 2 - (windowRect.bottom - windowRect.top) / 2;

            SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        }

        auto monoFont = CreateFontA(layout::CRASHLOG_FONT_SIZE, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, TEXT("Consolas"));
        auto guiFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));

        auto handleText = CreateWindowA(
            "EDIT", "Crashlog text goes here", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_LEFT | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_BORDER,
            0, 0, 100, 100,
            hwnd, TO_HMENU(ID_CRASHLOG_TEXT), NULL, NULL
        );
        SendMessage(handleText, WM_SETFONT, WPARAM(monoFont), TRUE);
        // does nothing :(
        // Edit_SetEndOfLine(handleText, EC_ENDOFLINE_LF);

        auto button = CreateWindowA(
            "BUTTON", "Close",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT,
            hwnd, TO_HMENU(ID_BUTTON_CLOSE), NULL, NULL
        );
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE);

        button = CreateWindowA(
            "BUTTON", "Open crashlog folder",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT,
            hwnd, TO_HMENU(ID_BUTTON_OPEN_FOLDER), NULL, NULL
        );
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE);

        button = CreateWindowA(
            "BUTTON", "Copy to clipboard",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0, 0, layout::BUTTON_WIDTH, layout::BUTTON_HEIGHT,
            hwnd, TO_HMENU(ID_BUTTON_COPY_CLIPBOARD), NULL, NULL
        );
        SendMessage(button, WM_SETFONT, WPARAM(guiFont), TRUE);
    } break;

    case WM_SIZE: {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        SetWindowPos(
            GetDlgItem(hwnd, ID_CRASHLOG_TEXT), NULL,
            layout::PADDING, layout::PADDING,
            clientRect.right - layout::PADDING * 2, clientRect.bottom - layout::BUTTON_HEIGHT - layout::PADDING * 3,
            SWP_NOZORDER
        );

        auto buttonY = clientRect.bottom - layout::BUTTON_HEIGHT - layout::PADDING;
        SetWindowPos(
            GetDlgItem(hwnd, ID_BUTTON_COPY_CLIPBOARD), NULL,
            layout::PADDING, buttonY,
            0, 0,
            SWP_NOZORDER | SWP_NOSIZE
        );
        SetWindowPos(
            GetDlgItem(hwnd, ID_BUTTON_CLOSE), NULL,
            clientRect.right - layout::BUTTON_WIDTH - layout::PADDING, buttonY,
            0, 0,
            SWP_NOZORDER | SWP_NOSIZE
        );
        SetWindowPos(
            GetDlgItem(hwnd, ID_BUTTON_OPEN_FOLDER), NULL,
            clientRect.right - layout::BUTTON_WIDTH * 2 - layout::BUTTON_SPACING - layout::PADDING, buttonY,
            0, 0,
            SWP_NOZORDER | SWP_NOSIZE
        );
    } break;

    case WM_CTLCOLORSTATIC: {
        return (LRESULT)(COLOR_WINDOWFRAME);
    } break;

    case WM_COMMAND: {
        auto id = LOWORD(wParam);
        if (id == ID_BUTTON_CLOSE) {
            DestroyWindow(hwnd);
        } else if (id == ID_BUTTON_OPEN_FOLDER) {
            geode::utils::file::openFolder(g_crashlogPath);
        } else if (id == ID_BUTTON_COPY_CLIPBOARD) {
            geode::utils::clipboard::write(g_crashlogText);
        }
    } break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

bool showCustomCrashlogWindow(std::string text, std::filesystem::path const& crashlogPath) {
    static constexpr auto WINDOW_CLASS_NAME = "GeodeCrashHandlerWindow";

    g_crashlogPath = crashlogPath;
    g_crashlogText = text;

    // i cant get the edit control to use LF, so just replace them myself :-)
    for (int i = 0; i < text.size(); ++i) {
        auto c = text[i];
        if (c == '\n') {
            text.insert(text.begin() + i, '\r');
            ++i;
        }
    }

    WNDCLASS wc = {0};
    wc.lpfnWndProc = &WndProc;
    wc.hInstance = GetModuleHandleA(NULL);
    wc.lpszClassName = WINDOW_CLASS_NAME;

    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

    if (!RegisterClass(&wc)) {
        return false;
    }

    auto hwnd = CreateWindowExA(
        0,
        WINDOW_CLASS_NAME,
        "Geode Crash Handler",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, wc.hInstance, NULL
    );

    if (hwnd == NULL) {
        return false;
    }

    SetWindowTextA(GetDlgItem(hwnd, ID_CRASHLOG_TEXT), text.c_str());

    ShowWindow(hwnd, SW_SHOWNORMAL);
    PlaySound((LPCTSTR)SND_ALIAS_SYSTEMDEFAULT, NULL, SND_ASYNC | SND_ALIAS_ID);
    UpdateWindow(hwnd);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    return true;
}
