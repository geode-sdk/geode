# Geode Changelog

## v1.0.0
 * Make JSON library exported (a2b8f27)
 * Fix GLFW headers for Windows (6ada069)
 * Add crashlog to MacOS (9b58b33)
 * Implement a new updater / bootstrapper for Windows (b47870e)
 * Don't download resources if the tag doesn't exist (991e714)
 * Fix MacOS wine by using `std::filesystem::weakly_canonical` (46fab0e)

## v1.0.0-beta.18
 * Fix pickup bounds checks

## v1.0.0-beta.17
 * Fix `Mod::addHook` (372e2aa)
 * Enable ANSI color support on Windows for logs (af8d4a0)
 * Better unicode support for paths on Windows (a1e9ac4)

## v1.0.0-beta.16
 * Fix mod.json not loading because of broken working directory on MacOS (d7ccfde)
 * Fix markdown resources not updating between versions (b0cf6dc)

## v1.0.0-beta.15
 * Fix `ghc::filesystem::create_directories` crash on wine (c174f81)
 * Fix fuzzy search giving meaningless results (7af062f)
 * Add logging for gd::string (e7ab46c)
 * Fix `ComparableVersionInfo` comparison being inverted (e7ab46c)
 * Add `FileWatchEvent` for file watching (50ff15c)
 * Fix `geode::utils::file::pickFile` on MacOS (8842e8f)
 * Fix `geode::utils::file::writeToJson` (1c699c3)
 * Fix notifications not being at the top (4754994)
 * Fix filter copy constructors (4cec5d7)
 * Update json library

## v1.0.0-beta.14
 * Error on trying to use missing bindings at compile time
 * Fix every resources getting reloaded every time a mod is loaded
 * Fix every listener getting called on every event post
 * Fix `CCTextInputNode` content size to be off
 * Return `ListenerResult` from events
 * Log severity with colors on MacOS

## v1.0.0-beta.13
 * Add `SpacerNode` for layouts
 * Add the `Dispatch` system for using events without linking to optional dependencies
 * Add `EventFilter::getCallback` and make `EventListener::getFilter` return a reference
 * Add `CCNode::getEventListenerCount`
 * Add IDs to `CustomSongWidget`
 * Rework event listeners to have them not all be placed in the same queue aswell as fixing issues with dangling pointers
 * Make `SentAsyncWebRequest` dllexported, allowing mods to use async web requests
 * Fix issues with `WeakRef`
 * Fix being able to recursively click on developer names to open the list of their created mods

