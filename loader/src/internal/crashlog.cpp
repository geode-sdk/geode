#include "crashlog.hpp"
#include <fmt/core.h>
#include "about.hpp"
#include "../loader/ModImpl.hpp"
#include <Geode/Utils.hpp>
#include <Geode/utils/web.hpp>
#include <asp/time/SystemTime.hpp>
#include <Geode/utils/async.hpp>
#include <ranges>

using namespace geode::prelude;

std::string_view crashlog::Image::name() const {
    if (name_.empty()) {
        return "<Unknown>";
    }
    return name_;
}

std::string_view crashlog::Image::shortName() const {
    if (name_.empty()) {
        return "<Unknown>";
    }

    auto slash = name_.find_last_of("/\\");
    if (slash == std::string::npos) {
        return name_;
    }
    return std::string_view{name_}.substr(slash + 1);
}

ptrdiff_t crashlog::StackFrame::imageOffset() const {
    if (image == nullptr) {
        return 0;
    }
    return address - image->address;
}

ptrdiff_t crashlog::StackFrame::functionOffset() const {
    return offset;
}

crashlog::Image* crashlog::CrashContext::imageFromAddress(void const* addr) {
    for (auto& img : std::ranges::reverse_view(images)) {
        if ((uintptr_t)addr >= img.address) {
            // bounds check
            if (img.size != 0 && (uintptr_t)addr >= img.address + img.size) {
                continue;
            }

            return &img;
        }
    }
    return nullptr;
}

Mod* crashlog::CrashContext::modFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }
    auto image = this->imageFromAddress(addr);
    if (image == nullptr) {
        return nullptr;
    }

    std::filesystem::path imagePath = image->name();
    // if (!std::filesystem::exists(imagePath)) {
    //     return nullptr;
    // }
    if (imagePath.filename() == getGeodeBinaryName()) {
        return Mod::get();
    }

    for (auto& mod : Loader::get()->getAllMods()) {
        std::error_code ec;

        if (!mod->isLoaded() || !std::filesystem::exists(mod->getBinaryPath(), ec)) {
            continue;
        }

        if (std::filesystem::equivalent(imagePath, mod->getBinaryPath(), ec)) {
            return mod;
        }
    }
    return nullptr;
}

void crashlog::CrashContext::formatAddress(void const* addr, Buffer& stream, bool shortName) {
    auto image = imageFromAddress(addr);

    if (image) {
        stream.append(
            "{} ({} + {})",
            addr,
            shortName ? image->shortName() : image->name(),
            reinterpret_cast<void const*>((uintptr_t)addr - (uintptr_t)image->address)
        );
    } else {
        stream.append("{}", addr);
    }
}

void crashlog::CrashContext::initialize(void const* crashAddr) {
    this->registers = getRegisters();
    this->images = getImages();
    this->crashAddr = crashAddr;
    this->faultyMod = modFromAddress(crashAddr);

    std::sort(images.begin(), images.end(), [](auto const a, auto const b) {
        return (uintptr_t)a.address < (uintptr_t)b.address;
    });

    // this MUST come after getImages()
    this->frames = getStacktrace();

    // gather info
    this->writeInfo(infoStream);
}

std::string crashlog::getDateString(bool filesafe) {
    auto const now = std::chrono::system_clock::now();
    if (filesafe) {
        return fmt::format("{:%F_%H-%M-%S}", now);
    }
    return fmt::format("{:%FT%T%z}", now);
}

void crashlog::printGeodeInfo(Buffer& stream) {
    stream.append(
        "Loader Version: {}\n"
        "Loader Commit: {}\n"
        "Bindings Commit: {}\n"
        "Installed mods: {}\n"
        "Failed to load: {}\n",
        Loader::get()->getVersion().toVString(),
        about::getLoaderCommitHash(),
        about::getBindingsCommitHash(),
        Loader::get()->getAllMods().size(),
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
            mod->isLoaded() ? "x"sv :
            mod->targetsOutdatedVersion() ? "*"sv : // thank you very much for this bug report
            mod->failedToLoad() ? "!"sv : // thank you for this bug report
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

    async::spawn(
        web::WebRequest().get(
            "https://prevter.github.io/bindings-meta/CodegenData-"
            GEODE_GD_VERSION_STRING "-"
            GEODE_WINDOWS("Win64") GEODE_INTEL_MAC("Intel") GEODE_ARM_MAC("Arm") GEODE_IOS("iOS")
            ".json"
        ),
        [](web::WebResponse res) {
            if (!res.ok()) return;

            (void) res.into(dirs::getGeodeSaveDir() / "bindings.json");
            Mod::get()->setSavedValue<uint64_t>("bindings-update-time", std::time(nullptr));
        }
    );
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

    file.append("\nPlease share the whole crash log when asking for help.\n");

    // geode info
    file.append("\n== Geode Information ==\n");
    printGeodeInfo(file);

    // exception info
    file.append("\n== Exception Information ==\n");
    file.append(info);

    // stack trace
    file.append("\n== Stack Trace (the most important part) ==\n");
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

std::string crashlog::writeCrashlog(const CrashContext& ctx) {
    std::filesystem::path outPath;
    return writeCrashlog(ctx, outPath);
}

std::string crashlog::writeCrashlog(const CrashContext& ctx, std::filesystem::path& outCrashlogPath) {
    StringBuffer<> stacktrace;
    StringBuffer<> registers;

    // TODO: this should use the fetched symbol names from prevter server
    for (auto& frame : ctx.frames) {
        if (frame.image) {
            stacktrace.append("- {} + 0x{:x}", frame.image->shortName(), frame.imageOffset());
            if (!frame.symbol.empty()) {
                stacktrace.append(" ({} + 0x{:x}", frame.symbol, frame.offset);

                if (!frame.file.empty()) {
                    stacktrace.append(" | {}:{}", frame.file, frame.line);
                }
                stacktrace.append(")");
            }
        } else {
            stacktrace.append("- 0x{:x}", frame.address);
        }

        std::string_view description = frame.description;
        if (description.empty() && !frame.image) {
            // try to determine if it's a special tulip function
            auto tinfo = tulip::hook::getFunctionInformation((void*)frame.address);
            if (tinfo) {
                using enum tulip::hook::FunctionInformationReturn::Type;
                switch (tinfo->type) {
                    case Handler: description = "hook handler"; break;
                    case Relocated: description = "relocated function"; break;
                    case Trampoline: description = "trampoline"; break;
                    case Intervener: description = "intervener"; break;
                    default: description = "unknown tulip function"; break;
                }
            }
        }

        if (!description.empty()) {
            stacktrace.append(" ({})", description);
        }

        stacktrace.append("\n");
    }

    for (auto& reg : ctx.registers) {
        registers.append("{}: 0x{:x}\n", reg.name, reg.value);
    }

    return writeCrashlog(ctx.faultyMod, ctx.infoStream.view(), stacktrace.view(), registers.view(), outCrashlogPath);
}
