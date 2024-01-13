#include <Geode/loader/IPC.hpp>
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

std::string Loader::Impl::getGameVersion() {
    if (m_gdVersion.empty()) {
        // TODO: detect gd version
    }
    return m_gdVersion;
}

void Loader::Impl::setupIPC() {
    log::warn("IPC is not supported on this platform!");
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}
