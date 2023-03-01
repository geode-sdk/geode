# Geode Changelog

## v1.0.0-beta.9
 * Fix multiple modifiers not being able to have fields on same class due to having same field index (7710fa9)
 * Add `Result::ok` and `Result::err` for converting the `Result` into `std::optional` (4a15afc)

## v1.0.0-beta.8
 * Unload the mod even when first time warning pops up (63b4774)
 * Error when address of a function returns nullptr when hooking (724a9d3)
 * Add support for Geode CLI v2.0.0 (which has not been released yet) (088ac7b, deadb58)
 * Logging no longer causes a crash on invalid formats, but instead just warns (6aba7cf)
 * `file::pickFile` now uses the last item in the default path as the default filename to save/open (5c9ee08)
 * Fix `EditorPauseLayer` crashing constantly due to some members being accidentally set to `nullptr` (33a91d6)

## v1.0.0-beta.7
 * Add `Mod::getResourcesDir` for getting the mod resources directory (0055032)
 * Deprecate `file::listFiles` for `file::readDirectory` (9f60091)
 * Fix getting virtual function addresses for CCFileUtils (c183a35)
 * Rename `BasedButtonSprite` sprite names to be more expressive (4d2daec)
 * Fix `typeinfo_cast` causing a crash if passed a `nullptr` on MacOS (d024dbb)
 * Fix settings not getting broadcasted (7089194)
 * Make `Loader::getLoadedMod` and `Loader::isModLoaded` also check if mod is enabled (3222097)
 * Display Geode commit hash in the bottom right of the mod info layer (1dfa907)
 * Fix `EditorPauseLayer` info labels on the top left being too big (f5983a2)

