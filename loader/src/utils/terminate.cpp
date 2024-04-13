#include <Geode/utils/terminate.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

std::string geode::utils::detail::fmtTerminateError(const char* reason, Mod* mod, std::source_location loc) {
    return fmt::format(
        "The mod '{}' by {} has deliberately asked the game to crash.\n"
        "Reason: {}\n"
        "Source: {}::{}():{}:{}",
        mod->getID(), mod->getDeveloper(), reason,
        loc.file_name(), loc.function_name(), loc.line(), loc.column()
    );
}
