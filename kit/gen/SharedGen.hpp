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

    static void editArguments(Function& f) {
        for (size_t i = 0; i < f.argnames.size(); ++i) {
            if (f.argnames[i] == "") {
                f.argnames[i] = fmt::format("p{}", i);
            }
        }
    }

    static string getBind(Function const& f) {
        return f.binds[CacShare::platform];
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
            parameters.push_back(fmt::format("{} {}", f.args[i], f.argnames[i])); // p0, myFloat
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

    static string getConvention(Function const& f) {
    	if (CacShare::platform != kWindows) return "DefaultConv";
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
    }
    

    // defined : we have a symbol that is usable
    static bool isFunctionDefined(Function const& f) { 
        // basically this is true for
        // fmod for all platforms
        // cocos for windows
        // all funcs without stl parameter for android
        // destructors for no platforms
        if (f.function_type == kDestructor) return false;
        if (f.parent_class->name.rfind("fmod::", 0) == 0) return true;
        if (f.parent_class->name.rfind("cocos2d::", 0) == 0 && platform == kWindows) return true;
        if (getParameterTypes(f).find("gd::", 0) != string::npos && platform == kAndroid) return false;
        if (platform == kAndroid) return true;
        return false;
    }

    // definable : we can define it and hook it
    static bool isFunctionDefinable(Function const& f) {
    	// basically this is true for
        // all funcs that we have the offset for
        // all funcs with stl parameter for android
        if (getParameterTypes(f).find("gd::", 0) != string::npos && platform == kAndroid) return true;
        if (getBind(f) != "") return true;
        return false;
    }

    // and end here

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
