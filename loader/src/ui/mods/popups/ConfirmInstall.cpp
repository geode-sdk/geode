#include "ConfirmInstall.hpp"
#include <Geode/utils/ranges.hpp>
#include <Geode/ui/Popup.hpp>
#include <server/DownloadManager.hpp>

using namespace geode::prelude;
using namespace server;

void askConfirmModInstalls() {
    struct ToConfirm final {
        size_t modCount = 0;
        size_t replacementCount = 0;
        size_t dependencyCount = 0;
        std::unordered_set<Mod*> toDisable;
        std::unordered_set<Mod*> toEnable;
    };

    auto toConfirm = ToConfirm();

    // Collect all things we need to ask confirmation for
    for (auto download : ModDownloadManager::get()->getDownloads()) {
        auto status = download.getStatus();
        if (auto conf = std::get_if<DownloadStatusConfirm>(&status)) {
            if (auto dep = download.getDependencyFor()) {
                toConfirm.dependencyCount += 1;
            }
            else {
                toConfirm.modCount += 1;
                if (download.getReplacesMod()) {
                    toConfirm.replacementCount += 1;
                }

                // Since the user has already explicitly chosen to download these mods, we 
                // are going to assume they want these mods enabled over already installed 
                // ones

                // If this mod has incompatabilities that are installed, disable them
                for (auto inc : conf->version.metadata.getIncompatibilities()) {
                    if (inc.mod && inc.version.compare(conf->version.metadata.getVersion()) && inc.mod->isOrWillBeEnabled()) {
                        toConfirm.toDisable.insert(inc.mod);
                    }
                }
                // If some installed mods are incompatible with this one, disable them
                for (auto mod : Loader::get()->getAllMods()) {
                    for (auto inc : mod->getMetadata().getIncompatibilities()) {
                        if (inc.id == conf->version.metadata.getID() && inc.version.compare(mod->getVersion()) && mod->isOrWillBeEnabled()) {
                            toConfirm.toDisable.insert(mod);
                        }
                    }
                }

                // If this mod has required dependencies that are disabled, enable them
                for (auto dep : conf->version.metadata.getDependencies()) {
                    if (
                        dep.importance == ModMetadata::Dependency::Importance::Required &&
                        dep.mod && !dep.mod->isOrWillBeEnabled()
                    ) {
                        toConfirm.toEnable.insert(dep.mod);
                    }
                }
            }
        }
    }

    auto joinModsToIDs = [](std::unordered_set<Mod*> const& mods) {
        return ranges::join(
            ranges::map<std::vector<std::string>>(
                mods, [](Mod* mod) { return fmt::format("<cp>{}</c>", mod->getID()); }
            ),
            ", "
        );
    };

    createQuickPopup(
        "Confirm Install",
        fmt::format(
            "<cj>{}</c> mods will be installed, of which <cy>{}</c> are <cy>dependencies</c> and <cy>{}</c> are <cy>replacements</c>.\n"
            "<cr>{} mods will be force-disabled, as they are incompatible</c>: {}\n"
            "<cg>{} mods will be force-enabled</c>: {}",
            toConfirm.modCount, toConfirm.dependencyCount, toConfirm.replacementCount,
            toConfirm.toDisable.size(), joinModsToIDs(toConfirm.toDisable),
            toConfirm.toEnable.size(), joinModsToIDs(toConfirm.toEnable)
        ),
        "Cancel", "Continue",
        [toConfirm](auto*, bool btn2) {
            if (btn2) {
                for (auto mod : toConfirm.toDisable) {
                    (void)mod->disable();
                }
                for (auto mod : toConfirm.toEnable) {
                    (void)mod->enable();
                }
                ModDownloadManager::get()->confirmAll();
            }
            else {
                ModDownloadManager::get()->cancelAll();
            }
        }
    );
}
