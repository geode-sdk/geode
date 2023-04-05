#pragma once

#include <array>
#include <broma.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <ghc/filesystem.hpp>
#include <fstream>

using std::istreambuf_iterator;

#ifdef _MSC_VER
    #pragma warning(disable : 4996)
#endif

#if _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

std::string generateAddressHeader(Root& root);
std::string generateModifyHeader(Root& root, ghc::filesystem::path const& singleFolder);
std::string generateWrapperHeader(Root& root);
std::string generateTypeHeader(Root& root);
std::string generateBindingHeader(Root& root, ghc::filesystem::path const& singleFolder);
std::string generatePredeclareHeader(Root& root);
std::string generateBindingSource(Root& root);
std::string generateTidyHeader(Root& root);

inline void writeFile(ghc::filesystem::path const& writePath, std::string const& output) {
    std::ifstream readfile;
    readfile >> std::noskipws;
    readfile.open(writePath);
    std::string data((std::istreambuf_iterator<char>(readfile)), std::istreambuf_iterator<char>());
    readfile.close();

    if (data != output) {
        std::ofstream writefile;
        writefile.open(writePath);
        writefile << output;
        writefile.close();
    }
}

inline std::string str_if(std::string&& str, bool cond) {
    return cond ? str : "";
}

inline bool can_find(std::string const& str, char const* text) {
    return str.find(text) != std::string::npos;
}

inline bool is_cocos_class(std::string const& str) {
    return can_find(str, "cocos2d") || can_find(str, "pugi::") || str == "DS_Dictionary";
}

enum class BindStatus {
    Binded,
    NeedsBinding,
    Unbindable
};

struct codegen_error : std::runtime_error {
    inline codegen_error(char const* msg) : std::runtime_error(msg) {}
};

namespace codegen {

    template <typename... Args>
    inline codegen_error error(Args... args) {
        return codegen_error(fmt::format(args...).c_str());
    }

    inline Platform platform;

    inline uintptr_t platformNumberWithPlatform(Platform p, PlatformNumber const& pn) {
        switch (p) {
            case Platform::Mac: return pn.mac;
            case Platform::Windows: return pn.win;
            case Platform::iOS: return pn.ios;
            case Platform::Android: return pn.android;
            default: // unreachable
                return pn.win;
        }
    }

    inline uintptr_t platformNumber(PlatformNumber const& p) {
        return platformNumberWithPlatform(codegen::platform, p);
    }

    inline BindStatus getStatusWithPlatform(Platform p, Field const& field) {
        FunctionBegin const* fb;

        if (auto fn = field.get_as<FunctionBindField>()) {
            if (platformNumberWithPlatform(p, fn->binds)) return BindStatus::NeedsBinding;

            fb = &fn->beginning;
        }
        else if (auto fn = field.get_as<OutOfLineField>()) {
            fb = &fn->beginning;
        }
        else return BindStatus::Unbindable;

        // if (field.parent.rfind("GDString", 0) == 0) return BindStatus::NeedsBinding;

        if (p == Platform::Android) {
            for (auto& [type, name] : fb->args) {
                if (type.name.find("gd::") != std::string::npos) return BindStatus::NeedsBinding;
            }

            if (field.parent.rfind("cocos2d::CCEGLView", 0) == 0) return BindStatus::Unbindable;

            return BindStatus::Binded;
        }
        
        if (fb->type == FunctionType::Normal) {
            if (field.parent.rfind("fmod::", 0) == 0) return BindStatus::Binded;
            if (
                (field.parent.rfind("cocos2d::", 0) == 0 || field.parent == "DS_Dictionary") &&
                p == Platform::Windows
            ) return BindStatus::Binded;
        }

        return BindStatus::Unbindable;
    }

    inline BindStatus getStatus(Field const& field) {
        return getStatusWithPlatform(codegen::platform, field);
    }

    inline std::string getParameters(FunctionBegin const& f) { // int p0, float p1
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(fmt::format("{} {}", t.name, n));
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getParameterTypes(FunctionBegin const& f) { // int, float
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(t.name);
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getParameterNames(FunctionBegin const& f) { // p0, p1
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(n);
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getModifyConventionName(Field& f) {
        if (codegen::platform != Platform::Windows) return "Default";

        if (auto fn = f.get_fn()) {
            auto status = getStatus(f);

            if (fn->is_static) {
                if (status == BindStatus::Binded) return "Cdecl";
                else return "Optcall";
            }
            else if (fn->is_virtual || fn->is_callback) {
                return "Thiscall";
            }
            else {
                if (status == BindStatus::Binded) return "Thiscall";
                else return "Membercall";
            }
        }
        else throw codegen::error("Tried to get convention of non-function");
    }

    inline std::string getConvention(Field& f) {
        if (codegen::platform != Platform::Windows) return "DefaultConv";

        return std::string("x86::") + getModifyConventionName(f);
    }

    inline std::string getModifyConvention(Field& f) {
        if (codegen::platform != Platform::Windows) return "tulip::hook::DefaultConvention";

        return std::string("tulip::hook::") + getModifyConventionName(f) + "Convention";
    }

    inline std::string getUnqualifiedClassName(std::string const& s) {
        auto index = s.rfind("::");
        if (index == std::string::npos) return s;
        return s.substr(index + 2);
    }
}
