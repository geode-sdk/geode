#include <Windows.h>

#include "nfdwin.hpp"
#include <Geode/utils/string.hpp>
#include <algorithm>

using Path = asp::fs::Path;
using Paths = std::vector<asp::fs::Path>;

static BOOL COMIsInitialized(HRESULT coResult) {
    if (coResult == RPC_E_CHANGED_MODE) {
        // If COM was previously initialized with different init flags,
        // NFD still needs to operate. Eat this warning.
        return true;
    }
    return SUCCEEDED(coResult);
}

static HRESULT COMInit() {
    return CoInitializeEx(
        nullptr,
        COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE
    );
}

static void COMUninit(HRESULT coResult) {
    // do not uninitialize if RPC_E_CHANGED_MODE occurred -- this
    // case does not refcount COM.
    if (SUCCEEDED(coResult)) {
        CoUninitialize();
    }
}

static std::pair<std::wstring, std::wstring> transformFilter(
    file::FilePickOptions::Filter const& filter
) {
    std::wstring extensions {};
    bool first = true;
    for (auto& ext : filter.files) {
        if (first) {
            first = false;
        } else {
            extensions += L';';
        }
        extensions += string::utf8ToWide(ext);
    }
    return {
        string::utf8ToWide(filter.description),
        extensions,
    };
}

static bool addFiltersToDialog(
    ::IFileDialog *dialog,
    std::vector<file::FilePickOptions::Filter> const& filters
) {
    if (!filters.size()) {
        return true;
    }
    std::vector<std::pair<std::wstring, std::wstring>> wfilters {};
    wfilters.reserve(filters.size());
    std::transform(
        filters.begin(),
        filters.end(),
        std::back_inserter(wfilters),
        transformFilter
    );
    wfilters.push_back({ L"All Files", L"*.*" });
    std::vector<COMDLG_FILTERSPEC> specList {};
    specList.reserve(filters.size() + 1);
    for (auto& filter : wfilters) {
        specList.push_back(
            {filter.first.c_str(),
            filter.second.c_str()}
        );
    }
    return SUCCEEDED(
        dialog->SetFileTypes(specList.size(), specList.data())
    );
}

static Result<Paths> convShellItems(IShellItemArray* shellItems) {
    DWORD shellItemCount;
    if (!SUCCEEDED(shellItems->GetCount(&shellItemCount))) {
        return Err("Unable to get shell item count");
    }

    std::vector<asp::fs::Path> paths;
    for (DWORD i = 0; i < shellItemCount; i++) {
        IShellItem* shellItem;
        if (!SUCCEEDED(shellItems->GetItemAt(i, &shellItem))) {
            return Err("Unable to get shell item");
        }

        SFGAOF attribs;
        if (!SUCCEEDED(shellItem->GetAttributes(SFGAO_FILESYSTEM, &attribs))) {
            return Err("Unable to get shell item attributes");
        }
        if (!(attribs & SFGAO_FILESYSTEM)) {
            continue;
        }

        LPWSTR name;
        shellItem->GetDisplayName(SIGDN_FILESYSPATH, &name);

        paths.push_back(GEODE_UNWRAP(asp::fs::Path::fromWideResult(name)));

        CoTaskMemFree(name);
    }
    return Ok(paths);
}

static bool setDefaultPath(
    IFileDialog* dialog,
    asp::fs::PathView defaultPath
) {
    IShellItem* folder;
    if (!SUCCEEDED(SHCreateItemFromParsingName(
        defaultPath.native().c_str(), nullptr,
        IID_PPV_ARGS(&folder)
    ))) {
        return false;
    }
    dialog->SetFolder(folder);
    folder->Release();
    return true;
}

static bool setDefaultFile(
    IFileDialog* dialog,
    asp::fs::PathView fileName
) {
    dialog->SetFileName(fileName.native().c_str());
    return true;
}

template<class T>
struct Holder {
    T m_deallocator;
    Holder(T&& deallocator) : m_deallocator(deallocator) {}
    ~Holder() {
        m_deallocator();
    }
};

