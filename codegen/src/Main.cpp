#include "Shared.hpp"
#include "../include/utils/fs/filesystem.hpp" // bruh

using namespace codegen;

void writeFile(ghc::filesystem::path const& writePath, std::string const& output) {
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

int main(int argc, char** argv) {
	if (argc != 4) codegenerr("Invalid number of parameters (expected 3 found %d)", argc-1);

    std::string p = argv[1];

    if (p == "Win32") codegen::platform = kWindows;
    else if (p == "MacOS") codegen::platform = kMac;
    else if (p == "iOS") codegen::platform = kIos;
    else if (p == "Android") codegen::platform = kAndroid;
    else codegenerr("Invalid platform %s\n", p.c_str());

    chdir(argv[2]);
    std::stringstream stream;
    stream << "#include <Entry.bro>";

    ghc::filesystem::path writeDir = argv[3];
    ghc::filesystem::create_directories(writeDir);
    auto fileWithDir = [writeDir](char const* file) {
    	return writeDir / file;
    };
    Root root = parseTokens(lexStream(stream));

    /* add missing parameter names */
    for (auto& [name, c] : root.classes) {
		for (auto& f : c.functions) {
            for (size_t i = 0; i < f.argnames.size(); ++i) {
	            if (f.argnames[i] == "") {
	                f.argnames[i] = fmt::format("p{}", i);
	            }
	        }
        }
    }

    writeFile(fileWithDir("GeneratedAddress.hpp"), generateAddressHeader(root));
    writeFile(fileWithDir("GeneratedModify.hpp"), generateModifyHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(fileWithDir("GeneratedWrapper.hpp"), generateWrapperHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(fileWithDir("GeneratedType.hpp"), generateTypeHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(fileWithDir("GeneratedHeader.hpp"), generateGDHeader(root));
    writeFile(fileWithDir("GeneratedSource.cpp"), generateGDSource(root));
}
