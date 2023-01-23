
#include <Geode/loader/Tulip.hpp>

tulip::hook::Result<void*> geode::hook::createWrapper(
    void* address,
    tulip::hook::WrapperMetadata const& metadata
) noexcept {
    return tulip::hook::createWrapper(address, metadata);
}

std::shared_ptr<tulip::hook::CallingConvention> geode::hook::createConvention(
    tulip::hook::TulipConvention convention
) noexcept {
    return tulip::hook::createConvention(convention);
}
