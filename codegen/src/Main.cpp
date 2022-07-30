#include "Shared.hpp"
#include <fs/filesystem.hpp> // bruh

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
	if (argc != 4) throw codegen::error("Invalid number of parameters (expected 3 found {})", argc-1);

    std::string p = argv[1];

    if (p == "Win32") codegen::platform = Platform::Windows;
    else if (p == "MacOS") codegen::platform = Platform::Mac;
    else if (p == "iOS") codegen::platform = Platform::iOS;
    else if (p == "Android") codegen::platform = Platform::Android;
    else throw codegen::error("Invalid platform {}\n", p);

    chdir(argv[2]);

    ghc::filesystem::path writeDir = argv[3];
    ghc::filesystem::create_directories(writeDir);

    Root root = broma::parse_file("Entry.bro");

    for (auto cls : root.classes) {
        for (auto dep : cls.depends) {
            if(!can_find(dep, "cocos2d::") && std::find(root.classes.begin(), root.classes.end(), dep) == root.classes.end()) {
                throw codegen::error("Class {} depends on unknown class {}", cls.name, dep);
            }
        }
    }

    writeFile(writeDir / "GeneratedAddress.hpp", generateAddressHeader(root));
    writeFile(writeDir / "GeneratedModify.hpp", generateModifyHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(writeDir / "GeneratedWrapper.hpp", generateWrapperHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(writeDir / "GeneratedType.hpp", generateTypeHeader(root)); // pretty much obsolete with a custom compiler
    writeFile(writeDir / "GeneratedHeader.hpp", generateGDHeader(root));
    writeFile(writeDir / "GeneratedSource.cpp", generateGDSource(root));
}
