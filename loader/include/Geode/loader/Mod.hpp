#pragma once

#include "../DefaultInclude.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include <Geode/Result.hpp>
#include "../utils/VersionInfo.hpp"
#include "../utils/general.hpp"

#include "Loader.hpp" // very nice circular dependency fix
#include "Hook.hpp"
#include "ModMetadata.hpp"
#include "Setting.hpp"
#include "Types.hpp"
#include "Loader.hpp"
#include "../utils/string.hpp"

#include <matjson.hpp>
#include <matjson/stl_serialize.hpp>
#include <optional>
#include <string_view>
#include <tulip/TulipHook.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace geode {
    template <class T>
    struct HandleToSaved : public T {
        Mod* m_mod;
        std::string m_key;

        HandleToSaved(std::string const& key, Mod* mod, T const& value) :
            T(value), m_key(key), m_mod(mod) {}

        HandleToSaved(HandleToSaved const&) = delete;
        HandleToSaved(HandleToSaved&&) = delete;
        ~HandleToSaved();
    };

    enum class ModRequestedAction {
        None,
        Enable,
        Disable,
        Uninstall,
        UninstallWithSaveData,
        Update
    };

    static constexpr bool modRequestedActionIsToggle(ModRequestedAction action) {
        return action == ModRequestedAction::Enable || action == ModRequestedAction::Disable;
    }
    static constexpr bool modRequestedActionIsUninstall(ModRequestedAction action) {
        return action == ModRequestedAction::Uninstall || action == ModRequestedAction::UninstallWithSaveData;
    }

    Mod* takeNextLoaderMod();

    class ModImpl;

    /**
     * Represents a Mod ingame.
     * @class Mod
     */
    class GEODE_DLL Mod final {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class Loader;

        template <class = void>
        static inline GEODE_HIDDEN Mod* sharedMod = nullptr;

        // used internally in geode_implicit_load
        template <class = void>
        static inline GEODE_HIDDEN void setSharedMod(Mod* mod) {
            sharedMod<> = mod;
        }

        friend void GEODE_CALL ::geode_implicit_load(Mod*);

    public:
        // no copying
        Mod(Mod const&) = delete;
        Mod operator=(Mod const&) = delete;

        // Protected constructor/destructor
        Mod() = delete;
        Mod(ModMetadata const& metadata);
        ~Mod();

        std::string getID() const;
        std::string getName() const;
        std::vector<std::string> getDevelopers() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        std::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool isOrWillBeEnabled() const;
        bool isInternal() const;
        bool needsEarlyLoad() const;

        [[deprecated("Use Mod::getMetadataRef which is better for efficiency")]]
        ModMetadata getMetadata() const; // TODO: remove in v5
        ModMetadata const& getMetadataRef() const;

        std::filesystem::path getTempDir() const;
        /**
         * Get the path to the mod's platform binary (.dll on Windows, .dylib
         * on Mac & iOS, .so on Android)
         */
        std::filesystem::path getBinaryPath() const;
        /**
         * Get the path to the mod's runtime resources directory (contains all
         * of its resources)
         */
        std::filesystem::path getResourcesDir() const;

        /**
         * Get the dependency settings for a specific dependency via its ID. For
         * example, if this mod depends on Custom Keybinds, it can specify the
         * keybinds it wants to add in `mod.json` under
         * `dependencies."geode.custom-keybinds".settings.keybinds`
         * @returns Null JSON value if there are no settings or if the mod
         * doesn't depend on the given mod ID
         */
        matjson::Value getDependencySettingsFor(std::string_view dependencyID) const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setMetadata(ModMetadata const& metadata);
        std::vector<Mod*> getDependants() const;
#endif

        using CheckUpdatesTask = Task<Result<std::optional<VersionInfo>, std::string>>;
        /**
         * Check if this Mod has updates available on the mods index. If
         * you're using this for automatic update checking, use
         * `openInfoPopup` from the `ui/GeodeUI.hpp` header to open the Mod's
         * page to let the user install the update
         * @returns A task that resolves to an option, either the latest
         * available version on the index if there are updates available, or
         * `std::nullopt` if there are no updates. On error, the Task returns
         * an error
         */
        CheckUpdatesTask checkUpdates() const;

        Result<> saveData();
        Result<> loadData();

        /**
         * Get the mod's save directory path
         */
        std::filesystem::path getSaveDir() const;
        /**
         * Get the mod's config directory path
         */
        std::filesystem::path getConfigDir(bool create = true) const;
        /**
         * Get the mod's persistent directory path
         * This directory is not deleted even when Geode/mod is uninstalled
         */
        std::filesystem::path getPersistentDir(bool create = true) const;

        /**
         * Returns true if this mod has any settings
         */
        bool hasSettings() const;
        /**
         * Get a list of all this mod's setting keys (in the order they were
         * declared in `mod.json`)
         */
        std::vector<std::string> getSettingKeys() const;
        bool hasSetting(std::string_view key) const;

        /**
         * Get the definition of a setting, or null if the setting was not found,
         * or if it's a custom setting that has not yet been registered using
         * `Mod::registerCustomSettingType`
         * @param key The key of the setting as defined in `mod.json`
         */
        std::shared_ptr<Setting> getSetting(std::string_view key) const;

        /**
         * Register a custom setting type. See
         * [the setting docs](https://docs.geode-sdk.org/mods/settings) for more
         * @param type The type of the setting. This should **not** include the
         * `custom:` prefix!
         * @param generator A pointer to a function that, when called, returns a
         * newly-created instance of the setting type
         */
        Result<> registerCustomSettingType(std::string_view type, SettingGenerator generator);

        /**
         * Returns a prefixed launch argument name. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         */
        std::string getLaunchArgumentName(std::string_view name) const;
        /**
         * Returns the names of the available mod-specific launch arguments.
         */
        std::vector<std::string> getLaunchArgumentNames() const;
        /**
         * Equivalent to a prefixed `Loader::hasLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        bool hasLaunchArgument(std::string_view name) const;
        /**
         * Get a mod-specific launch argument. This is equivalent to `Loader::getLaunchArgument`
         * with the argument name prefixed by the mod ID. For example, a launch argument named
         * `mod-arg` for the mod `author.test` would be specified with `--geode:author.test.mod-arg=value`.
         * @param name The argument name
         * @return The value, if present
         */
        std::optional<std::string> getLaunchArgument(std::string_view name) const;
        /**
         * Equivalent to a prefixed `Loader::getLaunchFlag` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        bool getLaunchFlag(std::string_view name) const;
        /**
         * Equivalent to a prefixed `Loader::parseLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        template <class T>
        std::optional<T> parseLaunchArgument(std::string_view name) const {
            return Loader::get()->parseLaunchArgument<T>(this->getLaunchArgumentName(name));
        }

        matjson::Value& getSaveContainer();
        matjson::Value& getSavedSettingsData();

        /**
         * Get the value of a [setting](https://docs.geode-sdk.org/mods/settings).
         * To use this for custom settings, first specialize the
         * `SettingTypeForValueType` class, and then make sure your custom
         * setting type has a `getValue` function which returns the value
         */
        template <class T>
        T getSettingValue(std::string_view key) const {
            using S = typename SettingTypeForValueType<T>::SettingType;
            if (auto sett = cast::typeinfo_pointer_cast<S>(this->getSetting(key))) {
                return sett->getValue();
            }
            return T();
        }

        template <class T>
        T setSettingValue(std::string_view key, T const& value) {
            using S = typename SettingTypeForValueType<T>::SettingType;
            if (auto sett = cast::typeinfo_pointer_cast<S>(this->getSetting(key))) {
                auto old = sett->getValue();
                sett->setValue(value);
                return old;
            }
            return T();
        }

        bool hasSavedValue(std::string_view key);

        template <class T>
        T getSavedValue(std::string_view key) {
            auto& saved = this->getSaveContainer();
            if (auto res = saved.get(key).andThen([](auto&& v) {
                return v.template as<T>();
            }); res.isOk()) {
                return res.unwrap();
            }
            return T();
        }

        template <class T>
        T getSavedValue(std::string_view key, T const& defaultValue) {
            auto& saved = this->getSaveContainer();
            if (auto res = saved.get(key).andThen([](auto&& v) {
                return v.template as<T>();
            }); res.isOk()) {
                return res.unwrap();
            }
            saved[key] = matjson::Value(defaultValue);
            return defaultValue;
        }

        /**
         * Set the value of an automatically saved variable. When the game is
         * closed, the value is automatically saved under the key
         * @param key Key of the saved value
         * @param value Value
         * @returns The old value
         */
        template <class T>
        T setSavedValue(std::string_view key, T const& value) {
            auto& saved = this->getSaveContainer();
            auto old = this->getSavedValue<T>(key);
            saved[key] = value;
            return old;
        }

        /**
         * Get the Mod of the current mod being developed
         * @returns The current mod
         */
        template <class = void>
        static inline GEODE_HIDDEN Mod* get() {
            if (!sharedMod<>) {
                sharedMod<> = takeNextLoaderMod();
            }
            return sharedMod<>;
        }

        /**
         * Create a hook at an address. Call the original
         * function by calling the original function –
         * no trampoline needed
         * @param address The absolute address of
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
         * @param displayName Name of the hook that will be
         * displayed in the hook list
         * @param convention Calling convention of the hook
         * @param hookMetadata Metadata of the hook
         * @returns Successful result containing the
         * Hook pointer, errorful result with info on
         * error
         */
        template<class DetourType>
        Result<Hook*> hook(
            void* address, DetourType detour, std::string const& displayName = "",
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default,
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto hook = Hook::create(address, detour, displayName, convention, hookMetadata);
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook)));
            return Ok(ptr);
        }

        Result<Hook*> hook(
            void* address, void* detour, std::string const& displayName,
            tulip::hook::HandlerMetadata const& handlerMetadata,
            tulip::hook::HookMetadata const& hookMetadata
        ) {
            auto hook = Hook::create(address, detour, displayName, handlerMetadata, hookMetadata);
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook)));
            return Ok(ptr);
        }

        /**
         * Claims an existing hook object, marking this mod as its owner.
         * If the hook has "auto enable" set, this will enable the hook.
         * @returns Returns a pointer to the hook, or an error if the
         * hook already has an owner, or was unable to enable the hook.
         */
        Result<Hook*> claimHook(std::shared_ptr<Hook> hook);

        /**
         * Disowns a hook which this mod owns, making this mod no longer its owner.
         * If the hook has "auto enable" set, this will disable the hook.
         * @returns Returns an error if this mod doesn't own the hook, or
         * if disabling the hook failed.
         */
        Result<> disownHook(Hook* hook);

        /**
         * Get all hooks owned by this Mod
         * @returns Vector of hooks
         */
        [[nodiscard]] std::vector<Hook*> getHooks() const;

        /**
         * Write a patch at an address
         * @param address The address to write into
         * @param data The data to write there
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<Patch*> patch(void* address, ByteVector const& data) {
            auto patch = Patch::create(address, data);
            GEODE_UNWRAP_INTO(auto ptr, this->claimPatch(std::move(patch)));
            return Ok(ptr);
        }

        /**
         * Claims an existing patch object, marking this mod as its owner.
         * If the patch has "auto enable" set, this will enable the patch.
         * @returns Returns a pointer to the patch, or an error if the
         * patch already has an owner, or was unable to enable the patch.
         */
        Result<Patch*> claimPatch(std::shared_ptr<Patch> patch);

        /**
         * Disowns a patch which this mod owns, making this mod no longer its owner.
         * If the patch has "auto enable" set, this will disable the patch.
         * @returns Returns an error if this mod doesn't own the patch, or
         * if disabling the patch failed.
         */
        Result<> disownPatch(Patch* patch);

        /**
         * Get all patches owned by this Mod
         * @returns Vector of patches
         */
        [[nodiscard]] std::vector<Patch*> getPatches() const;

        /**
         * Enable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> enable();

        /**
         * Disable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> disable();

        /**
         * Delete the mod's .geode package.
         * @param deleteSaveData Whether should also delete the mod's save data
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> uninstall(bool deleteSaveData = false);
        bool isUninstalled() const;

        ModRequestedAction getRequestedAction() const;

        /**
         * Check whether or not this Mod
         * depends on another mod
         */
        bool depends(std::string_view id) const;

        /**
         * Check whether all the required
         * dependencies for this mod have
         * been loaded or not
         * @returns True if the mod has unresolved
         * dependencies, false if not.
         */
        bool hasUnresolvedDependencies() const;
        /**
         * Check whether none of the
         * incompatibilities with this mod are loaded
         * @returns True if the mod has unresolved
         * incompatibilities, false if not.
         */
        bool hasUnresolvedIncompatibilities() const;

        std::string_view expandSpriteName(std::string_view name);

        /**
         * Get info about the mod as JSON
         * @note For IPC
         */
        ModJson getRuntimeInfo() const;

        /**
         * Get the current logging status for this mod.
         */
        bool isLoggingEnabled() const;
        /**
         * Set the logging status for this mod.
         * @param enabled Whether to enable or disable logging
         */
        void setLoggingEnabled(bool enabled);

        /**
         * Get the minimum log level for this mod.
         */
        Severity getLogLevel() const;
        /**
         * Set the minimum log level for this mod.
         * @param level The new log level
         */
        void setLogLevel(Severity level);

        /**
         * If this mod is built for an outdated GD or Geode version, returns the
         * `LoadProblem` describing the situation. Otherwise `nullopt` if the
         * mod is made for the correct version of the game and Geode
         */
        std::optional<LoadProblem> targetsOutdatedVersion() const;
        /**
         * @note Make sure to also call `targetsOutdatedVersion` if you want to
         * make sure the mod is actually loadable
         */
        bool hasLoadProblems() const;
        std::vector<LoadProblem> getAllProblems() const;
        std::vector<LoadProblem> getProblems() const;
        std::vector<LoadProblem> getRecommendations() const;
        bool shouldLoad() const;
        bool isCurrentlyLoading() const;

        friend class ModImpl;
    };
}

