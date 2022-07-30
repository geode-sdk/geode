#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#undef snprintf

USE_GEODE_NAMESPACE();
using namespace std::string_literals;

bool Mod::validateID(std::string const& id) {
    // ids may not be empty
    if (!id.size()) return false;
    for (auto const& c : id) {
        if (!(
            ('a' <= c && c <= 'z') ||
            ('A' <= c && c <= 'Z') ||
            ('0' <= c && c <= '9') ||
            (c == '-') ||
            (c == '_') ||
            (c == '.')
        )) return false;
    }
    return true;
}

Result<Mod*> Loader::loadModFromFile(std::string const& path) {
    // load mod.json
    auto res = ModInfo::createFromGeodeFile(path);
    if (!res) {
        return Err(res.error());
    }

    // check that a duplicate has not been loaded
    if (m_mods.count(res.value().m_id)) {
        return Err("Mod with ID \"" + res.value().m_id + "\" has already been loaded!");
    }
    
    // create and set up Mod instance
    auto mod = new Mod(res.value());
    mod->m_saveDirPath = Loader::get()->getGeodeSaveDirectory() / GEODE_MOD_DIRECTORY / res.value().m_id;
    mod->loadDataStore();
    ghc::filesystem::create_directories(mod->m_saveDirPath);

    // enable mod if needed
    mod->m_enabled = Loader::get()->shouldLoadMod(mod->m_info.m_id);
    this->m_mods.insert({ res.value().m_id, mod });
    mod->updateDependencyStates();

    return Ok<>(mod);
}