Result<> nfdPick(
    NFDMode mode,
    file::FilePickOptions const& options,
    void* result,
    HWND parent
) {
    auto coResult = COMInit();
    if (!COMIsInitialized(coResult)) {
        return Err("Could not initialize COM");
    }

    auto clsid = (
        mode == NFDMode::SaveFile ?
        CLSID_FileSaveDialog :
        CLSID_FileOpenDialog
    );
    auto iid = (
        mode == NFDMode::SaveFile ?
        IID_IFileSaveDialog :
        IID_IFileOpenDialog
    );

    IFileDialog* dialog = nullptr;
    Holder _([&]() {
        if (dialog) {
            dialog->Release();
        }
        COMUninit(coResult);
    });

    if (!SUCCEEDED(CoCreateInstance(
        clsid, nullptr, CLSCTX_ALL, iid,
        reinterpret_cast<void**>(&dialog)
    ))) {
        return Err("Could not create dialog");
    }

    if (!addFiltersToDialog(dialog, options.filters)) {
        return Err("Unable to add filters to dialog");
    }


    if (options.defaultPath) {
        auto path = *options.defaultPath;
        path.makePreferred();
        if (mode == NFDMode::OpenFile || mode == NFDMode::SaveFile) {
            if (!path.isDir()) {
                if (auto filename = path.filename()) {
                    setDefaultFile(dialog, *filename);
                }
                if (auto parent = path.parent()) {
                    path = *parent;
                }
                else {
                    path = "";
                }
            }
        }
        if (!path.empty() && !setDefaultPath(dialog, path)) {
            return Err("Unable to set default path to dialog");
        }
    }

    if (mode == NFDMode::OpenFiles) {
        DWORD flags;
        if (!SUCCEEDED(dialog->GetOptions(&flags))) {
            return Err("Unable to get dialog options");
        }
        if (!SUCCEEDED(
            dialog->SetOptions(flags | FOS_ALLOWMULTISELECT)
        )) {
            return Err("Unable to set dialog options");
        }
    }
    else if (mode == NFDMode::OpenFolder) {
        DWORD flags;
        if (!SUCCEEDED(dialog->GetOptions(&flags))) {
            return Err("Unable to get dialog options");
        }
        if (!SUCCEEDED(
            dialog->SetOptions(flags | FOS_PICKFOLDERS)
        )) {
            return Err("Unable to set dialog options");
        }
    }

    auto taskbar = FindWindow("Shell_TrayWnd", NULL);
    SetForegroundWindow(taskbar);
    auto dialogResult = dialog->Show(parent);
    ShowWindow(parent, SW_RESTORE);

    switch (dialogResult) {
        case S_OK: {
            switch (mode) {
                case NFDMode::OpenFile:
                case NFDMode::SaveFile: {
                    IShellItem* shellItem = nullptr;
                    if (!SUCCEEDED(dialog->GetResult(&shellItem))) {
                        return Err("Could not get result from dialog");
                    }
                    Holder _([&]() {
                        shellItem->Release();
                    });

                    wchar_t* filePath = nullptr;
                    if (!SUCCEEDED(
                        shellItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath)
                    )) {
                        return Err("Could not get path from result");
                    }
                    *reinterpret_cast<Path*>(result) = GEODE_UNWRAP(Path::fromWideResult(filePath));
                    CoTaskMemFree(filePath);

                    return Ok();
                } break;

                case NFDMode::OpenFiles: {
                    IShellItemArray *shellItems;
                    if (!SUCCEEDED(static_cast<IFileOpenDialog*>(
                        dialog
                    )->GetResults(&shellItems))) {
                        return Err("Could not get results from dialog");
                    }
                    Holder _([&]() {
                        shellItems->Release();
                    });
                    GEODE_UNWRAP_INTO(*reinterpret_cast<Paths*>(result), convShellItems(shellItems));
                    return Ok();
                } break;

                case NFDMode::OpenFolder: {
                    IShellItem* shellItem = nullptr;
                    if (!SUCCEEDED(dialog->GetResult(&shellItem))) {
                        return Err("Could not get result from dialog");
                    }
                    Holder _([&]() {
                        shellItem->Release();
                    });

                    wchar_t* filePath = nullptr;
                    if (!SUCCEEDED(
                        shellItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &filePath)
                    )) {
                        return Err("Could not get path from result");
                    }

                    *reinterpret_cast<Path*>(result) = GEODE_UNWRAP(Path::fromWideResult(filePath));
                    CoTaskMemFree(filePath);

                    return Ok();
                } break;
            }
        } break;

        case HRESULT_FROM_WIN32(ERROR_CANCELLED): {
            return Err("Dialog cancelled");
        } break;

        default: break;
    }

    return Err("Unknown error");
}
