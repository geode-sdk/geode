# Geode Changelog

## v1.4.0
 * Add Android support !!!!
   * Implement every Geode functionality except `utils::file::openFolder`
   * Requires the Geode launcher in order to be used
   * Fixes the text input node allowing typing for 1 less character
   * Uses `logcat` in order to get crash reports, so reopening the game is required to generate them
   * Broma requires classes to be added `[[link(android)]]` in order to be linked
   * All Geode and GD files are stored in `Android/data/com.geode.launcher/files`
     * Game files in `game`, save files in `save`
 * Allow logging to be disabled per mod (6d599a5)
 * Mod cells use layouts (114fa46)
 * MacOS console is now separate (182984d)
 * Add uninstall button to Geode mod (Only functional in Windows currently) (a738320)
 * Make new version label invisible on download (0f179da)
 * Fix the toggling of disabled dependencies (cd89ef1)
 * Fix spritesheet issues (ef47647)
 * Change `LoadingLayer` (ef47647)
 * Make mod info popup top a layout (dd806e0)
 * Add `GEODE_HIDDEN` to inline unique functions (71a79ab)
 * Fix big mod icons (26a6c7e)
 * Fix `CCNode::removeChildByID` export (23cd456)
 * Make `MDTextArea` fit its size (140f38b)
 * Enable ESC/Back to go back in Geode mod list (2847bee)
 * Add `SimpleTextArea` (7f277a7)
 * Check modified date when unzipping `.geode` files (5c765c6)
 * Only hash markdown files on resource checking (f563c46)

## v1.3.10
 * Panic if decompressString2 fails, to prevent data loss (0787b8f4)

## v1.3.9
 * Fix followThunkFunction (4b766301)

## v1.3.8
 * Implement a save file fix for Windows (391f63ed)
 * Recursively follow jumps in followThunkFunction (44a018cd)
 * Remove need for GEODE_DEBUG for crashlogs (e8a326f7)
 * Some bindings (f18335fa)

## v1.3.7
 * Fix web download deadlock (16418562)
 * Set max time for updating index notification (f7962246)
 * Log geode version on startup (c5550a67)
 * Fix logic error in addHook (5cf0f3c2)
 * Improve logging + minor refactors (5083017b)
 * Some bindings changes

## v1.3.6
 * Allow error responses in our WebRequest classes (237128bf)
 * Display unhandled C++ exceptions in crash log (fdd78aca, 0d091626, 52421d8c, 0472075f)
 * Fix GEODE_CLI force caching when not found (0a113744)
 * Only write checksum file after unzipping, fixes inconsistent index state (b4fbea51)
 * Fix the index notif staying on all the time (c967b520)
 * Bump minimum required CMake version in codegen (27ed63e7)
 * Only show update indicator if mod is enabled (8762714c)
 * Fix FLAlertLayer m_scrollingLayer not being a ScrollingLayer (9694b35d)
 * Fix gnustl vector dtor (b55e6465, 0bdb0df7)
 * Loads of bindings changes

## v1.3.5
 * Follow redirect in web::utils functions (a942a45)
 * Lots of bindings
 * Make codegen symbols private visibility (696a2ca)
 * Add deadstrip to macos (0d62940)
 * Readd the nullptr check in InstallListPopup::createCells (499f256)
 * Fix garagelayer ids on not logged in users (dd0179c)

## v1.3.4
 * Implement string setting character filters (cf8fbba)
 * Update bindings

## v1.3.3
 * Reunify resources.zip (81de161)

## v1.3.2
 * Fix alignment of some textures (8f39c38)
 * Bring back unknown problems (0663569)
 * Fix some Windows 7 incompatibility (2d2bdd1)
 * Remove enabled from the crashlogs (5b7d318)
 * Make index unzipping async (7c582f1)
 * Fix mods by developer crashing when mod was toggled (a6a47bf)
 * Fix nested lists in the markdown (2723588)
 * Fix search paths (8f39c38, aa55ebe)

## v1.3.1
 * Fix TulipHook not relocating RIP relative operands on MacOS (6cad19d)

## v1.3.0
 * Completely remove runtime enabling & disabling of mods (d817749)
 * Patches auto enabling can be disabled (69821f3)
 * Move ModEventType::Loaded to after hooks & patches are enabled (23c3095)
 * Update index to be able to store multiple versions of the same mod (5572f9c)
 * Implement UI for selection of downloading specific mod versions (5d15eb0)
 * Change install & uninstall popups to reflect the new changes (d40f467)
 * Keep the scroll when enabling, installing etc. a mod (b3d444a)
 * Update MacOS crashlog to include base and offset (7816c43)
 * Add user agent to AsyncWebRequest (c256207)
 * Add post and custom requests to AsyncWebRequest (c256207)

## v1.2.1
 * Mods now target macOS 10.13 instead of 10.14 (7cc1cd4)
 * Fix CustomizeObjectLayer ids moving around when multiple objects are selected (9ee0994, 87749d4)
 * Fix Windows uninstaller trying to uninstall from parent directory instead of current (bcb8a6f)
 * Fix search paths on reload (65b4364)
 * Fix search paths missing for mods without spritesheets (e78eaf0)
 * Fix dependencies getting installed again when they're already installed (4f2835a)

