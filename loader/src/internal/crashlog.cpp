#include "crashlog.hpp"
#include <fmt/core.h>
#include "about.hpp"
#include "../loader/ModImpl.hpp"
#include <Geode/Utils.hpp>

using namespace geode::prelude;

std::string crashlog::getDateString(bool filesafe) {
    auto const now = std::time(nullptr);
    auto const tm = *std::localtime(&now);
    char buffer[20];
    if (filesafe) {
        std::strftime(buffer, sizeof(buffer), "%F_%H-%M-%S", &tm);
    }
    else {
        std::strftime(buffer, sizeof(buffer), "%FT%T%z", &tm);
    }
    return buffer;
}

void crashlog::printGeodeInfo(std::stringstream& stream) {
    auto loader = Loader::get();
    stream << "Loader Version: " << loader->getVersion().toString() << "\n"
           << "Loader Commit: " << about::getLoaderCommitHash() << "\n"
           << "Bindings Commit: " << about::getBindingsCommitHash() << "\n"
           << "Installed mods: " << loader->getAllMods().size() << "\n"
           << "Problems: " << loader->getProblems().size() << "\n";
}

void crashlog::printMods(std::stringstream& stream) {
    auto mods = Loader::get()->getAllMods();
    if (mods.empty()) {
        stream << "<None>\n";
        return;
    }
    
    std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) {
        return a->getID().compare(b->getID()) < 0;
    });
    
    for (auto& mod : mods) {
        auto impl = ModImpl::getImpl(mod);
        char status = ' ';
        if (impl->isCurrentlyLoading()) status = 'o';
        else if (mod->isEnabled()) status = 'x';
        else if (impl->hasProblems()) status = '!';
        else if (mod->shouldLoad()) status = '~';

        stream << fmt::format("{} | [{}] {}\n", status, mod->getVersion().toString(), mod->getID());
    }
}

std::string crashlog::writeCrashlog(geode::Mod* faultyMod, const std::string& info, const std::string& stacktrace, const std::string& registers) {
    utils::file::createDirectoryAll(crashlog::getCrashLogDirectory());
    utils::file::writeBinary(crashlog::getCrashLogDirectory() / "last-crashed", {});
    
    std::string file;
    file.reserve(4096);
    
    file += getDateString(false) + "\n";
    file += std::showbase + "Whoopsies! An unhandled exception has occurred.\n";

    if (faultyMod) {
        file += fmt::format("It appears that the crash occurred while executing code from the \"{}\" mod. Please submit this crash report to it's developers ({}) for assistance.\n",
                            faultyMod->getID(), ranges::join(faultyMod->getDevelopers(), ", "));
    }

    file += "\n== Geode Information ==\n";
    printGeodeInfo(file);

    file += "\n== Exception Information ==\n";
    file += info;

    file += "\n== Stack Trace ==\n";
    file += stacktrace;

    file += "\n== Register States ==\n";
    file += registers;

    file += "\n== Installed Mods ==\n";
    printMods(file);

    std::ofstream actualFile((crashlog::getCrashLogDirectory() / (getDateString(true) + ".log")).string(), std::ios::app);
    actualFile << file;
    actualFile.close();

    return file;
}
