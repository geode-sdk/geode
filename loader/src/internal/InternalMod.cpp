#include "InternalMod.hpp"

#include "InternalLoader.hpp"
#include "about.hpp"

static constexpr char const* SUPPORT_INFO = R"MD(
**Geode** is funded through your gracious <cy>**donations**</c>!
You can support our work by sending <cp>**catgirl pictures**</c> to [HJfod](user:104257) :))
)MD";

static ModInfo getInternalModInfo() {
    try {
        auto json = ModJson::parse(LOADER_MOD_JSON);
        auto infoRes = ModInfo::create(json);
        if (infoRes.is_error()) {
            InternalLoader::platformMessageBox(
                "Fatal Internal Error",
                "Unable to parse loader mod.json: \"" + infoRes.error() +
                    "\"\n"
                    "This is a fatal internal error in the loader, please "
                    "contact Geode developers immediately!"
            );
            exit(1);
        }
        auto info = infoRes.value();
        info.m_details = LOADER_ABOUT_MD;
        info.m_supportInfo = SUPPORT_INFO;
        info.m_supportsDisabling = false;
        return info;
    }
    catch (std::exception& e) {
        InternalLoader::platformMessageBox(
            "Fatal Internal Error",
            "Unable to parse loader mod.json: \"" + std::string(e.what()) +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        exit(1);
    }
}

InternalMod::InternalMod() : Mod(getInternalModInfo()) {
    m_saveDirPath = Loader::get()->getGeodeSaveDirectory() / GEODE_MOD_DIRECTORY / m_info.m_id;

    ghc::filesystem::create_directories(m_saveDirPath);

    // make sure spritesheets get added
    m_addResourcesToSearchPath = true;

    auto sett = this->loadSettings();
    if (!sett) {
        log::log(Severity::Error, this, "{}", sett.error());
    }
}

InternalMod::~InternalMod() {}

InternalMod* InternalMod::get() {
    static auto g_mod = new InternalMod;
    return g_mod;
}
