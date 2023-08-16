#include "Shared.hpp"

#include <ghc/filesystem.hpp> // bruh

using namespace codegen;

std::map<void const*, size_t> codegen::idMap;

int main(int argc, char** argv) try {
    if (argc != 4)
        throw codegen::error("Invalid number of parameters (expected 3 found {})", argc - 1);

    std::string p = argv[1];

    if (p == "Win32") codegen::platform = Platform::Windows;
    else if (p == "MacOS") codegen::platform = Platform::Mac;
    else if (p == "iOS") codegen::platform = Platform::iOS;
    else if (p == "Android") codegen::platform = Platform::Android;
    else throw codegen::error("Invalid platform {}\n", p);

    auto rootDir = ghc::filesystem::path(argv[2]);
    ghc::filesystem::current_path(rootDir);

    auto writeDir = ghc::filesystem::path(argv[3]) / "Geode";
    ghc::filesystem::create_directories(writeDir);
    ghc::filesystem::create_directories(writeDir / "modify");
    ghc::filesystem::create_directories(writeDir / "binding");

    Root root = broma::parse_file("Entry.bro");

    for (auto cls : root.classes) {
        for (auto dep : cls.depends) {
            if (!is_cocos_class(dep) &&
                std::find(root.classes.begin(), root.classes.end(), dep) == root.classes.end()) {
                throw codegen::error("Class {} depends on unknown class {}", cls.name, dep);
            }
        }
    }

    codegen::populateIds(root);

    writeFile(writeDir / "GeneratedAddress.cpp", generateAddressHeader(root));
    writeFile(writeDir / "GeneratedModify.hpp", generateModifyHeader(root, writeDir / "modify"));
    writeFile(writeDir / "GeneratedBinding.hpp", generateBindingHeader(root, writeDir / "binding"));
    writeFile(writeDir / "GeneratedPredeclare.hpp", generatePredeclareHeader(root));
    writeFile(writeDir / "GeneratedSource.cpp", generateBindingSource(root));
}

catch (std::exception& e) {
    std::cout << "Codegen error: " << e.what() << "\n";
    return 1;
}
