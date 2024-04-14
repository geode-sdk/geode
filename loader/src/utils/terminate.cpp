#include <Geode/utils/terminate.hpp>
#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

std::string geode::utils::detail::fmtTerminateError(const char* reason, Mod* mod) {
    return fmt::format(
        "The mod '{}' by {} has deliberately asked the game to crash.\nReason: {}",
        mod->getID(), mod->getDeveloper(), reason
    );
}
