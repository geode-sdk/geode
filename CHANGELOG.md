# Geode Changelog

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

