# Geode Changelog

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

