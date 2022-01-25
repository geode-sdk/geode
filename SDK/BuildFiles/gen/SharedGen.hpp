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
                	string type;
                	if (f.function_type == kStaticFunction) type = fmt::format("ret{index}(*)({raw_arg_types}) {const}",
						fmt::arg("index",f.index),
                		fmt::arg("class_name", f.parent_class->name),
                		fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
                		fmt::arg("const", f.is_const ? "const " : "")
                	);
                	else type = fmt::format("ret{index}({class_name}::*)({raw_arg_types}) {const}",
                		fmt::arg("index",f.index),
                		fmt::arg("class_name", f.parent_class->name),
                		fmt::arg("raw_arg_types", CacShare::formatRawArgTypes(f.args)),
                		fmt::arg("const", f.is_const ? "const " : "")
                	);
                    if (f.function_type == kVirtualFunction)
                        return fmt::format("addresser::getVirtual(({})(&{}::{}))", type, f.parent_class->name, f.name);
                    else
                        return fmt::format("addresser::getNonVirtual(({})(&{}::{}))", type, f.parent_class->name, f.name);
                } else {
                    return "base::get()+" + f.binds[kWindows];
                }
            case kAndroid:
                if (any_of(f.args.begin(), f.args.end(), [](string a) {return a.find("gd::") != string::npos;}))
                    return fmt::format("(uintptr_t)dlsym((void*)base::get(), \"{}\")", f.android_mangle);
                else {
                    if (f.function_type == kVirtualFunction)
                        return fmt::format("addresser::getVirtual((mem{})(&{}::{}))", f.index, f.parent_class->name, f.name);
                    else
                        return fmt::format("addresser::getNonVirtual((mem{})(&{}::{}))", f.index, f.parent_class->name, f.name);
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

    static string formatArgTypes(vector<string> args) {
        return args.size() > 0 ? fmt::format(", {}", fmt::join(args, ", ")) : string("");
    }

    static string formatRawArgTypes(vector<string> args) {
        return args.size() > 0 ? fmt::format("{}", fmt::join(args, ", ")) : string("");
    }

    static string formatRawArgs(vector<string> args) {
        string out = "";
        size_t c = 0;
        for (auto& i : args) {
            out += fmt::format("{} p{}, ", i, c);
            ++c;
        }
        return out.substr(0, out.size()-2);
    }

    static string formatArgs(vector<string> args, vector<string> argnames) {
        string out = "";
        size_t c = 0;
        for (auto& i : args) {
            if (argnames[c] == "") out += fmt::format("{}, ", i); 
            else out += fmt::format("{} {}, ", i, argnames[c]); 
            ++c;
        }
        return out.substr(0, out.size()-2);
    }

    static string formatBases(vector<string> args) {
        return args.size() > 0 ? " : " + fmt::format("{}", fmt::join(args, ", ")) : string(""); 
    }

    static string formatRawParameters(size_t paramCount) {
        if (paramCount) {
            vector<string> c;
            for (auto i = 0u; i<paramCount; ++i)
                c.push_back(fmt::format("p{}", i));
            return fmt::format("{}", fmt::join(c, ", "));
        } else {
            return "";
        }
    }

    static string formatParameters(size_t paramCount) {
        if (paramCount) {
            return fmt::format(", {}", formatRawParameters(paramCount));
        } else {
            return "";
        }
    }

    

    static void formatWinParameters(Function const& f) {

    }

    static string getConvention(Function const& f) {
        switch (f.function_type) {
            case kConstructor:
            case kDestructor:
            case kRegularFunction:
            	if (f.args.size() == 0) return "Cdecl";
                return "Membercall";
            case kVirtualFunction:
                return "Thiscall";
            case kStaticFunction:
            	if (f.args.size() == 0) return "Cdecl";
                return "Optcall";
        }
        return "Membercall";
    }

    static string getReturn(Function const& f) {
    	switch (f.function_type) {
    		case kConstructor:
    		case kDestructor:
    			return "void";
    		default:
    			break;
    	}
        if (f.return_type == "auto") {
            string out = fmt::format("decltype(std::declval<{}>().{}(", f.parent_class->name, f.name);
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
