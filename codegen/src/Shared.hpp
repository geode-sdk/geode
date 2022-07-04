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

std::string generateAddressHeader(Root const& root);
std::string generateModifyHeader(Root const& root);
std::string generateWrapperHeader(Root const& root);
std::string generateTypeHeader(Root const& root);
std::string generateGDHeader(Root const& root);
std::string generateGDSource(Root const& root);
std::string generateTidyHeader(Root const& root);

namespace codegen {
	inline Platform platform;

	inline std::string getBind(Function const& f) {
        return f.binds[codegen::platform];
    }

    inline std::string getIndex(Function const& f) {
    	return fmt::format("{}", f.global_index);
    }

    inline std::string getClassName(Function const& f) {
    	return f.parent_class->name;
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

    inline std::string getParameters(Function const& f) { // int p0, float p1
        std::vector<std::string> parameters;
        for (size_t i = 0; i < f.args.size(); ++i) {
            parameters.push_back(fmt::format("{} {}", f.args[i], f.argnames[i])); // p0, myFloat
        }
        return fmt::format("{}", fmt::join(parameters, ", "));
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
    }
}