## v1.0.0-beta.6

 * Reworked layouts from the ground up - see [the docs page](https://docs.geode-sdk.org/tutorials/layouts) for more information about how the new system works (#137)
 * Update the IDs for a bunch of layers, aswell as adding some predefined layouts (3f64b98, ef9e741, a78bc0c, cb1a6f1, ea037a9, f7ddf0a, ...)
 * Add IDs & layouts to `EditorPauseLayer` (12d88ae)
 * Add `CCNode::insertBefore` and `CCNode::insertAfter` for adding children before/after existing ones (eb10eca, 4613af6)
 * Add `CCSize::aspect` convenience method
 * Add `Mod::getResourcesDir` for getting a mod's runtime resources directory (where `[mod.json].resources.files` are placed) (0055032)
 * Add `Mod::addCustomSetting` for convenience in registering custom settings (7089194)
 * Add `file::readDirectory` as a sanely named alternative to `file::listFiles` (9f60091)
 * Move `GEODE_DLL` to the structs themselves in `JsonValidation` (06bc6fd)
 * Versions now support tag numbers & version tags are now used in comparisons. This change does not affect old betas, which still internally report their version as just `v1.0.0-beta`, but starting with this beta the version is correctly reported as `v1.0.0-beta.6` and correctly compared against other versions (bbbf332)
 * `Loader::getLoadedMod` and `Loader::isModLoaded` now only return if the mod is also enabled (3222097)
 * Geode's internal mod representation is now included in the loader's loaded mods list (4261e99)
 * Fix settings value changes not being broadcast. This causes an API break relating to custom settings; `SettingValue` now requires the owner mod ID in its constructor (7089194)
 * Fix some warnings (9c9706b, f7bfa21)
 * Fix `CCNode::swapChildIndices` (ba0851e)
 * Fix `typeinfo_cast` causing a crash if passed a `nullptr` (f4a3258)
 * Fix `ranges::reverse` causing UB (ffd50eb)
 * Other fixes & improvements (cb00c21)

## v1.0.0-beta.5

- Make ModInfo Pimpl 51990ad89b25cecbabaf748a5bcb279227fce090
- Fix crash with event listeners 1f7d50a9b9140d02f6a9afb97734eb9761b6a0d4
- Some bindings 4a9f6ba52a3d756d9bc28c1809afc92479783673
- Make mods binaries not silently fail to load 0eb5f01ca81435cb90f2bc9d8d97a86405dadd1c
- Assume dependency version is >= comparison by default 41aef57758d7b858d5fa7cb22ab1ffe603ff365f
- Fix not following thunks 65f2cbb286cc1e5af57e43451862a2233d66453e

## v1.0.0-beta.4

- add some bindings
- fix macOS libzstd crash

## v1.0.0-beta.3

 - Better support for dependencies with [Geode CLI v1.4.x](https://github.com/geode-sdk/cli/releases/latest): mod dependencies are now automatically installed from the mods index by simply declaring them in your `mod.json`. See [the tutorial page in docs](https://docs.geode-sdk.org/mods/dependencies/) for more (f32aaa8b124bdd040a453bc25d31a4e463cf1309)
 - The `create_geode_file` CMake function has been replaced by `setup_geode_mod`. The old `create_geode_file` function is still available, but will be deprecated in the future (f32aaa8b124bdd040a453bc25d31a4e463cf1309)
 - `Result::except` now works with non-copyable types (f32aaa8b124bdd040a453bc25d31a4e463cf1309)
 - `Zip` and `Unzip` now support in-memory ZIP extraction and creation (f32aaa8b124bdd040a453bc25d31a4e463cf1309)
 - `ComparableVersionInfo::compare` now always returns false if the major versions are different (f32aaa8b124bdd040a453bc25d31a4e463cf1309)
 - `ComparableVersionInfo` parsing now expects equal to be marked with a single `=` instead of two (`==v1.2.3` => `=v1.2.3`) (6d3847d9e13fdf69cea0b9a69376ebcd88e71725)
 - Fix `DS_Dictionary`-related `gd::string` Cocos2d functions not being linked (ab0030136ab8c20731fe768ef5f3b16ae4245583)
 - `CC_DLL` no longer expands to dllexport/dllimport (ab0030136ab8c20731fe768ef5f3b16ae4245583)
 - The JSON lib now default constructs to object, hopefully fixing uncaught bugs (30101fc0b5112317aac3c9eeea0aab888ca8b30d)
 - Something related to codegen and addresser? I have no clue what it does, so you probably won't have either (ae1eb8bb7162342f598e047a459e3808801f731a, 1e9faac5aa5a32b0cb5e6ffaac0c5cbfc217e9cb, fea049cca069a08181ab66cacda6f2417c006a2f, ad261846919e503cc3faf75d777ab2f2882bc6bb, ...)
 - MacOS minimum version bumped to 10.14 (916f54063008c6bdf892d02f8bcd92b58606817e)

## v1.0.0-beta.2

 * Fixed bug where `Mod::getSavedValue` would cause a crash due to trying operator on a null JSON value (5bbd34c)
 * Fixed bug where loading would crash if one of the mods' binaries failed to load (ef86ae0)

## v1.0.0-beta.1

 * Switched to [a new custom-built JSON library](https://github.com/geode-sdk/json) to replace `nlohmann::json` for compile-time improvements; if you were using the old JSON library, you can add & link to `nlohmann::json` in your own project, or update to use the new API. (deab672)
 * Fix resources not being downloaded automatically by using a fallback to latest release (a418828)
 * Add a new clear instruction popup in case downloading still fails (30dc9ad)
 * String ID hooks now have higher priority, so they should always be applied regardless of if you call `NodeIDs::provideFor` or not (though it can still be called to absolutely ensure the IDs are there!) (b6a6e4d)
 * Various internal bugfixes & improvements (ceb02e9, a90b3e1, b00ab40, c644b43)


## v1.0.0-alpha

 - Major rework of the entire framework; most notable changes include switching to a whole new hooking framework (TulipHook), simplifying many parts of the framework, and making it production-ready.

## v0.7.0
 - Jesus H. Christmas Kallen there's like 300 new commits since v0.6.1 I don't think there's a point in listing them all, we basically redesigned the whole framework

## v0.6.1
 - Add `geode::cocos::switchToScene` utility for easily switching to a layer with the default fade transition
 - Add `Mod::getPackagePath` as a replacement for `Mod::getPath`
 - Add `geode/config` directory as a standardized place for mods to add their config files
 - Add `Mod::getConfigDir` for getting a mods' config directory
 - Add open config directory button to mods with a config directory
 - Add open save directory button to mods' settings popup
 - Removed deprecation from `Result` (it still will be replaced in v1.0.0, we just got tired of the warnings)
 - `JsonChecker<nlohmann::json>` and `JsonChecker<nlohmann::ordered_json>` are now defined in-source as exported symbols to speed up compilation times
 - Add more bindings

## v0.6.0

 - Mod resource loading has been reworked again, with the intent of adding support for texture pack loaders
 - Added `Loader::addTexturePath` and `Loader::removeTexturePath` to work with additional resource paths
 - Mods that work with Cocos2d search paths directly should convert to using the above functions
 - New header `Geode/ui/LayerBG.hpp` with `createLayerBG` function for creating the blue gradient background GD layers have
 - All Cocos2d include paths have been changed to be relative
 - `cocos2dx` folder has been removed, cocos2d is now directly inside `Geode/cocos`
 - Loader resources updating will no longer check for plist file differences due to problems with CLI on Mac
 - More codegen optimizations, Geode mods should now compile faster
 - Added `NewResult` class, which will eventually replace the old `Result`
 - Add `deprecate` and `nodiscard` attributes to the old `Result` class
 - Cocos2d selectors (like `SEL_MenuHandler`) no longer require `using namespace cocos2d`
 - Many more bindings on Mac and Windows
 - Add IDs to `LevelSearchLayer` (thank you @Jollycistaken)

## v0.5.0

 - Added `CCFileUtils::get`
 - Fix crashes related to setting IDs in `MenuLayer`
 - Remove `Loader::updateModResourcePaths` and `Loader::updateResourcePaths`. The minimum mod target version is still v0.4.0 however, as you should never have been using these functions.
 - Rework how mod resources are added
 - Deprecate `geode::utils::vector` and `geode::utils::container` namespaces and replace them with `geode::utils::ranges`
 - Finally added a license to Geode! The framework is now licensed under BSL v1.0.

## v0.4.8

 - CLI issues fixed in v1.0.6 so loader again verifies if loader resources are missing / corrupt on startup
 - Resource download text is no longer a popup on top of the title but instead just replaces the loading text
 - Add delegates to `EditLevelLayer`

## v0.4.7

 - Loader resources check would always fail due to CLI issues, so for now loader just checks if the resources folder exists

## v0.4.6

 - Automatically checks & downloads loader resources if they are missing / corrupt on startup
 - CMake rework; `GeodeFile.cmake` now checks and verifies CLI version
 - Add optional `DONT_INSTALL` parameter to `create_geode_file`
 - Test mods are now no longer automatically installed
 - Add `package_geode_resources_now` command for packaging resources at configure time and creating a header with their calculated hashes
 - Fix `getSceneDelegate`
 - Change `CCArrayExt` to use `Ref`

## v0.4.5

 - Rework bindings and codegen to improve compile times, now individual bindings can be included with `<Geode/binding/{ClassName}.hpp>`
 - Modify has also been separated, you can now include individual modifiers with `<Geode/modify/{ClassName}.hpp>`
 - Various other fixes to improve compile times
 - Fix mod resources not being loaded when installed from Index
 - Fix crashes related to downloading mods
 - Fix `Loader::queueInGDThread` sometimes leaving out functions
 - Fix crashes related to logging
 - Add new overloads to `file` utils and deprecate ones that don't use `ghc::filesystem::path`
 - Index mods now show their `about.md` files
 - More addresses
 - Various other fixes & improvements
 - Index reworked
 - Fix issues with `VERSION` file
 - Add `GEODE_DEBUG` macro for enabling `log::debug` to actually print stuff
 - Show crashlog on crash when `GEODE_DEBUG` is enabled
 - Add `JsonChecker::at` and `JsonChecker::array` for dealing with arrays
 - Add `geode::utils::web::fetchBytes` for fetching a byte array synchronously
 - Add `geode::utils::web::AsyncWebRequest` for creating thread-safe asynchronous web requests
 - Add `Loader::updateModResourcePaths` for adding a mods' resources to search paths. Not recommended to be called manually
 - Add an overload to `geode::createQuickPopup` for specifying popup width
 - `ModInfo::createFromFile` now checks for `about.md` and other special files in the same directory
 - Remove automatic mod updating for now, however automatic update checking for mods is still there

## v0.4.4

 - New `listenForSettingChanges` API for more ergonomically listening for setting changes
 - Fixed bug where GD was unopenable through Steam
 - Various other internal fixes

## v0.4.3

 - Simplified the minimum and maximum loader versions, loader will now load any mod whose target version major and minor match. In practice, this means that for example mods whose target version is v0.4.8 can be loaded by loader of version v0.4.6.
 - Add `Geode/ui/GeodeUI.hpp` header for exposing some access to internal Geode UI like opening a mod's settings popup
 - Fix crash with settings that could have a slider control

## v0.4.2

 - Moved SDK version to its own file so CLI can query it
 - md4c is now linked statically on MacOS
 - Fix log filenames

## v0.4.1

 - Initial dev release of Geode.

---

Note that from here on, changes to the framework were not tracked by versions as the framework was still considered to be in heavy development and not released. Instead, major changes are listed by dates.

## 2022/10/10

 - Geode released for developers

## 2022/10/08

 - `ui` branch merged to `main`

## 2022/10/03

 - New CLI finished
 - `ui` branch finished

## 2022/08/01

 - CLI redesign started

## 2022/07/30

 - `sdk`, `loader` and `api` repos all merged into one `geode` repo

## 2022/05/24

 - Geode announced to be merging with Hyperdash, later on it turned out we were all pathological liars

## 2022/05/02

 - Installer on Windows

## 2022/04/30

 - Installing mods in-game works

## 2022/01/23

 - CLI started

## 2022/01/19

 - Lilac and Cacao merged and renamed to Geode

## 2021/07/30

 - Lilac started by Mat, HJfod and Pie

## 2021/01/25

 - CacaoSDK started by Camila, based on CappuccinoSDK

