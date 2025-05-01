#include "ModListSource.hpp"

void ModPackListSource::resetQuery() {}
ModPackListSource::ProviderTask ModPackListSource::fetchPage(size_t page, bool forceUpdate) {
    return ProviderTask::immediate(Err(LoadPageError("Coming soon ;)")));
}

ModPackListSource::ModPackListSource() {}

ModPackListSource* ModPackListSource::get() {
    static auto inst = new ModPackListSource();
    return inst;
}

void ModPackListSource::setSearchQuery(std::string const& query) {}

std::unordered_set<std::string> ModPackListSource::getModTags() const {
    return {};
}
void ModPackListSource::setModTags(std::unordered_set<std::string> const& set) {}
bool ModPackListSource::isDefaultQuery() const {
    return true;
}

bool ModPackListSource::isLocalModsOnly() const {
    return false;
}
