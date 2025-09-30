# Geode Changelog

## v4.9.0
 * Add Modtober 2025 content (#1501)
 * Add `geode::cocos::getObjectName` for getting demangled type names (c734b29)
 * Add mod load priority, load mods in alphabetical order (#1492, #1500)
 * Implement `CCImage::saveToFile` for macOS and iOS (#1496)
 * Allow using SDK without `GEODE_MOD_ID` macro (useful for static libs) (a6f215f)
 * Use OS APIs for file operations for improved performance and clearer error messages (ec46ad3)
 * Fix crashlog window title and icons on Windows (d527c08, efdde0a)
 * Fix custom settings not reloading dependants (#1488)
 * Fix `removeAllChildren` UB (dd9446b)
 * Fix Android `gd::string` causing double frees (#1490)
 * Optimize `pathToString` conversion (2cd8559)
 * `ConstexprString` can now be used as a template parameter (cf795fb)
 * Add `Self` type on `Modify` to make referring to self type more idiomatic (945f6d5)
 * Fix `aligned_storage` deprecation warnings when using C++23 (e7eed58)
 * Fix `CC_SYNTHESIZE` calls and define inline getters/setters for cocos (869dcfa)
 * EventV2 + async fixes (b3e6536)
 * Improve the outdated GD version warning (#1475)

## v4.8.0
 * Fix `geode::Notification` z ordering (127733e)
 * Add `CCNode::getChildrenExt` (d198df5)
 * Add `CCNode::getChildByIndex` (0ba1f2a)
 * Fix `GEODE_MOD_STATIC_PATCH` not using the geode namespace (#1456)
 * Fix Linux install script flatpak directory (2cf1e57)
 * Document return paths for `dirs` functions (d821172)
 * Add `VMTHookManager` for hooking virtual functions that have not been defined by virtual table patching (383f8fd)
 * Fix Unicode logging on Windows (c5e93b9)
 * Remove all `std::filesystem::path::string` calls as they can crash on Unicode paths (#1473)
 * Replace all `A` WinApi calls with `W` calls (#1473)
 * Hook and patch toggling (#1468)
 * Added option to force cURL to use IPv4 (#1412)

## v4.7.0
 * Change from copying queue to moving queue for main thread (9b8fb73)
 * Add reading from unzipped/launch-args.txt on iOS as an option (ae90fc9)
 * Add new "binary-dir" launcher flag for changing the path of mod binaries (0b71c3c)
 * Use ColorProvider for SwelvyBG colors (01ee436)
 * Deprecate the usage of geode::cast::as (9d28ab9)
 * Return success for patching if there is nothing to do (f92e5b6)
 * Updated TulipHook to 3.1.4 (f469696)
   * Fixes struct params and enabling after disabled.

## v4.6.3
 * Fix Windows crashlogs not appearing (for real this time) (dedf1eb)

## v4.6.2
 * Fix Windows crashlogs not appearing (02cee65)
 * Unzip JIT-less iOS binaries separately from the .geode file (85fe11f)

## v4.6.1
 * Fix several random crashes caused by incorrect relocation code in Tuliphook
 * Make unzip upon download iOS only (0a6d09c)

## v4.6.0
 * JIT-less iOS support! (#1395)
   * This means that Geode now supports iOS 26 beta and above
   * Follow instructions in the [iOS launcher guide](https://github.com/geode-sdk/ios-launcher/blob/main/INSTALL.md) for setting up
   * Old mods compiled for iOS need to be recompiled to work with this version, but other platforms are unaffected
   * There are new additions to help with JIT-less specific hooking and patching
     * Geode's own `Modify` classes should work out of the box
     * You can check if the platform is JIT-less iOS by using the `Loader::isPatchless` function
     * For custom hooks, you can use the `GEODE_MOD_STATIC_HOOK` macro to add a hook. This hook can be enabled and disabled like normal.
     * For custom patches, you can use the `GEODE_MOD_STATIC_PATCH` macro to add a patch. This patch will be applied by the launcher and can not be disabled.
     * If you have any dynamic patches, you can not use them with JIT-less iOS
   * This is a pretty big update, so there might be some issues with it. Please report the issues you find.
 * Major TulipHook update from 2.5.0 to 3.1.0 (#1395)
   * Adds debug logging behind the launcher flag `enable-tulip-hook-logs`
   * This update is a mainly internal one, but it is the most sensitive part of Geode
   * If you find any issues with it, please report them
 * Unzip on mod download (#1390)
 * Add Italian translation for the Windows installer (#1393)
 * Add `AndroidInputTimestampEvent` (#1396)
 * Fix `keyUp` when command key is pressed on MacOS (#1324)
 * Don't create a popup when there are no mods to update (#1311)
 * Add unzipped binary cleanup of unused platforms (#1377)
 * Add API badge and tag for mod listing (#1389)
 * Create Linux installer script (d38acee)
 * Use `FORCE_COLOR` environment variable to force terminal colors (79761bf)
 * Patch conditional variable on older Wine versions (53df4d3)
 * Make `ObjWrapper::getValue` non-const (5c3afbd)
 * Make Windows clipboard utilities unicode (82d8faf)
 * Use custom certificate store for web requests on Windows (e0c1774)
 * Add verbose curl logging (ec34df6)
 * Add `writeStringSafe` that writes to a temporary file first (af806ba)
 * Force unzip of mods on launch if the binary is missing (e4dd3e8)
 * Fix rare unzip crash (86fd2c4)
 * Fix PCH on iOS (0d972ab)
 * Don't load importance superseded mods (2c5230e)
 * Add some more documentation comments (9b27ddc)
 * Fix `LazySprite` overloads and performace issues (0f53f38, 5394954)
 * Error when trying to use `m_fields` in non-`CCNode` modify classes (0aed958)


## v4.5.0
 * Multipart form support for Geode web utilities (#1345)
 * Use system certificate store for web requests on Windows, should fix some SSL issues (665a000)
 * Fix windows file dialogs needing alt tab in full screen (f1a2b1b)
 * Improve iOS stack traces in crashlogs, show the memory address that caused the crash (230ad79)
 * Shorten DLL paths in Windows crashlogs, for DLLs that are from GD or mods (f6a5fb3)
 * Add a `geode::utils::string::pathToString` method, which safely handles UTF-16 conversions on Windows (95eed22, 138d071)
 * Fix base64 utilities not being exported and thus unusable by mods (dcd42df)
 * Fix `geode::utils::game::exit` and `restart` not linking on all platforms except Windows (50c09b0)
 * Fix `Mod::getLaunchArgumentName` not actually existing (#1356)
 * Fix `openFolder` not working properly on MacOS (b1a8464)
 * Fix `LazySprite` callback sometimes being called multiple times (d273ce6)
 * Fix `LazySprite` sometimes invoking undefined behavior (6b5146a)
 * Fix `WeakRef` leaking memory if a strong reference outlives all weak references (9587ce3)
 * Add a way to cancel `LazySprite` loading (3a35321)
 * Unify destructor fix across all platforms, replace a global map with a stack, fix Windows leaking references (#1350, b0f38ad)
 * Add some logs to web utilities and improve error messages (c2caafc)
 * Make AxisScaling::grow shrink to initial size if needed (0c80db6, c3b4e3d)
 * Make the name of the mod being uninstalled yellow (9a6346a)
 * Fix static notification queue object causing crashes when closing the game (#1340)
 * Fix compile issues with `gd::vector` on Android (#1351)
 * Use Clang 19 for windows builds (#1346)
 * Additional CCRenderTexture members (#1352)
 * Make property macros public (#1364)


## v4.4.0
 * Experimental support for iOS! (#1275, #1280, #1282, and a lot more other commits...)
 * Use MDPopup for mod setting descriptions (#1286)
 * Add a new filter that shows enabled mods before disabled ones on the install page (#1315)
 * Save filters from `InstalledModListSource` to disk (#1315)
 * Fix a vanilla crash that could happen with large save files on Android (#1270)
 * Fix a vanilla MacOS crash that could happen on game close (#1304, 7b877db, eb043db, 75c8efa, 8982011, 8bd735a)
 * Fix a rare crash that could happen when searching for mods (eb874ad)
 * Fix a small visual bug that happened when viewing mod problems (967b6ad)
 * Fix some file access related crashes that could happen when loading mods (27ca8b6)
 * Fix a small visual bug in `ModDeveloperList` (#1303, #1319)
 * Fix link buttons not being disabled when needed in `ModPopup` (#1303)
 * Print more useful information in Windows crashlogs when an access violation happens (c3d2b4e)
 * Fix small issues with installer spanish translation (#1254)
 * Optimize `ModsLayer` a lot (#1296, #1321, 427e0b0, eb874ad)
 * Remove Modtober tab (#1299)
 * Add `geode::dirs::getResourcesDir()` and `{gd_resources_dir}` setting variable (#1285)
 * Add `geode::globalListen()` and `EventListener` operator= move (#1218)
 * Add base64 utils (#1307)
 * Make `geode::utils::web::openLinkInBrowser` open the system webview on Android (#1295)
 * Add API to change log level per mod (#1305)
 * Add safe area API for Android and iOS node placement (#1284)
 * Add unicode conversion utils (#1310)
 * Add missing `SimpleAxisLayout` getters (#1314)
 * Add `format_as` for all SeedValue classes (#1316)
 * Add `LazySprite` class, which handles image loading in a separate thread as much as possible (#1320)
 * Add `gd::pair` for non-Android platforms (#1322)
 * Add optional saving for `geode::utils::game::exit` and `geode::utils::game::restart` (061c335)
 * Add members for `CCParticleSystem` (#1273)
 * Add `gnustl` vector and map, alongside other dependencies (#1255)
 * Make `gd::pair` constructible from `std::pair` (b444247)
 * Fix some `geode::stl` iterator conversions (e64ab20)
 * Expose `BreakLine` class (#1309)
 * Correctly check image size in MacOS crashlogs (#1274)
 * Fix a MacOS crash related to extra keys added by Geode (f7d44f3)
 * Fix memory leaks in `CCImage` usage (f38fe30)
 * Fix `CCSprite` members (#1308)
 * Update minimum CMake version to `3.21` (023274b)
 * Let our `libcurl` use the native system CA, alongside our bundle (#1301)
 * Fix `gnustl` set functions returning wrong type (ac1de8a)
 * Fix `SimpleAxisLayout` scaling not working properly in some cases (22f01bb)
 * Fix `SimpleAxisLayout` offset gaps in `BackToFront` disrection (f4deccd)
 * Fix `TextArea` not handling scale properly on touches (#1313)
 * Fix undefined behaviour in path handling on file utils
 * Rename `2.207` to `2.2070` in the timestamp map (79ba633)
 * Update `TulipHook` to 2.4.4

## v4.3.1
 * Fix `SimpleAxisLayout` not respecting `Layout::ignoreInvisibleChildren` (a16d230)
 * Fix MacOS arrow keys (#1267)
 * Fix ScrollLayer culling with scaled children (f03cee8)

## v4.3.0
 * Event export macro (#1243)
   * See [the docs](https://docs.geode-sdk.org/mods/dependencies#event-macro) for more info
 * Fix settings `enable-if` parsing (315bf46, 1542e29)
 * Add `SimpleAxisLayout` (#1223)
   * A simpler and faster alternative to `AxisLayout`. Some features may be missing
 * Add environment variable for forcing terminal colors (39b1bef)
 * Fix crashlog PDB search paths (#1222)
 * Add support for missing keys and mouse buttons (#1258)
 * Add utils::string::trim* overloads for specific charset (0d4dcb3, 17faf36)
 * Round number settings to 5 decimal places (c9dbc4b)
 * Allow number inputs to be invalid while typing and active (6c6215b)
 * Optimize m_fields access to perform no allocations (7548421)
 * Allow `Task<void>`, useful for coroutines (2bfff1a, 463ea22)
 * Add some coroutine utils (99cefab)
 * Fix culling on ScrollLayer (b136e3b, b733c36)
 * Fix `WeakRef` behavior with nullptr (9a8939f)
 * Remove handler from function if no hooks are active (dc14d4c)
 * Fix some bugs in `geode::utils::ranges` methods (#1236, #1239)
 * Add patch for `CCGLProgram::compileShader` on remaining platforms (#1241)
 * Update the pugixml headers to be v1.15 compatible (#1247)
 * Allow auto update across major versions if running in forward compat mode (4bb17a9)
 * Re-add console log level to android (b1dc29a)

## v4.2.0
 * Implement gd::set for android (#1197, #1207)
 * Log levels for console and file logging (#1208)
   * Now in the Geode settings you can set the minimum severity for logs
 * Add GameEvent::Loaded, to avoid hooking Menulayer (c04bcaa)
   * Check the header for more info
 * Add setColor shorthands to RGBAProtocol (961c7c8)
 * New dependency list syntax, also add per dependency settings (1161e18, 889fb0d, c1d3a53)
   * The dependencies can then read these settings from their dependants
 * Add auto log deletion after period of time (8f60c3c)
   * Period length can be changed in settings
 * Add `log::getCurrentLogPath()` (4c9f990)
 * Load changelog and mod about page from latest version on the index, instead of local (1d716fc)
 * Cascade color and opacity for BasedButtonSprite (17107a2)
 * Add Event v2 classes, wrapper around current event system (728df4e)
 * Serialize std::filesystem::path as UTF-8 for matjson (#1210)
 * Improve MDPopup (56a8ced)
 * Fix macOS file picker, add file ext filters (0b61b50, 241be1b, b8cff6d)
 * Add UUIDs for old GD versions on macOS (#1213)
 * Add CCFontSprite and CCSpriteExtra to cocos (#1194)
 * Reorganize buttons in Geode layer on 4:3 aspect ratio screens (54c93c4)
 * Patch CCGLProgram::compileShader to prevent closing the game on shader errors (0340f52)
 * Make all cocos members public (#1214)
 * Remove modtober animation (f1bf2b6)
 * Only show "Recommended by X mod" text on recommended dependencies, not suggested (5f64d77)
 * Fix SceneManager use-after-free (8707a1b)
 * Icon for pending mod updates is now persistent (0969ec3)
 * Cleanup stale files from old index version (4d6c173)
 * Rename Loader to Geode in LoadingLayer text (203c819)
 * Add `GEODE_BUNDLE_PDB` option in CMake to allow bundling PDB files with mods (#1134)

## v4.1.2
 * Prevent crashlog recursion on MacOS (a69c4c2)
 * Fix a grave misspelling error (#1203)
 * Fix a logic bug in SettingChangedFilter (3665da9)
 * Less strict sdk version checking for developers (100be49)

## v4.1.1
 * Check is mod is available for current version in ModPopup (fdf4348)
 * Revert earlier commit that broke incompatibilities (ded70bb)
 * Fix `fmt::join` on logs (22e4cbf, #979)
 * Allow any float or int type in `getSettingValue` (d08e141)
 * Fix cocos headers (9dd7128, 6c039c6)
 * Fix target Geode version comparison with tags (ec2e8d8)
 * Fix "GD 0.000" from being displayed in mod listing (fb85530)
 * Id overload for `openInfoPopup` now fetches the latest mod version (c6666a3, bda9444)

## v4.1.0
 * Add Modtober winner announcement (0aa2449)
 * Add `getHighestChildZ` crash fix (24189b1)
 * Add Greek and German to Windows installer (bff6324, f7559ec)
 * Add command line arg support for MacOS (6a62813)
 * Add ID for ModItem badge container (d7177c1)
 * Add `Stub` and `Replace` hook priorities (1f50390)
 * Allow passing pointers to `getChildByType`, `CCArrayExt`, `CCDictExt` (b956596)
 * Make `CCEGLView` members public (0d607bf)
 * Fix links not appearing on non-installed mods (#1184)
 * Fix trailing gap on `AxisLayout` when auto grow enabled (4b5d112)
 * Fix `null` not being convertible to `std::optional` in `JsonExpectedValue` (15a1b34)
 * Update Result, matjson, TulipHook (12069db)

## v4.0.1
 * Add cutoff constructor for CCRenderTexture (#1171)

## v4.0.0
 * Make chosen display type in mod list be saved between startups (07d92a3)
 * Fix `Task::all` not returning results in order (227adb0)
 * Fix bugs with changing display type (555ebe3, f90461f)

## v4.0.0-beta.2
 * Add grid view to mod list (7bcf50d, 1ff24f0)
 * Add safe mode tip to windows crashlog window (38f3385)
 * Disable enabled button on outdated mods (302eea1)
 * Add a button to copy list of mods to clipboard (#1039)
 * Fix VersionInfo toJson (f6c2322)
 * Add `GEODE_DESKTOP(...)` and `GEODE_MOBILE(...)` macros (d6f0c59)
 * Fix CCCallFuncExt (b9fb2f6)
 * Fix `utils::string::replaceIP` when filter is empty (4d5e465)
 * Fix more log nesting issues (2221095)
 * Fix new before/after priority system (17bf772)
 * Added European Portuguese translation (#1160)
 * Add missing CCHttpRequest methods and members (#1161)
 * Fix downloading many mods at once causing the UI to lag (c94a533)
 * Fix vv version (6e86b38)

## v4.0.0-beta.1
 * Button to manually install mods from files (e881dc5)
 * Add `ModRequestedAction::Update` (e881dc5)
 * Add `ModMetadata::checkGeodeVersion` and `ModMetadata::checkTargetVersions` (e881dc5)
 * Add `geode::createModLogo` for creating a logo from a `.geode` package (e881dc5)
 * Tags now use names provided by the server (893b03e)
 * Add web support for multiple request headers with same name (#1150)
 * Fix `Task::chain` using the wrong type in the impl (22a11b9)
 * Fix installing mods not checking the current version (#1148)
 * Fix searching for mods ignoring geode and gd version (#1153)
 * Fix crash when checking tags (01807fe)
 * Fix 'Outdated' label being visible while updating (6679a69)
 * Fix log nesting issue (0e8d4c6)
 * Remove forward compat message box as it confuses users (5592ef6)
 * Fix crash on opening mod changelogs (9834cb2)
 * Make `ColorPickPopup` pimpl (1a201e1)
 * Fix lag issue in `ColorPickPopup` (3081164)
 * Change return type of `ModSettingsManager::save` (da92090)
 * Fix every misspelling of successfully (#1151)
 * Allow building geode itself in debug mode (5645399)

## v4.0.0-alpha.1
 * Support for the 2.2074 update
 * Developers, see [this page for a migration guide](https://docs.geode-sdk.org/tutorials/migrate-v4)
 * Major API breaks:
   * Remove everything previously marked deprecated
   * `utils::MiniFunction` removed
   * Rewritten `geode::Result` class
   * Rewritten matjson library
   * Settings V2 completely removed, use V3 now
   * `JsonChecker` removed
 * Add new system for ordered hook priority, [see docs](https://docs.geode-sdk.org/tutorials/hookpriority) (673317d, 6db3084)
 * C++20 coroutine support for `geode::Task`, [see docs](https://docs.geode-sdk.org/tutorials/tasks#coroutines) (e61b2c0, ab196b9)
 * Add `Task::chain`, [see docs](https://docs.geode-sdk.org/tutorials/tasks#chaining-tasks) (3248831)
 * Single page local mods list (efb1fbf)
 * Split mod problems into load and outdated (12e8bbb, 09fa872, df2528c)
   * This means mods made for outdated gd or geode versions no longer count as actual errors, resulting in less clutter in the ui
 * Fix safe mode popup on windows showing up when not supposed to (038788b)
 * WebRequest::ignoreContentLength (#1126)
 * Lots of smaller fixes to the geode ui (c9afa75, f5f3365, 2d66279, 02845d9, 9b95301, 6d13f78, 123b3ab, 0b2fc66, f96ea5e, cad670f)
 * Fix CCArrayExt::pop_back() return type (#1130)
 * Add missing spanish translations to installer (#1145)
 * Add hashtag symbol to CommonFilter::Any (#1131)
 * Disable forward compat on android (c9e97af)

## v3.9.3
 * Add cutoff constructor for CCRenderTexture (#1171)
 * Add XInputSetState export in proxy loader, fixing certain steam emus (480b12)
 * Force update to 4.0.1 on forward compat (2ed1886)

## v3.9.2
 * Fix searching for mods returning unavailable mods (#1149)

## v3.9.1
 * Fix mod downloads not checking version (f575187)

## v3.9.0
 * Many changes to the settings ui (#1108)
   * Fuzzy search is now more reasonable
 * Add `CCNode::getChildByType` utility method, replacing the `getChildOfType` util (0089d13, c8aa2e3, 5f70080)
 * Add `geode::utils::string::caseInsensitiveCompare` (f3d38a7)
   * Now local mods are ordered correctly
 * Add `CCCallFuncExt` util, which accepts lambdas and such (d1053b1, 1d31576)
 * Add missing `m_nExtraKerning` to `CCLabelBMFont` (#1088)
 * Truncate numbers in `numToAbbreviatedString`, no longer rounding up (82e703b)
 * Fix string settings not having their character filters applied (6d0b583)
 * Error on `numFromString` when string has left over data (d4ca28c)
 * Change positioning of MenuLayer `top-right-menu` menu (6d2557b)
 * Add `SceneManager::getPersistedNodes` (5009caa)
 * Fix android `std::unordered_map` copy constructor (67f59e7)
 * Fix crash on Linux when piping output to terminal (8ecb1c5)
 * Fix crash when geode menu is disabled (b69f810)
 * Fix infinite recursion in `WeakRef` hashing (d68e358)

## v3.8.1
 * Fix CCLightning header
 * Fix server query default value (8be97b7)
 * Fix importance resolving in disabled mods (d40ba6d)

## v3.8.0
 * Add Modtober integration! For more about Modtober, join [the GDP Discord server](https://discord.gg/gd-programming-646101505417674758) (964624b)
 * Add `Popup::CloseEvent` (6270e1c)
 * Add `openSettingsPopup` overload that returns the created `Popup` (dc8d271)
 * Fix `CCNode::querySelector` logspamming (b53759f)
 * Fix `followThunkFunction` following through into hook handlers (ad26357)

## v3.7.1
 * Fix crash with saving older custom settings (c3e7f23)
 * Add missing CCDrawNode::drawRect overload (145adb2)

## v3.7.0
 * Add persistent directory for mods to save data that won't be deleted when the mod/Geode is uninstalled (68ab475)
 * Fix touch priority issues in the settings popup (b3d5474)
 * Fix settings with custom types not being saved & loaded properly (f0b6a70)

## v3.6.1
 * Fix modrm rip relative instruction displacement saving the clobbered register (ddfe8e2)
 * Fix `DataSaved` and `DataLoaded` events (8615bbd, 43adb35)
 * Fix file settings mixed separators (1281c76)

## v3.6.0
 * Major rework of the entire settings system with lots of new features; see the [docs page](https://docs.geode-sdk.org/mods/settings) for more
 * Rework JSON validation; now uses the `JsonExpectedValue` class with the `checkJson` helper (89d1a51)
 * Add `Task::cancelled` for creating immediately cancelled Tasks (1a82d12)
 * Add function type utilities in `utils/function.hpp` (659c168)
 * Add `typeinfo_pointer_cast` for casting `std::shared_ptr`s (28cc6fd)
 * Add `GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH` (1032d9a)
 * Add `PlatformID::getCovered` (d5718be)
 * Add `limitNodeWidth` and `limitNodeHeight` utils (e8751bf)
 * Add `std::hash<WeakRef>` (2d9ce8f)
 * Allow limiting image width/height in `MDTextArea` (e8751bf)
 * The current working directory is automatically always set to the game directory (7c558ee)
 * Rename `toByteArray` to `toBytes` (6eb0797)
 * Improve `AxisLayout::getSizeHint` (85e7b5e)
 * Fix issues with file dialogs on Windows (62b6241, 971e3fb)
 * Fix PCH issues (aedd986)
 * Mod incompatibilities may now be platform-specific (9f1c70a)
 * Show appropriate popup for unavailable mods (6944f80, 30dc70c)
 * Add support for description as a default value on file settings (75186f6)
 * Remove early ub related to console (54ff48c)
 * Temporary workaround for Wine crash due to console (dc170d7)
 * Check also for incompatibilities with newly installed mods when downloading (9115091)
 * Make cocos geometry classes constexpr (db8a6c8)
 * Fix rip displacement issues when hooking some functions on Windows (175dc27)

## v3.5.0
 * Move CCLightning to cocos headers (#1036)
 * Add new `gd::string` constructor (bae22b4)
 * Use `getChildren` instead of member in `getChildByID` (fe730ed)
 * Fix sprite order in `CCMenuItemExt::createToggler` (d729a12, 59a0ade)
 * Add restart button to windows's crashlog window (#1025)
 * Update FMOD headers (63b82f9)
 * Change SwelvyBG sprites to be 2048x512 (#1029)
 * Fix missing `GEODE_DLL` (e4054d4)
 * Add code of conduct (80693c1, ab8ace0, ca3a2a3)
 * Add ID system to Geode's web requests (#1040, 1f2aa2c, 1b5ae86)
 * Add `Notification::cancel` (cd5a66c)
 * Update matjson (e5dd2c9)
 * Update TulipHook (a31c68f)
 * Fix a bug where only 1 word wrap variant can exist (#1058)
 * Fix ScrollLayer when anchor point is not ignored (d95a43b)
 * Move macOS builds to using apple clang, fixing issues on older macOS versions (#1030)
 * Allow dashes when searching for developers (#1023)
 * Split update checks into multiple batches (#1066)
 * Show invalid mods on mod search (#1065)

## v3.4.0
 * Add an API for modifying the Geode UI via events; see [the corresponding docs page](https://docs.geode-sdk.org/tutorials/modify-geode) (2a3c35f)
 * Add `openInfoPopup` overload that accepts a mod ID and can open both an installed mod page or a server page (028bbf9)
 * Add `LoadingSpinner` for creating loading circles easily (5c84012)
 * Add `TextInput::focus` and `TextInput::unfocus` (749fdf1)
 * MDTextArea changes: hex colors are now formatted as `<c-XXXXXX></c>`; added support for `<cc>`, `<cd>`, `<cf>`, and `<cs>`; fixed `mod:` links (028bbf9)
 * Deprecate `cc3x` (6080fdb)
 * Don't cancel subtasks on `Task` destructor (4b4bc0e)

## v3.3.1
 * Move ObjectDecoder and its delegate to Cocos headers (95f9eeb, dceb91e)
 * Fix weird behavior with textures, objects and more by changing en-US.utf8 locale to C (2cd1a9e)
 * Change all C number parsing to use `numFromString` to further remove the risk of this happening again (006ede8)

## v3.3.0
 * Update network libraries, fixing problems with old Wine versions (e26fe95)
 * Fix scale on mod logos for low quality (ba7b0fa)
 * Fix platform-specific dependencies by using CLI 3.2.0 (98f82ff)
 * Add `GEODE_DONT_USE_CCACHE` CMake option (2edfb71)
 * Remove now useless `std::hash` impl for `std::filesystem::path` on android (2b35e0e)
 * Implement custom window for showing crashlog on windows (4f32197)
 * Update docs version to show v3.0.0 (f86d4db)
 * Check hash for downloaded mods (61e000d)
 * Update chinese installer translation (#995, #997)
 * Update polish installer translation (#998)

## v3.2.0
 * Fix auto-updater on MacOS (d752bc2)
 * Use tasks for `FileSettingNode` (f94e95e)
 * Fix single argument overload of `Task` (6fe1ac9)
 * Fix the GLFW message box fix (09c188a)
 * Shrink `TextInput` input to give some padding (1da73cf)
 * Undither account and editor blank sprites, add missing editor blank sprites (427e86e, efc4a00, 9fd9a78)
 * Fix populating web headers and add some new getters (a96ec91)
 * Build mods to load stack statically (255066a)
 * Force internal mod to always appear enabled (e659b97)
 * Bring back uninstall Geode button on Windows (22b2580)
 * Add `geode::openChangelogPopup` (e432e72)
 * Add special visuals for paid tag (0082765)
 * Add 64-bit check to the Windows installer (c45d8f6)
 * Add `Mod::checkUpdates` (9d02155)
 * Error on attempting to hook missing or inlined functions (2dc989f)
 * Implement function bound checking on Windows crashlog for symbol resolution (66c2f9a)
 * Add new syntax for image scale arguments (#983)

## v3.1.1
 * Update Windows installer translations (ae589d2, dca28db, d12fb37, 08d8af3, f52cf02, 3fa1d9b)
 * Add safe mode by holding shift on MacOS (e4905a0)
 * Fix bug on android where `FileOperation::getFilePath` was not actually hooked (9885212)
   * This was a problem with our bindings, there the return type was TodoReturn instead of gd::string causing the hook to silently fail, leaving only the launcher hook (which is prone to failing) as a fallback.

## v3.1.0
 * Implement LoaderImpl::getGameVersion on android, requires 1.4.0 launcher (00799b6)
 * Better compression for windows installer, saves 6mb (54da3b0)
 * Fix mod install confirmation popup not properly showing mods (5886324)
 * Fix missing version check for incompatibilities (83bb3c2)
 * Update TulipHook (f6260a5)
   * This fixes a crash with CustomKeybinds
 * Add more options to `web::WebRequest` (83f8a32) (#943)
 * Prioritize mod updates over failed mods on the main menu icon (89ed81a)
 * Windows installer improvements:
   * Delete old geode/update folder to prevent downgrading (116af49)
   * Delete left over dlls from 2.204 (d1e2919)
   * Installer can now forcibly install over existing mod loaders (736a61e)
   * Make installer delete msvcp dlls in gd folder (d9c7610, 9c6841e)
 * Improve CI by cross compiling from linux (#935, #939)
 * Alternate way of getting main function address on Windows (1384604)
   * Previously this would affect about 2 people on earth
 * Show special error on proxy loader if bad dlls are in gd folder (fd476fe)

## v3.0.0
 * Show loader commit hashes again (b72bb9e)
 * Don't save data if mod is uninstalled with save (518f941)
 * Add "open in browser" button to mods (581e2d3)
 * Remove the unimplemented Versions tab temporarily (581e2d3, 1f5ebf9)
 * Texture fix (#941)
 * Implement Android 64 bit exception fix in client side (cfb682e)
   * Most of the fix is in the launcher itself, so you need to update to 1.4.0 for the full fix

## v3.0.0-beta.5
 * Add More Details button for mod problems (9095005)
 * Display mods not on current platform (2cd0990)
 * Fix `DefaultEventListenerPool` missing constructor ABI break for Dispatch (d88a93e)
 * Implement `one-of` setting without ABI break (7742b57)
 * Remove the missing custom setting warning (4509607)

## v3.0.0-beta.4
 * Fix events crash caused by immediate enabling & disabling (e796711)
 * Make some classes final (ea96e2c)

## v3.0.0-beta.3
 * Fix a crash that might happen when entering the Geode mods page (79689cd)
 * Fix a locale related crash (fdb473a)
 * Cleanup old field syntax remnants (34e51ff)
 * Use `clamp` for `SwelvyBG` instead of `repeat` (0a6a5e6)
 * Add support for `one-of` settings for mods (3f674e5, 7d79ddd)
 * Remove `RT_ADD` and `RT_REMOVE` (7bd8a1a)
 * Add `CCDirector::m_bFastMenu` back (c3058ff)
 * Fix UI visual bugs in `ModsLayer` (e273ef2, b3925f1, 255a42c)
 * Fix race conditions for event pools (636be07, 4c15bdb, abc34f9)
 * Add a `static_assert` to prevent old field syntax (5f37d30)
 * Fix dangling `string_view` for `Task` names (92704b9)
 * Download the exact update suggested by Index instead of latest version (115f0e7)
 * Small cleanup and improvements for Windows stacktraces (2824c17)
 * Fix bugs related to `delayload` (ec1d846)

## v3.0.0-beta.2
 * (WebRequest) Don't change the method from POST to GET on redirect follow (6ae11dd)
 * Make `file::openFolder` actually work on selecting paths on Windows (0309e01)
 * Hide platform console option on Android (df3d147)
 * Add `gd::vector::erase` for Android (c3c2afa) - thanks SpaghettDev
 * Stop using `android_stl=c++_shared` on mods (100dbdc)
 * Show confirm dialog directly when updating mods (8f1a9cf)
 * Only check for updates once per launch (3313a44)
 * Fix various bugs with texture pack reloading (aeaf7f7)
 * Fix `Ref` and `WeakRef` move assignment (f352503)
 * Add comma to download counts (de50b98)
 * Add check for Texture Loader's fallback function for mod logos (a421047)
 * Bring back safe mode UI (6e6dace)
 * Fix texture corruption bug on `ModsLayer` (76e2ed4)
 * Fix `CCArrayExt::value_type` (97d2e44)
 * Add `AxisAlignment::Between`, for equal space between elements (adc4469)
 * Add discord and website links to loader mod.json (8792747) - thanks coopeeo
 * Make mod updates log into an info log instead of error (815c4a3)
 * Hopefully finally fix Windows crashlogs (70fe096)

## v3.0.0-beta.1
 * Add a special error for 1114 - vcredist update (d0821f5)
 * Properly fix Windows exceptions (84a2c6b)
 * Add keyboard support to ModsLayer (2b53e8a)
 * Fix "Loading mods cancelled" appearing with no reason (698112a)
 * Fix various compile warnings (68ac2b0, cb1e7de)
 * Use `string_view` instead of `string` for some Task functions (da617ee)
 * Add `Task::listen` for listening to Tasks globally (fa7a2de, eb7c4d9)
 * Add 2.2 `CCDrawNode` members (562fbf6) - thanks Prevter
 * Use `SPACE_WRAP` for mod problem textarea (4d267d3)
 * Fix download links for mods being generated without prerelease tag (06bd2e2)
 * Make developer filter search by username (014ec68, 1c40a63)
 * Fix `Notification` crashing when shown during a transition fade (0270d47)
 * Split `GEODE_PLATFORM_NAME` for Mac ARM and Mac Intel (48a7981)
 * Use `string_view` instead of `const char*` for `expandSpriteName` (22cc33b)
 * Make `_spr` compile time (b22a59d, bacab92)
 * Update server UserAgent format (9679b40)
 * Allow mods with old `gd` key syntax in `mod.json` to be checked for updates (c1fbc08)
 * Replace the "Recommended" tab with "Featured" (#897)
 * Use c++_shared STL on Android64 (115b9cf, d0cc62a, fbc6416)
 * `file::pick` and `file::pickMany` are the new file APIs, which use Tasks (#899)

## v3.0.0-alpha.2
 * Add `WebResponse::into()` for writing responses to files (f909a73)
 * Add `geodeImplicitEntry` and `geodeCustomEntry` (6b2ac24, 5969c90)
 * Fix padding and add a custom color for borders (#868)
 * Add more SMJS nodes to UI include (#869)
 * Fix an issue with CCParticleQuad (330c20e, #865)
 * Hopefully fix Windows crashlogs
 * Fix supersede incompatiblity IDs being validated (754ae3c)
 * Fix dates not respecting timezones (1c36854)
 * Change dependency / incompatibility / settings 'platform' string values (80d95cf)
 * Make tasks cancel when the handle is destroyed (c82112f)
 * Increase delay when restarting GD (84c0ba5)
 * trigger $on_mod(Loaded) for Loader
 * Make Windows installer use x64 vc_redist (6793fbd)
 * Fix some cocos members (#872, #881) - thanks Acaruso and SpaghettDev
 * geode::ui::Border fixes (#883) - thanks SMJS
 * Update the developer popup, and various other UI fixes (#877) - thanks Alphalaneous
 * Check if is_json is defined for getSavedValue custom types

## v3.0.0-alpha.1
 * Deprecated the old web API, replacing it with a new one (b129808)
 * Replace `ghc::filesystem` with `std::filesystem` (#713)
 * Make `queueInMainThread` take a move only function
 * Remove "gd" shorthand from mod.json (#471)
 * Remove old fields syntax (#715)
 * Make `TodoReturn` unusable (#714)
 * Remove unused wstring utilities
 * Add support for Geometry Dash 2.206
 * New UI for the Index, which also uses a server implementation (#833)
   * Implements new popups `ConfirmUninstallPopup`, `DevPopup`, `FiltersPopup`, `ModErrorPopup`, `ModPopup`, `SortPopup`
     * Mod popup now has new features like tags, reverting accidental disabling, version (1876af8, dac16a4, f365dc4)
   * Adds 4 tabs to the Geode menu: Installed, Recommended, Download, Recent
   * Creates a fancy new background with `SwelvyBG`
   * The entire color scheme has been changed to fit Geode's color language of pastel purple-pastel gold gradient
 * Remove old web utilities (b129808)
 * Implement Windows 64-bit and MacOS M1 arm64 support
 * Add color support to `ListView` (#854, #859)
 * Use `std::string` for `gd::string` on Windows (144b2d7)
 * Make `queueInMainThread` take in a movable callback (0c35a92)
 * Add `CCMenuItemExt` for lambda versions of CCMenuItem classes (de73317)
 * Add `scrollToTop` for ScrollLayer (7071bb1)
 * Add a new `Task` class for generic asynchronous task implementations
   * Documentation can be found [here](https://docs.geode-sdk.org/tutorials/tasks/)
   * Add a special task named `WebTask` for handling web requests
 * Fix `Unzip` crash on missing progress callback (1145426)
 * Fix `AnchorLayout` not ignoring `ignoreAnchorPointForPosition` (547c047)
 * Add `ObjWrapper` class for wrapping objects inside a `CCObject` (5e76da1)
 * Adjust fuzzy searching (cc5cb07)
 * Add `TextInput::setLabel` (991fce0)

## v2.0.0-beta.27
 * Implement some `BasedButtonSprite` fixes (edb8e6c)
 * Add early version check for MacOS (4083950)
 * Move `Enums.hpp` to bindings (23e04a0)
 * Add a query selector for query based child selection (c75ec63)
 * Add `getDisplayFactor` utility for MacOS Retina (d725126)
 * Add `Result::unwrapOrDefault` (1dc9ec4)
 * Fix texture pack removal (006413a)

## v2.0.0-beta.26
 * Bring in several UI helpers from the `new-index-but-better` branch: `ListBorders`, `addSideArt`, `AxisLayout` improvements, ... (26729c3, 7ff257c)
 * Make it possible to compile mods in Debug mode (517ad45)
 * Add `GJDifficultyName` and `GJFeatureState` (#706)
 * Add `geode::cocos::isSpriteName` and `geode::cocos::getChildBySpriteName` (#725)
 * Add some Android keycodes (4fa8098)
 * Update FMOD on Mac (c4a682b)
 * Bump JSON version (5cc23e7)
 * Fixes to `InputNode` touches (29b4732)
 * Fix `file::readFromJson` (77e0f2e)
 * Fix issues with TulipHook (f2ce7d0)

## v2.0.0-beta.25
 * Fix updater sometimes skipping releases (18dd0b7)
 * Fix resources getting downloaded every time (5f571d9)
 * Modify fields are now done using an explicit `Fields` struct to avoid forgetting `m_fields` (4505b0d)
 * Fix memory leak on new field containers (db32732)

## v2.0.0-beta.24
 * CMake error when compiling with Android SDK below 23 (ea34e12)
 * Fix rendering of soft line breaks in MDTextArea (c7f8b5f)
 * Fix `getChildOfType` entering an infinite loop with negative indices (a795487)
 * Don't cache fonts on Windows (e9da55f)
 * Add `level:` and `mod:` to MDTextArea (8c61a6f)
 * Fix json floating precision errors (318a7f2)
 * Fix loading bar going out of bounds (e50c3ab)
 * Fix field containers for modified classes that have an inheritance relationship sharing the container (00e971a)
 * Error if mod target geode version doesn't match installed SDK version (2c559f8)
 * Fix dangling pointer in `disownPatch` (4c492c1)
 * Add a way to access internal setting container (798cacc)
 * Add the crashed thread to Windows crashlog (f84e866)
 * Add Greek, Ukranian, Simplified and Traditional Chinese, French translations for the installer (8002ca0, bd5db26, 33fcd52, c02cc6d)

## v2.0.0-beta.23
 * Fix `CCMenuItem::activate` crashes (005d245, 7f9dcc6)
 * Fix ProfilePage fix misaligning profiles (b44184c)
 * Fix Impostor PlayLayer Fix being enabled in forward compatibility mode (96fee44)
 * Fix IPC using an incorrect mod ID (c02dc7d)
 * Fix opening folders not working with certain mods installed (43cf9fa)
 * Update Android `gd::map` (d165616, e4ab881, ea6ac9f)
 * Change outdated GD message on Windows (8ab3f56)
 * Enable FMOD initialization check on all platforms (682144f, 1a67f9b)
 * Re-implement `gd::unordered_map` and `gd::unordered_set` on Windows (#636, #644, 912aa23)
 * Fix handleTouchPriority not retaining nodes and sometimes crashing (2d13d4f, c8d1e88)
 * Fix restarting the game if executable filename includes spaces (223f168)
 * Implement "Report Issue" button (#634)
 * Fix a thread safety issue in logging (7155705)
 * Operation Big Sister - disallow modifying TodoReturn functions (f3267b0, 55e1f6a, dd6e20b)
 * Tuliphook: Fix parameters bigger than 4 bytes being passed through registers (f7bda30)
 * Tuliphook: Double the size of the handler and the trampoline (1a67f9b)

## v2.0.0-beta.22
 * Add `Patch::updateBytes` to update a patch (ba648340)
 * (Possibly) fix random curl crashes (dd440433, 1fb12f2d, 6cd6e4d0, 8998041e, 2be58549)
   * This was done by using a build with the thread resolver enabled, possibly fixing race conditions when initing curl
 * Prevent `GeodeUpdater.exe` from hanging (d139049b)
 * Fix a duplicated node ID in LoadingLayer (#574)
 * Fix minor memory leak in AsyncWebRequest (52ea6ea5)
 * Fix Wine crashing when launching via terminal (#501)
 * Use C locale instead of US (#566)
 * Impostor playlayer fix (#562)

## v2.0.0-beta.21
 * Fix `numFromString` for floating point numbers (6d91804)
 * Fix `ScrollLayer` cropping (5c8ee29)
 * Show featured mods on top (4e06c20)
 * Don't require admin for installer (5f8dc3a)
 * Add `char const*` and `std::string` overloads for `gd::string` (982e8ab, a19d26d)
 * Remove dithering from some textures (b9a76b3, 76a615c)
 * Replace node attributes with id based user objects (363a028)
 * Add FMOD initialization check for Android (0623563)
 * Remove deprecated values from `Permissions` (b082dd1)
 * Add a progress percentage to index unzipping (baf3a6b)
 * Fix `pushNest/popNest` not doing correct indent value (c7a1f76)
 * Optimize `DispatchEvent` to use their own pools, greatly increasing performance (d8ac85b)
 * Check for presence of children in `handleTouchPriority` (28bd757)
 * Fix floating point returns for optcall/membercall functions (dba5b01)

## v2.0.0-beta.20
 * Enable PCH on Mac for better compile times (dd62eac)
 * Add `numFromString` utility for safely parsing numbers (c4e9c17)
 * Add `CCNode::setContentWidth` along with respective height setter and getters (e06b907)
 * Add `getChildBySpriteFrameName` (85f8a20)
 * Add `isSpriteFrameName` (eea3556)
 * Add new more refined `TextInput` class, deprecating `InputNode` (28f393b)
 * Fix `InputNode` and `TextInput` using tags on their internal input node for controlling behaviour (29f99c2)
 * Remove `strfmt` and `cstrfmt` utilties for being outdated and unsafe (b69ac71)
 * Make `clamp` utility use template magic for better type inference (4ba0b7d)
 * Improve user errors (4b667cc)
 * Deprecate invalidally formatted mod IDs (e80d228)
 * Add new `superseded` importance level to incompatabilities (e80d228)
 * Fix `pickFile` on Android (9051779)

## v2.0.0-beta.19
 * Fix Windows forward compatibility mode (eef949c5, 824efbf3, 456075a2)
   * This was caused by Clang not setting `/DELAYLOAD` properly, but also a mutex introduced in beta.7 causing Geode not to load at all
 * Slightly improve Mod List sorting (3497692)

## v2.0.0-beta.18
 * Use sccache for caching instead of PCH to improve compile times (#493)
 * Add translations for Japanese in installer (#504)
 * Add `geode::openSupportPopup` for opening the Support page for a mod (438252f)
 * Add `<ca>` to MDTextArea (ca683fa)
 * Add more `GameObjectType`s (8e4b76f)
 * Add `Mod::hasAvailableUpdate` (b71ae17)
 * Add callbacks for Android keyboard inputs (#507)
 * Add option to activate Safe Mode on Windows by holding Shift when launching (a327f72)
 * Move loader commit hash information to the Info popup (94ab199)
 * Make `is_json` functions more accurate (803df4f)
 * Fix color parsing from hex (131539f)
 * Fix `JsonValidation` leaking exceptions in `into` (83847e3)
 * Fix struct returns for `CCPoint` not actually working properly (1daa671)
 * Fix `geode::web::fetchJSON` and `ColorPickPopup` not being DLL-exported (2c1eb67, 8250a73)
 * Fix usernames not showing up on profile links in Markdown (3fe102d)
 * Fix `MDPopup` height estimations (cc4c32e)
 * Various other small fixes & improvements

## v2.0.0-beta.17
 * Add `$override` macro for syntactic sugar (e7a1913)
 * Add support for platform-specific setting default values as well as platform-specific settings (9c8fcf1)
 * Change `cc3bFromHexString` and `cc4bFromHexString` to be more strict with parsing by default (9c8fcf1)
 * `JsonMaybeValue::is` is now a lot more reasonable (9c8fcf1)
 * Make `LayoutOptions` also `CCObject` (3b7621c)
 * Fix RobTop's socials in MenuLayer (cee8c74)
 * Fix issues in CMake (d574248, 4ddd92d)
 * Fix input nodes being clickable when invisible (bf32946)
 * Add `gd::vector::push_back` and `gd::vector::pop_back` (1593564)

## v2.0.0-beta.16
 * Bump minimum CMake version to 3.25
 * Add support for platform-specific dependencies (30c7d3f)
 * Adjust content size for the bottom menu in MenuLayer to prevent overlaps

## v2.0.0-beta.15
 * Add `!` for problem mods to crashlog (5c45725)
 * Log crashlog path when saved (f832cc5)
 * Change log filenames (65907ca)
 * Fix `o` not appearing in crashlog (e95d454)
 * Fix Scrollbar touch (34e80c1)
 * Fix infinite loop in nested dependency chain (fda3790)
 * Fix touch priority messing up after mod download (ba0e13f)
 * Improve safe mode, listing mods as normal (241ddc3)
 * Fix `SimpleTextArea` text alignment and size (9d92a7c)
 * Install resources on local Geode build (e9aa889)
 * Update json library for better errors (0731f44)
 * Run binding codegen in CMake configure time (2e20ccc)
 * Replace media perms with file perms (5f7af1a)
 * Fix `FloatSettingNode` not accepting decimal point (c83858d)
 * Throw on invalid json output (e2150cc)
 * Fix icon positioning in `InstallListCell` (cb7c024)

## v2.0.0-beta.14
 * Use Breakpad for crash reports on Android ([#481](https://github.com/geode-sdk/geode/pull/481))
 * Fix available label on LocalModInfoPopup (b4037093)
 * Remove trailing CR from headers (17153a4f)
   * Ends up **fixing auto update**
 * Implement getGameVersion on mac (86ae005f)
 * Fix mysterious AsyncWebRequest crash (c0352782)
 * Show message for old android launcher users (c00ccd35)
 * Add install target for loader itself (36727ced)

## v2.0.0-beta.13
 * Add new `AnchorLayout` feature ([#476](https://github.com/geode-sdk/geode/pull/476))
 * Add support for multiple developers in `mod.json` through the `developers` key (85180d5f, f21542a8)
   * requires CLI v2.9.0 and VS Code extension v1.7.0
 * No longer consider outdated mods invalid .geode files altogether (d9c65b37)

## v2.0.0-beta.12
 * Increase target MacOS version to 10.15 (6dc6e9b4)
   * Not even steam supports 10.13 anymore, and with this change
   we can finally stop worrying about std::filesystem
 * Add safety hook for save path on android (0188eee5)
   * We had a bug where the game would randomly reset your save file
   on android, and we believe this to be a hook by the launcher itself
   not being placed properly. So, we also do the hook on Geode itself,
   for safety.
 * Fix rob's broken MessageBoxW on Windows (5d631921)
 * Add utils for setting thread name, show it on log (832bcf81, ce53fb31)
 * Add some launch arguments for geode (7ccaef90)
 * Deprecate blocking file picking utils (ee97e2da)
 * Sort mods by id in crashlog (984d1482)

## v2.0.0-beta.11
 * Fix `InputNode` funkiness on Windows (bb1fcbe)
 * Add methods for requesting and querying permissions (9ff9191)

## v2.0.0-beta.10
 * Make index loading when not downloading async (e81b5e9)
   * (I can't wait to get rid of the old index)
 * Log nesting and alignment improvements (f7980d3, 4693eb1)
 * Fix `Patch` intersection code (45ce360)
 * Fix `InputNode` touch logic (c551d43)
 * Force Windows errors to be in English (6ba656c)
 * Add sprite scaling for `MDTextArea` (5802b78)

## v2.0.0-beta.9
 * Make the index population async as well (3b3e174)
 * Run Android file callbacks on main thread (79d9184)
 * Implement launch arguments support (a2b164a)

## v2.0.0-beta.8
 * Fix TulipHook arm32 relocation of conditional branch - blame Dobby (69b9b2d)
 * Remove try-catch blocks, replacing them with other means of handling (065d0c4)
 * Revert touch priority hook, instead make it an opt-in behavior for nodes (7370761)
   * This should fix every touch priority issue that happened before
 * Implement conditional requests to GitHub API (df07409)
 * Add response headers to `SentAsyncWebRequest` (df07409)
 * Move deletion of old index folder into separate thread (6835f57)
 * Fix MacOS installer erroring after installing anyway (09ebcf9)
 * Fix Windows `openFolder` opening parent folder (9e973a9)
 * Reenable last crashed popup and index update on MenuLayer (16b9689)

## v2.0.0-beta.7
 * MacOS Support! (9869ddc)
   * Huge thanks to @dankmeme01 for almost single-handedly making this work!
   * Note most mods do not support MacOS at the moment.
 * Fix exceptions on android32 (9c87d4c)
 * Move GD version check to loadBinary (d2a51354, 82808c3a)
 * Show updates for mods that are disabled (bd9d0635)
 * Force geode internal mod to be first on list (c2d1e587)
 * Fix geode layer eating inputs if search is selected (18983974)
 * Move clipping rect fix, fixes editor particle editor (eeeedbb9)
 * Fix force priority fix, fixes editor scale warp lock button (fd8c3630, f1a32b6a)
 * Clearer error message when platform binary is missing from mod (dc8cd219)
 * Better error message on windows dll load failure (77736d3)
 * Make Logger::push thread-safe (d9358d8)

## v2.0.0-beta.6
 * Fix unzipping skipping first entry (2bc0d1a)
 * Revert the input box fix again (4ab7197)

## v2.0.0-beta.5
 * Android 2.205 support, both 32 and 64 bit!
 * Fix Android 32 bit TulipHook relocation of branching (98e1519)
 * Fix logcat adding all crashlogs if it can't strip (d7ed016)
 * Readd the text input box fix from 2.1 (316e652)
 * Fix resource hash mismatches for good (4916669)

## v2.0.0-beta.4
 * Show which DLL is needed to remove on Windows installer (d323f0e)
 * Optimize unzipping of index (b22ed7d)
 * Force all children of Layout to ignore anchor point (c42b5dd)
 * Fix squishing of layouts (b61d466)
 * Fix `SearchFilterPopup` ui (5bacaf3)

## v2.0.0-beta.3
 * Avoid virtual functions in forward compat code (dc2fba2)
 * Fix updating index message (b63611e)
 * Use new launcher function for unzipped mod dir (120fa4a)

## v2.0.0-beta.2
 * Fix `geode::utils::cocos::reloadTextures` on windows

## v2.0.0-beta.1
 * 2.2 support!
   * Android arm64-v8 support!
   * Future version compatibility mode, where the Geode UI is disabled
 * Split bindings into their own repo (3aa3ae7d)
 * Add `gd` as a required key in mod.json, to specify target GD version (f7a3947c)
 * Load native binaries from mod resources (acd9bd78)
 * Refactor json library, namespace and headers renamed to `matjson` (b9a09f73)
 * Refactor `AsyncWebRequest` class (d56c70fe)
   * `postFields` renamed to `body` and `bodyRaw`
   * `customRequest` renamed to `method`
 * Change api for `CCDictionaryExt`, now 2nd template arg must be a pointer, making it more similar to std::map (e1b7b1f4)
 * Refactor Hook and Patch classes, Mod::addHook renamed to Mod::hook (a5a3a08d)
 * Remove many deprecated keys from `mod.json`, most importantly `required` in dependencies (39d372c7, b4235bb5)
 * Refactor logging, now pipes directly to fmtlib (2ba8761f)
 * Refactor gd::string (24bbc65f)
 * Refactor many other parts of the codebase

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