Thank you to [Fleeym](https://github.com/Fleeym/Fleeym) for contributing to this release!

## v1.0.0-beta.12
 * Fix crash when installing mods 
 * FMOD is now linked on MacOS 
 * `GEODE_DEBUG` on longer needs to be defined for `log::debug` to work 
 * Make Geode changelog visible in-game
 * Make the changelog only be loaded once the changelog button is pressed in ModInfoPopup
 * Fix the scrollbar not working for the changelog in ModInfoPopup
 * Fix visual issues with scrollbars

## v1.0.0-beta.11
 * New `geode::prelude` namespace to replace the old `USE_GEODE_NAMESPACE()` macro 
 * Add `CCNode::removeChildByID` 
 * Add `CCNode::hasAncestor` 
 * Add `CCScene::get` and `CCScheduler::get` 
 * Add `geode::cocos::getMousePos` 
 * Add `GEODE_DONT_INSTALL_MODS` option to CMake 
 * Add logging `std::vector` with `log::` functions 
 * Add `EventListener::getFilter` 
 * Add `AttributeSetEvent` for detecting when attributes are added to nodes 
 * Add `CCNode::addEventListener` (along with other related functions) as convenience methods for adding event listeners that target nodes 
 * Add `WeakRef` as a weak pointer alternative to `Ref` (see [the docs](https://docs.geode-sdk.org/tutorials/memory#weakref) for a tutorial) 
 * Add option to ignore invisible children to `Layout` 
 * `CCNode` attributes now use `json::Value` over `std::any` for ABI compatability 
 * Implement file picker on Mac 
 * Define `CCNode::retainCount` inline 
 * `Layout` now inherits from `CCObject`, allowing you to share layouts across multiple nodes 
 * Update TulipHook version 
 * Make sure mod load/enable/etc. events are only ever posted in the GD thread 
 * `Mod::getResourcesDir` now returns `geode/unzipped/{mod.id}/resources/{mod.id}` in conjunction with [CLI v2.1.0](https://github.com/geode-sdk/cli/releases/tag/v2.1.1) 
 * Give a name to `ccTouchType` 
 * Fix `Scrollbar` being funky sometimes 
 * Fix mod resources not being loaded if the mod is enabled at runtime 
 * Fix `EditLevelLayer` description update button ID & layout 
 * Fix hooking functions with unpredictable calling conventions 
 * Fix `setup_geode_mod` not linking to Geode if CLI calls are disabled 
 * Fix code editors showing a ton of warnings with `$modify` 
 * Fix top sprite sizes of `CircleButtonSprite` and `EditorButtonSprite` 
 * Fix `Mod::enableHook` error message 
 * Lots of bindings on both Windows & Mac (50+ commits related to bindings)

## v1.0.0-beta.10
 * Fix loader minimum mod version preventing the loader itself from loading 
 * Fix recursive comparison in VersionTag 
 * `geode/unzipped` is now deleted on startup if it exists 

## v1.0.0-beta.9
 * Fix multiple modifiers not being able to have fields on same class due to having same field index 
 * Add `Result::ok` and `Result::err` for converting the `Result` into `std::optional` 

## v1.0.0-beta.8
 * Unload the mod even when first time warning pops up 
 * Error when address of a function returns nullptr when hooking 
 * Add support for Geode CLI v2.0.0 (which has not been released yet) 
 * Logging no longer causes a crash on invalid formats, but instead just warns 
 * `file::pickFile` now uses the last item in the default path as the default filename to save/open 
 * Fix `EditorPauseLayer` crashing constantly due to some members being accidentally set to `nullptr` 

## v1.0.0-beta.7
 * Add `Mod::getResourcesDir` for getting the mod resources directory 
 * Deprecate `file::listFiles` for `file::readDirectory` 
 * Fix getting virtual function addresses for CCFileUtils 
 * Rename `BasedButtonSprite` sprite names to be more expressive 
 * Fix `typeinfo_cast` causing a crash if passed a `nullptr` on MacOS 
 * Fix settings not getting broadcasted 
 * Make `Loader::getLoadedMod` and `Loader::isModLoaded` also check if mod is enabled 
 * Display Geode commit hash in the bottom right of the mod info layer 
 * Fix `EditorPauseLayer` info labels on the top left being too big 

## v1.0.0-beta.6
 * Reworked layouts from the ground up - see [the docs page](https://docs.geode-sdk.org/tutorials/layouts) for more information about how the new system works (#137)
 * Update the IDs for a bunch of layers, aswell as adding some predefined layouts (3f64b98, ef9e741, a78bc0c, cb1a6f1, ea037a9, f7ddf0a, ...)
 * Add IDs & layouts to `EditorPauseLayer` 
 * Add `CCNode::insertBefore` and `CCNode::insertAfter` for adding children before/after existing ones 
 * Add `CCSize::aspect` convenience method
 * Add `Mod::getResourcesDir` for getting a mod's runtime resources directory (where `[mod.json].resources.files` are placed) 
 * Add `Mod::addCustomSetting` for convenience in registering custom settings 
 * Add `file::readDirectory` as a sanely named alternative to `file::listFiles` 
 * Move `GEODE_DLL` to the structs themselves in `JsonValidation` 
 * Versions now support tag numbers & version tags are now used in comparisons. This change does not affect old betas, which still internally report their version as just `v1.0.0-beta`, but starting with this beta the version is correctly reported as `v1.0.0-beta.6` and correctly compared against other versions 
 * `Loader::getLoadedMod` and `Loader::isModLoaded` now only return if the mod is also enabled 
 * Geode's internal mod representation is now included in the loader's loaded mods list 
 * Fix settings value changes not being broadcast. This causes an API break relating to custom settings; `SettingValue` now requires the owner mod ID in its constructor 
 * Fix some warnings 
 * Fix `CCNode::swapChildIndices` 
 * Fix `typeinfo_cast` causing a crash if passed a `nullptr` 
 * Fix `ranges::reverse` causing UB 
 * Other fixes & improvements 

## v1.0.0-beta.5
- Make ModInfo Pimpl
- Fix crash with event listeners
- Some bindings
- Make mods binaries not silently fail to load
- Assume dependency version is >= comparison by default
- Fix not following thunks

## v1.0.0-beta.4
- Add some bindings
- Fix macOS libzstd crash

## v1.0.0-beta.3
 - Better support for dependencies with [Geode CLI v1.4.x](https://github.com/geode-sdk/cli/releases/latest): mod dependencies are now automatically installed from the mods index by simply declaring them in your `mod.json`. See [the tutorial page in docs](https://docs.geode-sdk.org/mods/dependencies/) for more 
 - The `create_geode_file` CMake function has been replaced by `setup_geode_mod`. The old `create_geode_file` function is still available, but will be deprecated in the future 
 - `Result::except` now works with non-copyable types 
 - `Zip` and `Unzip` now support in-memory ZIP extraction and creation 
 - `ComparableVersionInfo::compare` now always returns false if the major versions are different 
 - `ComparableVersionInfo` parsing now expects equal to be marked with a single `=` instead of two (`==v1.2.3` => `=v1.2.3`) 
 - Fix `DS_Dictionary`-related `gd::string` Cocos2d functions not being linked 
 - `CC_DLL` no longer expands to dllexport/dllimport 
 - The JSON lib now default constructs to object, hopefully fixing uncaught bugs 
 - Something related to codegen and addresser? I have no clue what it does, so you probably won't have either 
 - MacOS minimum version bumped to 10.14 

## v1.0.0-beta.2
 * Fixed bug where `Mod::getSavedValue` would cause a crash due to trying operator on a null JSON value 
 * Fixed bug where loading would crash if one of the mods' binaries failed to load 

## v1.0.0-beta.1
 * Switched to [a new custom-built JSON library](https://github.com/geode-sdk/json) to replace `nlohmann::json` for compile-time improvements; if you were using the old JSON library, you can add & link to `nlohmann::json` in your own project, or update to use the new API. 
 * Fix resources not being downloaded automatically by using a fallback to latest release 
 * Add a new clear instruction popup in case downloading still fails 
 * String ID hooks now have higher priority, so they should always be applied regardless of if you call `NodeIDs::provideFor` or not (though it can still be called to absolutely ensure the IDs are there!) 
 * Various internal bugfixes & improvements 


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

