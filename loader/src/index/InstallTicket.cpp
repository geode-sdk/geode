#include "Index.hpp"
#include <thread>
#include <Geode/utils/json.hpp>
#include <hash.hpp>
#include "fetch.hpp"

void InstallTicket::postProgress(
    UpdateStatus status,
    std::string const& info,
    uint8_t percentage
) {
    if (m_progress) {
        Loader::get()->queueInGDThread([this, status, info, percentage]() -> void {
            m_progress(this, status, info, percentage);
        });
    }
    if (status == UpdateStatus::Failed || status == UpdateStatus::Finished) {
        Log::get() << "Deleting InstallTicket at " << this;
        // clean up own memory
        delete this;
    }
}

InstallTicket::InstallTicket(
    Index* index,
    std::vector<std::string> const& list,
    ItemInstallCallback progress
) : m_index(index),
    m_installList(list),
    m_progress(progress) {}

std::vector<std::string> InstallTicket::getInstallList() const {
    return m_installList;
}

void InstallTicket::install(std::string const& id) {
    // run installing in another thread in order 
    // to render progress on screen while installing
    auto indexDir = Loader::get()->getGeodeDirectory() / "index";

    auto item = m_index->getKnownItem(id);

    this->postProgress(UpdateStatus::Progress, "Checking status", 0);
    
    // download to temp file in index dir
    auto tempFile = indexDir / item.m_download.m_filename;

    this->postProgress(UpdateStatus::Progress, "Fetching binary", 0);
    auto res = fetchFile(
        item.m_download.m_url,
        tempFile,
        [this, tempFile](double now, double total) -> int {
            // check if cancelled
            std::lock_guard cancelLock(m_cancelMutex);
            if (m_cancelling) {
                try { ghc::filesystem::remove(tempFile); } catch(...) {}
                return 1;
            }

            // no need to scope the lock guard more as this 
            // function is going to exit right after anyway

            this->postProgress(
                UpdateStatus::Progress,
                "Downloading binary",
                static_cast<uint8_t>(now / total * 100.0)
            );
            return 0;
        }
    );
    if (!res) {
        try { ghc::filesystem::remove(tempFile); } catch(...) {}
        return this->postProgress(
            UpdateStatus::Failed,
            "Downloading failed: " + res.error()
        );
    }

    // check if cancelled
    {
        std::lock_guard cancelLock(m_cancelMutex);
        if (m_cancelling) {
            ghc::filesystem::remove(tempFile);
            return;
        }
        // scope ends here since we don't need to 
        // access m_cancelling anymore
    }

    // check for 404
    auto notFound = file_utils::readString(tempFile);
    if (notFound && notFound.value() == "Not Found") {
        try { ghc::filesystem::remove(tempFile); } catch(...) {}
        return this->postProgress(
            UpdateStatus::Failed,
            "Binary file download returned \"Not found\". Report "
            "this to the Geode development team."
        );
    }

    // verify checksum
    this->postProgress(UpdateStatus::Progress, "Verifying", 100);

    auto checksum = ::calculateHash(tempFile.string());

    if (checksum != item.m_download.m_hash) {
        try { ghc::filesystem::remove(tempFile); } catch(...) {}
        return this->postProgress(
            UpdateStatus::Failed,
            "Checksum mismatch! (Downloaded file did not match what "
            "was expected. Try again, and if the download fails another time, "
            "report this to the Geode development team."
        );
    }

    // move temp file to geode directory
    try {
        auto modDir = Loader::get()->getGeodeDirectory() / "mods";
        auto targetFile = modDir / item.m_download.m_filename;

        // find valid filename that doesn't exist yet
        if (!m_replaceFiles) {
            auto filename = ghc::filesystem::path(
                item.m_download.m_filename
            ).replace_extension("").string();

            size_t number = 0;
            while (ghc::filesystem::exists(targetFile)) {
                targetFile = modDir /
                    (filename + std::to_string(number) + ".geode");
                number++;
            }
        }

        // move file
        ghc::filesystem::rename(tempFile, targetFile);
    } catch(std::exception& e) {
        try { ghc::filesystem::remove(tempFile); } catch(...) {}
        return this->postProgress(
            UpdateStatus::Failed,
            "Unable to move downloaded file to mods directory: \"" + 
            std::string(e.what()) + " \" "
            "(This might be due to insufficient permissions to "
            "write files under SteamLibrary, try running GD as "
            "administrator)"
        );
    }

    // call next in queue or post finish message
    Loader::get()->queueInGDThread([this, id]() -> void {
        // todo: Loader::get()->refreshMods(m_updateMod);
        // where the Loader unloads the mod binary and 
        // reloads it from disk (this should prolly be 
        // done only for the installed mods)
        Loader::get()->refreshMods();
        // already in GD thread, so might aswell do the 
        // progress posting manually
        if (m_progress) {
            (m_progress)(
                this, UpdateStatus::Finished, "", 100
            );
        }
        // clean up memory
        delete this;
    });
}

void InstallTicket::cancel() {
    // really no point in using std::lock_guard here 
    // since just plain locking and unlocking the mutex
    // will do the job just fine with the same legibility
    m_cancelMutex.lock();
    m_cancelling = true;
    m_cancelMutex.unlock();
}

void InstallTicket::start(InstallMode mode) {
    if (m_installing) return;
    // make sure we have stuff to install
    if (!m_installList.size()) {
        return this->postProgress(
            UpdateStatus::Failed, "Nothing to install", 0
        );
    }
    m_installing = true;
    switch (mode) {
        case InstallMode::Concurrent: {
            for (auto& id : m_installList) {
                std::thread(&InstallTicket::install, this, id).detach();
            }
        } break;

        case InstallMode::Order: {
            std::thread([this]() -> void {
                for (auto& id : m_installList) {
                    this->install(id);
                }
            }).detach();
        } break;
    }
}
