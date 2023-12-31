#include "crashlog.hpp"
#include <fmt/core.h>
#include "about.hpp"

using namespace geode::prelude;

std::string crashlog::getDateString(bool filesafe) {
    auto const now = std::time(nullptr);
    auto const tm = *std::localtime(&now);
    std::ostringstream oss;
    if (filesafe) {
        oss << std::put_time(&tm, "%F_%H-%M-%S");
    }
    else {
        oss << std::put_time(&tm, "%FT%T%z"); // ISO 8601
    }
    return oss.str();
}

void crashlog::printGeodeInfo(std::stringstream& stream) {
    stream << "Loader Version: " << Loader::get()->getVersion().toString() << "\n"
           << "Loader Commit: " << LOADER_COMMIT_HASH << "\n"
           << "Installed mods: " << Loader::get()->getAllMods().size() << "\n"
           << "Problems: " << Loader::get()->getProblems().size() << "\n";
}

void crashlog::printMods(std::stringstream& stream) {
    auto mods = Loader::get()->getAllMods();
    if (mods.empty()) {
        stream << "<None>\n";
    }
    using namespace std::string_view_literals;
    for (auto& mod : mods) {
        stream << fmt::format("{} | [{}] {}\n",
            mod->isEnabled() ? "x"sv : mod->shouldLoad() ? "~"sv : " "sv,
            mod->getVersion().toString(), mod->getID()
        );
    }
}

std::string crashlog::writeCrashlog(geode::Mod* faultyMod, std::string const& info, std::string const& stacktrace, std::string const& registers) {
    // make sure crashlog directory exists
    (void)utils::file::createDirectoryAll(crashlog::getCrashLogDirectory());

    // add a file to let Geode know on next launch that it crashed previously
    // this could also be done by saving a loader setting or smth but eh.
    (void)utils::file::writeBinary(crashlog::getCrashLogDirectory() / "last-crashed", {});
    
    std::stringstream file;

    file << getDateString(false) << "\n"
         << std::showbase << "Whoopsies! An unhandled exception has occured.\n";

    if (faultyMod) {
        file << "It appears that the crash occurred while executing code from "
             << "the \"" << faultyMod->getID() << "\" mod. "
             << "Please submit this crash report to its developer (" << faultyMod->getDeveloper()
             << ") for assistance.\n";
    }

    // geode info
    file << "\n== Geode Information ==\n";
    printGeodeInfo(file);

    // exception info
    file << "\n== Exception Information ==\n";
    file << info;

    // stack trace
    file << "\n== Stack Trace ==\n";
    file << stacktrace;

    // registers
    file << "\n== Register States ==\n";
    file << registers;

    // mods
    file << "\n== Installed Mods ==\n";
    printMods(file);

    // save actual file
    std::ofstream actualFile;
    actualFile.open(
        crashlog::getCrashLogDirectory() / (getDateString(true) + ".log"), std::ios::app
    );
    actualFile << file.rdbuf() << std::flush;
    actualFile.close();

    return file.str();
}
