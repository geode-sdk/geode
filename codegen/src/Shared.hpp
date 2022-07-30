#pragma once

#include <broma.hpp>
#include <array>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fstream>

using std::istreambuf_iterator;

#ifdef _MSC_VER
#pragma warning(disable: 4996)
#endif

#if _WIN32
    #include <direct.h>
#else
    #include <unistd.h>
#endif

std::string generateAddressHeader(Root& root);
std::string generateModifyHeader(Root& root);
std::string generateWrapperHeader(Root& root);
std::string generateTypeHeader(Root& root);
std::string generateGDHeader(Root& root);
std::string generateGDSource(Root& root);
std::string generateTidyHeader(Root& root);

inline std::string str_if(std::string&& str, bool cond) {
    return cond ? str : "";
}

inline bool can_find(std::string const& str, char const* text) {
    return str.find(text) != std::string::npos;
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

    template <typename ...Args>
    inline codegen_error error(Args... args) {
        return codegen_error(fmt::format(args...).c_str());
    }


	inline Platform platform;

	inline uintptr_t platformNumber(PlatformNumber const& p) {
        switch (codegen::platform) {
            case Platform::Mac:
                return p.mac;
            case Platform::Windows:
                return p.win;
            case Platform::iOS:
                return p.ios;
            case Platform::Android:
                return p.android;
            default: // unreachable
                return p.win;
        }
    }

    inline BindStatus getStatus(Field const& field) {
        const FunctionBegin* fb;

        if (auto fn = field.get_as<FunctionBindField>()) {
            if (platformNumber(fn->binds))
                return BindStatus::NeedsBinding;

            fb = &fn->beginning;
        } else if (auto fn = field.get_as<OutOfLineField>()) {
            fb = &fn->beginning;
        } else return BindStatus::Unbindable;

        if (platform == Platform::Android) {
            for (auto& [type, name] : fb->args) {
                if (type.name.find("gd::") != std::string::npos)
                    return BindStatus::NeedsBinding;
            }
            return BindStatus::Binded;
        }

        if (fb->type == FunctionType::Normal) {
            if (field.parent.rfind("fmod::", 0) == 0)
                return BindStatus::Binded;
            if (field.parent.rfind("cocos2d::", 0) == 0 && platform == Platform::Windows)
                return BindStatus::Binded;
        }

        return BindStatus::Unbindable;
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

    inline std::string getConvention(Field& f) {
        if (codegen::platform != Platform::Windows)
            return "DefaultConv";

        if (auto fn = f.get_fn()) {
            auto status = getStatus(f);

            if (fn->is_static) {
                if (status == BindStatus::Binded)
                    return "x86::Cdecl";
                else
                    return "x86::Optcall";
            } else if (fn->is_virtual) {
                return "x86::Thiscall";
            } else {
                if (status == BindStatus::Binded)
                    return "x86::Thiscall";
                else
                    return "x86::Membercall";
            }
        } else throw codegen::error("Tried to get convention of non-function");
    }

    inline std::string getUnqualifiedClassName(std::string const& s) {
        auto index = s.rfind("::");
        if (index == std::string::npos) return s;
        return s.substr(index + 2);
    }

    /*

    // defined : we have a symbol that is usable
    inline bool isFunctionDefined(Function const& f) { 
        // basically this is true for
        // fmod for all platforms
        // cocos for windows
        // all funcs without stl parameter for android
        // destructors for no platforms
        if (f.function_type == kDestructor) return false;
        if (f.function_type == kConstructor) return false;
        if (f.parent_class->name.rfind("fmod::", 0) == 0) return true;
        if (f.parent_class->name.rfind("cocos2d::", 0) == 0 && codegen::platform == kWindows) return true;
        if (getParameterTypes(f).find("gd::", 0) != string::npos && codegen::platform == kAndroid) return false;
        if (codegen::platform == kAndroid) return true;
        return false;
    }

    // definable : we can define it and hook it
    inline bool isFunctionDefinable(Function const& f) {
        // basically this is true for
        // all funcs that we have the offset for
        // all funcs with stl parameter for android
        if (getParameterTypes(f).find("gd::", 0) != string::npos && codegen::platform == kAndroid) return true;
        if (getBind(f) != "") return true;
        return false;
    }

    inline std::string getUnqualifiedClassName(Function const& f) {
    	auto index = f.parent_class->name.rfind("::");
    	if (index == std::string::npos) return f.parent_class->name;
    	return f.parent_class->name.substr(index + 2);
    }

    inline std::string getFunctionName(Function const& f) {
        return f.name;
    }

    inline std::string getConst(Function const& f) {
    	return f.is_const ? "const" : "";
    }

    inline std::string getConstWhitespace(Function const& f) {
    	return f.is_const ? " " : "";
    }

    inline std::string getParameterTypes(Function const& f) { //int, float
    	return fmt::format("{}", fmt::join(f.args, ", "));
    }

    inline std::string getArguments(Function const& f) { // p0, p1
    	return fmt::format("{}", fmt::join(f.argnames, ", "));
    }

    inline std::string getParameterComma(Function const& f) { // int p0, float p1
    	return f.args.size() > 0 ? ", " : "";
    }

    inline std::string getParameterTypeComma(Function const& f) { //int, float
    	return f.args.size() > 0 ? ", " : "";
    }

    inline std::string getArgumentComma(Function const& f) { // p0, p1
    	return f.argnames.size() > 0 ? ", " : "";
    }

    // defined : we have a symbol that is usable
    inline bool isFunctionDefined(Function const& f) { 
        // basically this is true for
        // fmod for all platforms
        // cocos for windows
        // all funcs without stl parameter for android
        // destructors for no platforms
        if (f.function_type == kDestructor) return false;
        if (f.function_type == kConstructor) return false;
        if (f.parent_class->name.rfind("fmod::", 0) == 0) return true;
        if (f.parent_class->name.rfind("cocos2d::", 0) == 0 && codegen::platform == kWindows) return true;
        if (getParameterTypes(f).find("gd::", 0) != string::npos && codegen::platform == kAndroid) return false;
        if (codegen::platform == kAndroid) return true;
        return false;
    }

    // definable : we can define it and hook it
    inline bool isFunctionDefinable(Function const& f) {
    	// basically this is true for
        // all funcs that we have the offset for
        // all funcs with stl parameter for android
        if (getParameterTypes(f).find("gd::", 0) != string::npos && codegen::platform == kAndroid) return true;
        if (getBind(f) != "") return true;
        return false;
    }

    inline std::string getConvention(Function const& f) {
    	if (codegen::platform != kWindows) return "DefaultConv";
        switch (f.function_type) {
            case kConstructor: [[fallthrough]];
            case kDestructor: [[fallthrough]];
            case kRegularFunction:
            	if (isFunctionDefined(f)) return "x86::Thiscall";
                return "x86::Membercall";
            case kVirtualFunction:
                return "x86::Thiscall";
            case kStaticFunction:
            	if (isFunctionDefined(f)) return "x86::Cdecl";
                return "x86::Optcall";
        }
        return "x86::Membercall";
    }*/
}
