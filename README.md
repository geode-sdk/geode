![Geode Logo](https://github.com/geode-sdk.png?size=80) 

# Geode SDK

**Geode** is a [Geometry Dash](https://store.steampowered.com/app/322170/Geometry_Dash/) **mod loader** and **modding SDK** with a modern approach towards mod development. Unlike previous mod loaders, which merely inject the DLLs and let devs handle the rest, Geode aims to be a more comprehensive project, which manages loaded mods & hooks itself. Geode has been built to ensure performance, compatibility, portability and ease of use. For devs, Geode means **easy development and portability**; for end users, **a uniform and easy experience** using mods.

## Why Geode?

There's nothing worse than having to read thousands of words just to see what some library's code actually looks like, so instead, here's a **Hello World** in Geode right off the bat:

```cpp
#include <Geode/bindings.hpp>
#include <Geode/modify/MenuLayer.hpp>

USE_GEODE_NAMESPACE();

class $modify(MenuLayer) {
	void onMoreGames(CCObject*) {
		FLAlertLayer::create(
			"Geode",
			"Hello World from my Custom Mod!",
			"OK"
		)->show();
	}
};
```

This code hooks the "More Games" button in Geometry Dash, and makes it show a different popup when clicked. Compared to traditional modding methods such as MinHook, we would argue that this is **much simpler to write, understand & maintain**.

One of our main design goals with Geode has been to make a framework so good that **after using it once, you never want to go back**.

> :warning: It is important to note that Geode is **not meant to make learning to mod easier**. To make a good GD mod, you will still need to learn the ins and outs of C++, reverse engineering, byte patching, and everything else. Geode is merely a collection of tools that make modding more enjoyable for people who are already familiar with it; not a magic wand for making mods without the hours of experience and practice required.

## Documentation

Detailed documentation, tutorials, and references on using the Geode SDK can be found [here](https://geode-sdk.github.io/docs/).

## Contribution

You can contribute to Geode by opening a [Pull Request](https://github.com/geode-sdk/geode/pulls)!

## Questions, help, etc.

If you have any further questions, need help, or just want to share your love for catgirls, be sure to join [our Discord server](https://discord.gg/9e43WMKzhp)!
