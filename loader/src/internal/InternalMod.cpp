#include "InternalMod.hpp"

#include "about.hpp"

#include <Geode/loader/Dirs.hpp>
#include <LoaderImpl.hpp>

static constexpr char const* SUPPORT_INFO = R"MD(
**Geode** is funded through your gracious <cy>**donations**</c>!
You can support our work by sending <cp>**catgirl pictures**</c> to [HJfod](user:104257) :))
)MD";

static ModInfo getInternalModInfo() {
    try {
        auto json = ModJson::parse(LOADER_MOD_JSON);
        auto infoRes = ModInfo::create(json);
        if (infoRes.isErr()) {
            LoaderImpl::get()->platformMessageBox(
                "Fatal Internal Error",
                "Unable to parse loader mod.json: \"" + infoRes.unwrapErr() +
                    "\"\n"
                    "This is a fatal internal error in the loader, please "
                    "contact Geode developers immediately!"
            );
            exit(1);
        }
        auto info = infoRes.unwrap();
        info.details = LOADER_ABOUT_MD;
        info.supportInfo = SUPPORT_INFO;
        info.supportsDisabling = false;
        return info;
    }
    catch (std::exception& e) {
        LoaderImpl::get()->platformMessageBox(
            "Fatal Internal Error",
            "Unable to parse loader mod.json: \"" + std::string(e.what()) +
                "\"\n"
                "This is a fatal internal error in the loader, please "
                "contact Geode developers immediately!"
        );
        exit(1);
    }
}

Mod* InternalMod::get() {
    auto& mod = Mod::sharedMod<>;
    if (mod) return mod;

    mod = new Mod(getInternalModInfo());

    auto setupRes = mod->setup();
    if (!setupRes) {
        log::error("Failed to setup internal mod! ({})", setupRes.unwrapErr());
        return mod;
    }
    return mod;
}
