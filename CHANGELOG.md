# Geode Changelog

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

