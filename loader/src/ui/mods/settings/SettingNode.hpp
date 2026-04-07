#pragma once
#include "SettingNodeV3.hpp"

using namespace geode::prelude;

using TitleSettingNode = TitleSettingNodeV3;
using BoolSettingNode = BoolSettingNodeV3;
template <class S>
using NumberSettingNode = NumberSettingNodeV3<S>;
using IntSettingNode = IntSettingNodeV3;
using FloatSettingNode = FloatSettingNodeV3;
using StringSettingNode = StringSettingNodeV3;
using FileSettingNode = FileSettingNodeV3;
using Color3BSettingNode = Color3BSettingNodeV3;
using Color4BSettingNode = Color4BSettingNodeV3;
using KeybindSettingNode = KeybindSettingNodeV3;
using UnresolvedCustomSettingNode = UnresolvedCustomSettingNodeV3;