namespace geode::geode_internal {
    // this impl relies on the GEODE_MOD_ID macro set by cmake
    template <size_t N>
    struct StringConcatModIDSlash {
        static constexpr size_t extra = sizeof(GEODE_MOD_ID);
        char buffer[extra + N]{};
        constexpr StringConcatModIDSlash(const char (&pp)[N]) {
            char id[] = GEODE_MOD_ID;
            for (int i = 0; i < sizeof(id); ++i) {
                buffer[i] = id[i];
            }
            buffer[extra - 1] = '/';
            for (int i = 0; i < N; ++i) {
                buffer[extra + i] = pp[i];
            }
        }
    };
}

template <geode::geode_internal::StringConcatModIDSlash Str>
constexpr auto operator""_spr() {
    return Str.buffer;
}

/**
 * Leaves a marker in the binary that can be used to patch
 * the game at a specific offset with a specific byte sequence.
 * Used for runtime patchless install.
 * @example
 * ```cpp
 * GEODE_MOD_STATIC_PATCH(0x1234, "\x12\x34\x56\x78");
 * GEODE_MOD_STATIC_PATCH(0x5678, {0x12, 0x34, 0x56, 0x78});
 * ```
 */
#define GEODE_MOD_STATIC_PATCH(Offset_, ...) \
    doNotOptimize(utils::string::ConstexprString::toLiteral([](){\
        utils::string::ConstexprString str2;                     \
        str2.push(__VA_ARGS__);                                  \
        utils::string::ConstexprString str;                      \
        str.push("[GEODE_PATCH_SIZE]");                          \
        str.push(str2.size(), 16);                               \
        str.push("[GEODE_PATCH_BYTES]");                         \
        str.push(str2);                                          \
        str.push("[GEODE_PATCH_OFFSET]");                        \
        str.push(Offset_, 16);                                   \
        str.push("[GEODE_PATCH_END]");                           \
        return str;                                              \
    }))

/**
 * Leaves a marker in the binary that can be used to hook
 * the game at a specific offset with a specific detour.
 * Used for runtime patchless install.
 * @example
 * ```cpp
 * auto res = GEODE_MOD_STATIC_HOOK(0x1234, &myDetour, "MenuLayer::init");
 * ```
 */
#define GEODE_MOD_STATIC_HOOK(Offset_, Detour_, ...) \
    (doNotOptimize(utils::string::ConstexprString::toLiteral([](){ \
        utils::string::ConstexprString str;                        \
        str.push("[GEODE_MODIFY_NAME]");                           \
        str.push(GEODE_STR(__VA_ARGS__));                          \
        str.push("[GEODE_MODIFY_OFFSET]");                         \
        str.push(Offset_, 16);                                     \
        str.push("[GEODE_MODIFY_END]");                            \
        return str;                                                \
    })), geode::Mod::get()->hook(                                  \
        reinterpret_cast<void*>(geode::base::get() + Offset_),     \
        Detour_,                                                   \
        GEODE_STR(__VA_ARGS__)))