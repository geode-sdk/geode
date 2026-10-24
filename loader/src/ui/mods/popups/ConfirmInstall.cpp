#include "ConfirmInstall.hpp"

#include <Geode/loader/Mod.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/ranges.hpp>
#include <server/DownloadManager.hpp>

using namespace geode::prelude;
using namespace server;

void askConfirmModInstalls() {
    struct ToConfirm final {
        size_t modCount = 0;
        StringSet dependencies;
        StringSet replacements;
        std::unordered_set<Mod*> toDisable;
        StringSet toDisableModId;
        std::unordered_set<Mod*> toEnable;
        StringSet modsToInstall;
    };

    ToConfirm toConfirm;

    // Collect all things we need to ask confirmation for
    for (auto const& download : ModDownloadManager::get()->getDownloads()) {
        auto status = download.getStatus();
        if (auto conf = std::get_if<DownloadStatusConfirm>(&status)) {
            if (auto dep = download.getDependencyFor()) {
                toConfirm.dependencies.insert(download.getID());

                if (!toConfirm.modsToInstall.contains(dep.value().first))
                    toConfirm.modsToInstall.insert(dep.value().first);
            }
            else {
                toConfirm.modCount += 1;
                if (download.getReplacesMod()) {
                    toConfirm.replacements.insert(download.getID());
                }

                // Since the user has already explicitly chosen to download these mods, we
                // are going to assume they want these mods enabled over already installed
                // ones

                // If this mod has incompatibilities that are installed, disable them
                for (auto const& inc : conf->version.metadata.getIncompatibilities()) {
                    if (inc.getMod() &&
                        inc.getVersion().compare(conf->version.metadata.getVersion()) &&
                        inc.getMod()->isOrWillBeEnabled()) {
                        toConfirm.toDisable.insert(inc.getMod());
                    }
                    for (auto const& download : ModDownloadManager::get()->getDownloads()) {
                        if (download.isDone() && inc.getID() == download.getID() &&
                            inc.getVersion().compare(conf->version.metadata.getVersion())) {
                            toConfirm.toDisableModId.insert(inc.getID());
                        }
                    }
                }
                // If some installed mods are incompatible with this one, disable them
                for (auto mod : Loader::get()->getAllMods()) {
                    for (auto const& inc : mod->getMetadata().getIncompatibilities()) {
                        if (conf->version.metadata.getID() == inc.getID() &&
                            inc.getVersion().compare(mod->getVersion()) && mod->isOrWillBeEnabled()) {
                            toConfirm.toDisable.insert(mod);
                        }
                    }
                }
                // If some newly downloaded mods are incompatible with this one, disable them
                for (auto const& download : ModDownloadManager::get()->getDownloads()) {
                    auto status = download.getStatus();
                    if (auto done = std::get_if<DownloadStatusDone>(&status)) {
                        for (auto const& inc : done->version.metadata.getIncompatibilities()) {
                            if (conf->version.metadata.getID() == inc.getID() &&
                                inc.getVersion().compare(done->version.metadata.getVersion())) {
                                toConfirm.toDisableModId.insert(download.getID());
                            }
                        }
                    }
                }

                // If this mod has required dependencies that are disabled, enable them
                for (auto const& dep : conf->version.metadata.getDependencies()) {
                    if (dep.isRequired() && dep.getMod() && !dep.getMod()->isOrWillBeEnabled()) {
                        toConfirm.toEnable.insert(dep.getMod());
                    }
                }
            }
        }
    }

    StringSet idsToDisable = toConfirm.toDisableModId;
    for (auto mod : toConfirm.toDisable) {
        idsToDisable.insert(mod->getID());
    }

    if (idsToDisable.size() == 0 && toConfirm.toEnable.size() == 0 &&
        toConfirm.dependencies.size() == 0 && toConfirm.replacements.size() == 0) {
        ModDownloadManager::get()->confirmAll();
        return;
    }

    StringBuffer content;

    if (toConfirm.modsToInstall.size() > 0) {
        content.append(
            "<cy>{} mod{} will be installed</c>:\n\n",
            toConfirm.modsToInstall.size(),
            toConfirm.modsToInstall.size() != 1 ? "s" : ""
        );
        for (auto mod : toConfirm.modsToInstall) {
            content.append("<mod:{}>\n\n", mod);
        }
    }

    if (toConfirm.dependencies.size() > 0) {
        content.append(
            "<cy>{} dependenc{} will also be installed</c>:\n\n",
            toConfirm.dependencies.size(),
            toConfirm.dependencies.size() != 1 ? "ies" : "y"

        );
        for (auto mod : toConfirm.dependencies) {
            content.append("<mod:{}>\n\n", mod);
        }
    }

    if (toConfirm.replacements.size() > 0) {
        content.append(
            "<cy>{} mod{} will be replaced</c>:\n\n",
            toConfirm.replacements.size(),
            toConfirm.replacements.size() != 1 ? "s" : ""
        );
        for (auto mod : toConfirm.replacements) {
            content.append("<mod:{}>\n\n", mod);
        }
    }

    if (toConfirm.toDisable.size() > 0) {
        content.append(
            "<cr>{} mod{} will be force-disabled, as they are incompatible</c>:\n\n",
            toConfirm.toDisable.size(),
            toConfirm.toDisable.size() != 1 ? "s" : ""
        );
        for (auto mod : toConfirm.toDisable) {
            content.append("<mod:{}>\n\n", mod->getID());
        }
    }

    if (toConfirm.toEnable.size() > 0) {
        content.append(
            "<cg>{} mod{} will be force-enabled</c>:\n\n",
            toConfirm.toEnable.size(),
            toConfirm.toEnable.size() != 1 ? "s" : ""

        );
        for (auto mod : toConfirm.toEnable) {
            content.append("<mod:{}>\n\n", mod->getID());
        }
    }

    MDPopup::create(
        true, "Confirm Install", content.str(), "Cancel", "Continue", [toConfirm](bool btn2) {
            if (btn2) {
                for (auto mod : toConfirm.toDisable) {
                    (void)mod->disable();
                }
                for (auto const& modId : toConfirm.toDisableModId) {
                    Mod::get()->setSavedValue("should-load-" + modId, false);
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
    )->show();
}