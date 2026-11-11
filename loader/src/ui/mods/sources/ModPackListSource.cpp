#include "ModListSource.hpp"

void ModPackListSource::resetQuery() {}
ModPackListSource::ProviderTask ModPackListSource::fetchPage(size_t page, bool forceUpdate) {
    co_return Err(LoadPageError("Coming soon ;)"));
}

ModPackListSource::ModPackListSource() {}

ModPackListSource* ModPackListSource::get() {
    static auto inst = new ModPackListSource();
    return inst;
}

void ModPackListSource::setSearchQuery(std::string query) {}

StringSet ModPackListSource::getModTags() const {
    return {};
}
void ModPackListSource::setModTags(StringSet const& set) {}
bool ModPackListSource::isDefaultQuery() const {
    return true;
}

bool ModPackListSource::isLocalModsOnly() const {
    return false;
}
