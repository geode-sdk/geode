#include "crashlog.hpp"
#include <fmt/core.h>
#include "about.hpp"
#include "../loader/ModImpl.hpp"
#include <Geode/Utils.hpp>
#include <Geode/utils/web.hpp>
#include <asp/time/SystemTime.hpp>

using namespace geode::prelude;

std::string crashlog::getDateString(bool filesafe) {
    auto const now = std::time(nullptr);
    auto const tm = asp::localtime(now);
    if (filesafe) {
        return fmt::format("{:%F_%H-%M-%S}", tm);
    }
    return fmt::format("{:%FT%T%z}", tm);
}

void crashlog::printGeodeInfo(Buffer& stream) {
    stream.append(
        "Loader Version: {}\n"
        "Loader Commit: {}\n"
        "Bindings Commit: {}\n"
        "Installed mods: {}\n"
        "Outdated mods: {}\n"
        "Problems: {}\n",
        Loader::get()->getVersion().toVString(),
        about::getLoaderCommitHash(),
        about::getBindingsCommitHash(),
        Loader::get()->getAllMods().size(),
        Loader::get()->getOutdated().size(),
        Loader::get()->getLoadProblems().size()
    );
}

void crashlog::printMods(Buffer& stream) {
    auto mods = Loader::get()->getAllMods();
    if (mods.empty()) {
        stream.append("<None>\n");
        return;
    }

    std::sort(mods.begin(), mods.end(), [](Mod* a, Mod* b) {
        auto const s1 = a->getID();
        auto const s2 = b->getID();
        return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto a, auto b) {
            return std::tolower(a) < std::tolower(b);
        });
    });
    using namespace std::string_view_literals;
    for (auto& mod : mods) {
        stream.append("{} | [{}] {}\n",
            mod->isCurrentlyLoading() ? "o"sv :
            mod->isEnabled() ? "x"sv :
            mod->hasLoadProblems() ? "!"sv : // thank you for this bug report
            mod->targetsOutdatedVersion() ? "*"sv : // thank you very much for this bug report
            mod->shouldLoad() ? "~"sv :
            " "sv,
            mod->getVersion().toVString(), mod->getID()
        );
    }
}

void crashlog::updateFunctionBindings() {
    constexpr uint64_t UPDATE_INTERVAL = 24 * 60 * 60; // one day
    if (Mod::get()->getSavedValue<uint64_t>("bindings-update-time") + UPDATE_INTERVAL > std::time(nullptr)) {
        return;
    }

    web::WebRequest().get(
        "https://prevter.github.io/bindings-meta/CodegenData-"
        GEODE_GD_VERSION_STRING
        "-"
        GEODE_WINDOWS("Win64") GEODE_INTEL_MAC("Intel") GEODE_ARM_MAC("Arm") GEODE_IOS("iOS")
        ".json"
    ).listen([](web::WebResponse* res) {
        if (!res->ok()) return;

        (void) res->into(dirs::getGeodeSaveDir() / "bindings.json");
        Mod::get()->setSavedValue<uint64_t>("bindings-update-time", std::time(nullptr));
    });
}

static std::vector<crashlog::FunctionBinding> const& getBindings() {
    static auto bindings = file::readFromJson<std::vector<crashlog::FunctionBinding>>(
        dirs::getGeodeSaveDir() / "bindings.json"
    ).unwrapOrDefault();
    return bindings;
}

std::string_view crashlog::lookupClosestFunction(uintptr_t& address) {
    auto& bindings = getBindings();
    if (bindings.empty()) { return {}; }

    auto it = std::lower_bound(
        bindings.begin(), bindings.end(), address,
        [](FunctionBinding const& a, uintptr_t b) { return a.offset < b; }
    );

    if (it == bindings.end() || it->offset > address) {
        if (it == bindings.begin()) return {};
        --it;
    }

    address -= it->offset;
    return it->name;
}

std::string_view crashlog::lookupFunctionByOffset(uintptr_t address) {
    auto& bindings = getBindings();
    if (bindings.empty()) { return {}; }

    auto it = std::lower_bound(
        bindings.begin(), bindings.end(), address,
        [](FunctionBinding const& a, uintptr_t b) { return a.offset < b; }
    );

    if (it != bindings.end() && it->offset == address) {
        return it->name;
    }

    return {};
}

std::string crashlog::writeCrashlog(geode::Mod* faultyMod, std::string_view info, std::string_view stacktrace, std::string_view registers) {
    std::filesystem::path outPath;
    return writeCrashlog(faultyMod, info, stacktrace, registers, outPath);
}

std::string crashlog::writeCrashlog(
    Mod* faultyMod,
    std::string_view info,
    std::string_view stacktrace,
    std::string_view registers,
    std::filesystem::path& outPath
) {
    // make sure crashlog directory exists
    (void)utils::file::createDirectoryAll(crashlog::getCrashLogDirectory());

    // add a file to let Geode know on next launch that it crashed previously
    // this could also be done by saving a loader setting or smth but eh.
    (void)utils::file::writeBinary(crashlog::getCrashLogDirectory() / "last-crashed", {});

    Buffer file;

    file.append(getDateString(false));
    file.append("\nWhoopsies! An unhandled exception has occurred.\n");

    if (faultyMod) {
        file.append(
            "It appears that the crash occurred while executing code from the \"{}\" mod. "
            "Please submit this crash report to its developers ({}) for assistance.\n",
            faultyMod->getID(),
            fmt::join(faultyMod->getDevelopers(), ", ")
        );
    }

    // geode info
    file.append("\n== Geode Information ==\n");
    printGeodeInfo(file);

    // exception info
    file.append("\n== Exception Information ==\n");
    file.append(info);

    // stack trace
    file.append("\n== Stack Trace ==\n");
    file.append(stacktrace);

    // registers
    file.append("\n== Register States ==\n");
    file.append(registers);

    // mods
    file.append("\n== Installed Mods ==\n");
    printMods(file);

    // save actual file
    outPath = crashlog::getCrashLogDirectory() / (getDateString(true) + ".log");
    std::ofstream actualFile;
    actualFile.open(
        outPath, std::ios::app
    );
    actualFile << file.view() << std::flush;
    actualFile.close();

    return file.str();
}