## v1.2.0
 * Add Objective-C hooking utilities (a31d072)
 * Rename queueInGDThread to queueInMainThread (277f68b)
 * Fix build on apple silicon (dd16797)
 * Move windows uninstaller to gd folder (bdf11a2)
 * Option to pick features on windows installer (4cc9164)
 * Improve lag in some places due to search paths (9875f5d)
 * Some missing methods for macOS gd::map (db02c3d)
 * Some missing methods for windows gd::string (7139ac4)

## v1.1.1
 * Improve installation confirmation popup (9192769)
 * Remove unnecessary main thread queues for mod events (38cc38c)
 * Fix search and filter buttons being not clickable when over the view/restart button of a mod (ef1f1d1)
 * Improve tab textures (108f56a)
   * Properly align the borders
   * Make the selected and unselected tabs the same height

## v1.1.0
 * Fix json library not actually being dynamically exported/imported (5f65d97)
 * Update TulipHook, gets rid of keystone dependency and adds stdcall support (efcbf58, 7b90903)
 * Make resources.zip platform dependent (e41784e)
 * Add utils::string::join (82e128b)
 * Add logger nesting: log::pushNest, log::pushNest (7d74f16)
 * Add "any" version to comparable versions (2b1dc17)
 * Deprecate ModInfo, replaced with ModMetadata (53b52ea)
 * Add utils::game::restart (7f449b9, 0e1d639)
 * Rework the way dependencies and mod loading works (5200128)
   * Early load loads mods before the game starts, while non-early load loads on the loading screen now (e718069)
   * Add support for specifying incompatibilities (5200128, 8908235)
   * Add support for specifying suggested and recommended optional dependencies (5200128)
     * Add UI to select which mods to install (3707418, 73169fb, cd772bd)
   * Dependencies/dependants automatically get toggled on toggle (5200128, 6ab542d)
   * Add problems list (5200128, aee84c0)
   * Show amount of currently loaded mods on the loading screen while they're loading (e718069, 1d5fae8)
 * Improve index-related UI (73169fb)
   * Remove Android and iOS filters for now
   * Add filter to show non-installable mods
   * API in quick popups to distinguish between pressing button 1 and the Escape key
 * Add "API" label to API mods (cb8759b)
 * Fix index not displaying tags (ed5b5c9)
 * Change "Cancel" to say "Keep" in the remove mod data on mod uninstall dialogue (cd772bd)
 * Fix typos in the word "successfully" (5200128, f316c86)
 * Fix MacOS HSV button missing in `CustomizeObjectLayer` (d98cb2d)
 * Make missing functions and members private (d98cb2d)
 * Update Broma to latest version (0a58432)
 * Implement standalone function support in codegen (a552160)
 * Add link attribute to codegen for functions linked from elsewhere (a552160)

## v1.0.3
 * Fix mod changelogs only showing Geode's own changelog (2945422)
 * Fix some filesystem functions for Geode running under wine (7a83354)
 * Lazy load the real XInput9 dll, removing the need for an XInput 1.3 (041d44c)
 * Fix MacOS requiring openssl@3 directly from homebrew (e489681)
 * Default to Steam installation on MacOS installer (072a18c)
 * Implement uninstaller for MacOS (072a18c)

## v1.0.2
 * Fix auto-updating (1f7e6da)

## v1.0.1
 * New textures for list tabs (8ce1227)
 * Switch libraries for SHA3 256 (bbf2608)
 * Fix MacOS installer (ea5a5f0)
 * Add mod ids to deprecated keys logs of ModInfo (fc72c24)
 * Rename platform labels: (MacOS -> macOS, iOS -> IOS) (cf3ed32)
 * Fix Geode not loading when GeodeBootsrapper.dll exists (c2aacd9)
 * Hide tag from version in cmake when it doesnt exist (fc8a8e1)
 * Change MacOS installer fmod original name for compatibility with the old installer (44d5aff)

## v1.0.0
 * Make JSON library exported (a2b8f27)
 * Fix GLFW headers for Windows (6ada069)
 * Add crashlog to MacOS (9b58b33)
 * Implement a new updater / bootstrapper for Windows (b47870e)
 * Don't download resources if the tag doesn't exist (991e714)
 * Fix MacOS wine by using `std::filesystem::weakly_canonical` (46fab0e)
 * Make the Index pimpl aswell as remove ability for multiple sources (7a0ade2)
 * All mods including disabled ones are now always unzipped in order to load their logos (0a6d31e)
 * Add IDs and layouts for `CustomizeObjectLayer` (6d92bb8, 138fdbb, f8da816)
 * Add option to disable the Last Crashed popup (e00b3a6)
 * Fix `SpacerNode` not actually taking up any space (8ca2b99)

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

## v1.0.0-beta.9
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

## v1.0.0-beta.2
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

