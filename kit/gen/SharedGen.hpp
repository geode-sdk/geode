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

struct CacShare {
    inline static Platform platform;
    inline static string writePath;

    static Root init(int argc, char** argv) {
        if (argc < 4)
            cacerr("Invalid number of parameters (expected 3 found %d)", argc-1);

        char const* p = argv[1];
        if (strcmp(p, "Win32") == 0)
            platform = kWindows;
        else if (strcmp(p, "MacOS") == 0)
            platform = kMac;
        else if (strcmp(p, "iOS") == 0)
            platform = kIos;
        else if (strcmp(p, "Android") == 0)
            platform = kAndroid;
        else
            cacerr("Invalid platform %s\n", p);

        chdir(argv[2]);
        stringstream s;
        s << "#include <Entry.bro>";

        writePath = argv[3];
        return parseTokens(lexStream(s));
    }

    static bool isCocos(Function const& f) {
    	return f.parent_class->name.rfind("cocos2d", 0) == 0;
    }

    static string getAddress(Function const& f) {
        switch (CacShare::platform) {
            case kMac:
                return "base::get()+" + f.binds[kMac];
            case kIos:
                return "base::get()+" + f.binds[kIos];
            case kWindows:
                if (f.parent_class->name.rfind("cocos2d", 0) == 0) {
                	if (f.function_type == kConstructor || f.function_type == kDestructor) {
                		return "base::get()+" + f.binds[kWindows];
                	}
                    if (f.function_type == kVirtualFunction)
                        return fmt::format("addresser::getVirtual((types::member{})(&{}::{}))", f.global_index, f.parent_class->name, f.name);
                    else
                        return fmt::format("addresser::getNonVirtual((types::member{})(&{}::{}))", f.global_index, f.parent_class->name, f.name);
                } else {
                    return "base::get()+" + f.binds[kWindows];
                }
            case kAndroid:
                if (any_of(f.args.begin(), f.args.end(), [](string a) {return a.find("gd::") != string::npos;}))
                    return fmt::format("(uintptr_t)dlsym((void*)base::get(), \"{}\")", f.android_mangle);
                else {
                    if (f.function_type == kVirtualFunction)
                        return fmt::format("addresser::getVirtual((types::member{})(&{}::{}))", f.global_index, f.parent_class->name, f.name);
                    else
                        return fmt::format("addresser::getNonVirtual((types::member{})(&{}::{}))", f.global_index, f.parent_class->name, f.name);
                }
        }
        return "";
    }

    static bool functionDefined(Function const& f) {
    	return getAddress(f) != "base::get()+";
    }

    static string& getHardcode(Member & m) {
        return m.hardcodes[(std::array<size_t, 4> {0, 1, 0, 2})[CacShare::platform]];
    }

    static string getArray(size_t size) {
        return size > 0 ? fmt::format("[{}]", size) : string("");
    }

    static string toUnqualified(string qualifiedName) {
        if (qualifiedName.rfind("::") == string::npos) return qualifiedName;
        return qualifiedName.substr(qualifiedName.rfind("::")+2);
    }

    // new funcs start here

    // defined : we have a symbol that is usable
    static bool isFunctionDefined(Function const& f) { 
        // basically this is true for
        // fmod for all platforms
        // cocos for windows
        // all funcs without stl parameter for android
        // destructors for no platforms
        if (f.function_type == kDestructor) return false;
        if (f.parent_class->name.rfind("fmod", 0) == 0) return true;
        if (f.parent_class->name.rfind("cocos2d", 0) == 0 && platform == kWindows) return true;
    }

    // definable : we can define it and hook it
    static bool isFunctionDefinable(Function const& f) {
    	// basically this is true for
        // all funcs that we have the offset for
        // all funcs with stl parameter for android
    }

    static void editArguments(Function& f) {
        for (size_t i = 0; i < f.argnames.size(); ++i) {
            if (f.argnames[i] == "") {
                f.argnames[i] = fmt::format("p{}", i);
            }
        }
    }

    static string getIndex(Function const& f) {
    	return fmt::format("{}", f.global_index);
    }

    static string getClassName(Function const& f) {
    	return f.parent_class->name;
    }

    static string getFunctionName(Function const& f) {
    	return f.name;
    }

    static string getConst(Function const& f) {
    	return f.is_const ? "const" : "";
    }

    static string getConstWhitespace(Function const& f) {
    	return f.is_const ? " " : "";
    }

    static string getParameters(Function const& f) { // int p0, float p1
        vector<string> parameters;
        for (size_t i = 0; i < f.args.size(); ++i) {
            parameters.push_back(fmt::format("{} {}, ", f.args[i]}, f.argnames[i])); // p0, myFloat
        }
        return fmt::format("{}", fmt::join(parameters, ", "));
    }

    static string getParameterTypes(Function const& f) { //int, float
    	return fmt::format("{}", fmt::join(f.args, ", "));
    }

    static string getArguments(Function const& f) { // p0, p1
    	return fmt::format("{}", fmt::join(f.argnames, ", "));
    }

    static string getParameterComma(Function const& f) { // int p0, float p1
    	return f.args.size() > 0 ? ", " : "";
    }

    static string getParameterTypeComma(Function const& f) { //int, float
    	return f.args.size() > 0 ? ", " : "";
    }

    static string getArgumentComma(Function const& f) { // p0, p1
    	return f.argnames.size() > 0 ? ", " : "";
    }

    // and end here

    static string formatBases(vector<string> args) {
        return args.size() > 0 ? " : " + fmt::format("{}", fmt::join(args, ", ")) : string(""); 
    }

    static string getConvention(Function const& f) {
    	if (platform != kWindows) return "DefaultConv";
        switch (f.function_type) {
            case kConstructor: [[fallthrough]];
            case kDestructor: [[fallthrough]];
            case kRegularFunction:
            	if (isCocos(f)) return "Thiscall";
            	if (f.args.size() == 0) return "Thiscall";
                return "Membercall";
            case kVirtualFunction:
            	if (isCocos(f)) return "Thiscall";
                return "Thiscall";
            case kStaticFunction:
            	if (isCocos(f)) return "Cdecl";
            	if (f.args.size() == 0) return "Cdecl";
                return "Optcall";
        }
        return "Membercall";
    }

    static string getReturn(Function const& f) {
    	switch (f.function_type) {
    		case kConstructor: [[fallthrough]];
    		case kDestructor:
    			return "void";
    		default: break;
    	}
        if (f.return_type == "auto") {
            string out;
            if (f.function_type != kStaticFunction) {
                out = fmt::format("decltype(std::declval<{}>().{}(", f.parent_class->name, f.name);
            }
            else {
                out = fmt::format("decltype({}::{}(", f.parent_class->name, f.name);
            }
            vector<string> args;
            for (string i : f.args) {
                args.push_back(fmt::format("std::declval<{}>()", i));
            }

            out += fmt::format("{}))", fmt::join(args, ", "));
            return out;
        }
        return f.return_type;
    }

    static string getDocs(string docs) {
    	return docs.size() > 0 ? fmt::format("{}", docs) : string("");
    }

    static void writeFile(string& output) {
    	ifstream readfile;
    	readfile >> std::noskipws;
    	readfile.open(writePath);
    	string data((istreambuf_iterator<char>(readfile)), istreambuf_iterator<char>());
    	readfile.close();

    	if (data != output) {
    		ofstream writefile;
    		writefile.open(writePath);
    		writefile << output;
    		writefile.close();
    	}
    }
};

#ifdef _MSC_VER
#pragma warning(default: 4996)
#endif
