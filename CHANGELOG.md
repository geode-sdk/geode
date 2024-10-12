# Geode Changelog

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
