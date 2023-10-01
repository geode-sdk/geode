#pragma once

#include <array>
#include <broma.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>
#include <ghc/filesystem.hpp>

using std::istreambuf_iterator;

#ifdef _MSC_VER
    #pragma warning(disable : 4996)
#endif

using namespace broma;

std::string generateAddressHeader(Root const& root);
std::string generateModifyHeader(Root const& root, ghc::filesystem::path const& singleFolder);
std::string generateBindingHeader(Root const& root, ghc::filesystem::path const& singleFolder);
std::string generatePredeclareHeader(Root const& root);
std::string generateBindingSource(Root const& root);

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
    Unbindable,
    Missing,
};

struct codegen_error : std::runtime_error {
    inline codegen_error(char const* msg) : std::runtime_error(msg) {}
};

namespace codegen {
    extern std::map<void const*, size_t> idMap;

    inline void populateIds(Root const& root) {
        size_t id = 0;
        for (auto& f : root.functions) {
            idMap[&f] = id++;
        }
        
        for (auto& c : root.classes) {
            for (auto& f : c.fields) {
                if (auto fn = f.get_as<FunctionBindField>()) {
                    idMap[&f] = id++;
                }
            }
        }
    }

    inline size_t getId(Function const* f) {
        return idMap[f];
    }

    inline size_t getId(Field const* f) {
        return idMap[f];
    }

    template <typename... Args>
    inline codegen_error error(Args... args) {
        return codegen_error(fmt::format(args...).c_str());
    }

    inline Platform platform;

    inline ptrdiff_t platformNumberWithPlatform(Platform p, PlatformNumber const& pn) {
        switch (p) {
            case Platform::Mac: return pn.mac;
            case Platform::Windows: return pn.win;
            case Platform::iOS: return pn.ios;
            case Platform::Android: return pn.android;
            default: // unreachable
                return pn.win;
        }
    }

    inline ptrdiff_t platformNumber(PlatformNumber const& p) {
        return platformNumberWithPlatform(codegen::platform, p);
    }

    inline uintptr_t platformNumber(Field const& field) {
        if (auto fn = field.get_as<FunctionBindField>()) {
            return platformNumberWithPlatform(codegen::platform, fn->binds);
        }
        return 0;
    }

    inline BindStatus getStatusWithPlatform(Platform p, Field const& field) {
        if ((field.missing & p) != Platform::None) return BindStatus::Missing;

        if (auto fn = field.get_as<FunctionBindField>()) {
            if ((field.links & p) != Platform::None) return BindStatus::Binded;
            if (platformNumberWithPlatform(p, fn->binds) != -1) return BindStatus::NeedsBinding;
        }

        return BindStatus::Unbindable;
    }

    inline BindStatus getStatusWithPlatform(Platform p, Function const& f) {
        if ((f.missing & p) != Platform::None) return BindStatus::Missing;

        if ((f.links & p) != Platform::None) return BindStatus::Binded;
        if (platformNumberWithPlatform(p, f.binds) != -1) return BindStatus::NeedsBinding;

        return BindStatus::Unbindable;
    }

    inline bool shouldAndroidBind(const FunctionBindField* fn) {
        if (codegen::platform == Platform::Android) {
            if (fn->prototype.type != FunctionType::Normal) return true;
            for (auto& [type, name] : fn->prototype.args) {
                if (can_find(type.name, "gd::")) return true;
            }
        }
        return false;
    }

    inline BindStatus getStatus(Field const& field) {
        return getStatusWithPlatform(codegen::platform, field);
    }

    inline BindStatus getStatus(Function const& f) {
        return getStatusWithPlatform(codegen::platform, f);
    }

    inline std::string getParameters(FunctionProto const& f) { // int p0, float p1
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(fmt::format("{} {}", t.name, n));
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getParameterTypes(FunctionProto const& f) { // int, float
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(t.name);
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getParameterNames(FunctionProto const& f) { // p0, p1
        std::vector<std::string> parameters;

        for (auto& [t, n] : f.args) {
            parameters.push_back(n);
        }

        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    inline std::string getModifyConventionName(Field const& f) {
        if (codegen::platform != Platform::Windows) return "Default";

        if (auto fn = f.get_as<FunctionBindField>()) {
            auto status = getStatus(f);

            if (fn->prototype.is_static) {
                if (status == BindStatus::Binded) return "Cdecl";
                else return "Optcall";
            }
            else if (fn->prototype.is_virtual || fn->prototype.is_callback) {
                return "Thiscall";
            }
            else {
                if (status == BindStatus::Binded) return "Thiscall";
                else return "Membercall";
            }
        }
        else throw codegen::error("Tried to get convention of non-function");
    }

    inline std::string getModifyConventionName(Function const& f) {
        if (codegen::platform != Platform::Windows) return "Default";

        return "Cdecl";
    }

    inline std::string getConvention(Field const& f) {
        if (codegen::platform != Platform::Windows) return "DefaultConv";

        return std::string("x86::") + getModifyConventionName(f);
    }

    inline std::string getModifyConvention(Field const& f) {
        if (codegen::platform != Platform::Windows) return "tulip::hook::DefaultConvention";

        return std::string("tulip::hook::") + getModifyConventionName(f) + "Convention";
    }

    inline std::string getUnqualifiedClassName(std::string const& s) {
        auto index = s.rfind("::");
        if (index == std::string::npos) return s;
        return s.substr(index + 2);
    }
}
