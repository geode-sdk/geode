#include "../../../filesystem/fs/filesystem.hpp"
#include <mach-o/dyld.h>
#include <unistd.h>
#include <dlfcn.h>
#include <array>
#include <iostream>

void loadGeode() {
    auto dylib = dlopen("Geode.dylib", RTLD_LAZY);
    if (dylib) return;

    std::cout << "Couldn't open Geode: " << dlerror() << std::endl;
    return;
}

__attribute__((constructor)) void _entry() {
    std::array<char, PATH_MAX> gddir;

    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir.data(), &out);

    ghc::filesystem::path gdpath = gddir.data();
    auto workingDir = gdpath.parent_path().parent_path();

    auto updatesDir = workingDir / "geode" / "update";
    auto libDir = workingDir / "Frameworks";
	auto resourcesDir = workingDir / "geode" / "resources";

	auto error = std::error_code();

	if (ghc::filesystem::exists(updatesDir / "Geode.dylib", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "Geode.dylib", 
            workingDir / "Geode.dylib", error
        );
        if (error) {
            std::cout << "Couldn't update Geode: " << error.message() << std::endl;
            return loadGeode();
        }
    }

    if (ghc::filesystem::exists(updatesDir / "resources", error) && !error) {
        ghc::filesystem::remove_all(resourcesDir / "geode.loader", error);

        if (error) {
            std::cout << "Couldn't update Geode resources: " << error.message() << std::endl;
            return loadGeode();
        }

        ghc::filesystem::rename(
            updatesDir / "resources", 
            resourcesDir / "geode.loader", error
        );
        
        if (error) {
            std::cout << "Couldn't update Geode resources: " << error.message() << std::endl;
            return loadGeode();
        }
    }    

    return loadGeode();
}