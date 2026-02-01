#pragma once

#include "SettingV3.hpp"

namespace geode {
    using Setting = SettingV3;
    using SettingGenerator = SettingGeneratorV3;
    using SettingGeneratorRef = SettingGeneratorV3Ref;

    template <class T, class V = T>
    using SettingBaseValue = SettingBaseValueV3<T, V>;

    using TitleSetting = TitleSettingV3;
    using BoolSetting = BoolSettingV3;
    using IntSetting = IntSettingV3;
    using FloatSetting = FloatSettingV3;
    using StringSetting = StringSettingV3;
    using FileSetting = FileSettingV3;
    using Color3BSetting = Color3BSettingV3;
    using Color4BSetting = Color4BSettingV3;

    using SettingNode = SettingNodeV3;
    template <class S>
    using SettingValueNode = SettingValueNodeV3<S>;

    using SettingChangedEvent = SettingChangedEventV3;
    using SettingNodeSizeChangeEvent = SettingNodeSizeChangeEventV3;
    using SettingNodeValueChangeEvent = SettingNodeValueChangeEventV3;
}
