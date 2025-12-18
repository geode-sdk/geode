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
    using SettingChangedFilter = SettingChangedFilterV3;
    using SettingNodeSizeChangeEvent = SettingNodeSizeChangeEventV3;
    using SettingNodeValueChangeEvent = SettingNodeValueChangeEventV3;

    template <class T>
    EventListener<SettingChangedFilter>* listenForSettingChanges(std::string settingKey, auto callback, Mod* mod = getMod()) {
        return listenForSettingChangesV3<T>(settingKey, std::move(callback), mod);
    }

    EventListener<SettingChangedFilter>* listenForSettingChanges(std::string settingKey, auto callback, Mod* mod = getMod()) {
        return listenForSettingChangesV3(settingKey, std::move(callback), mod);
    }

    inline EventListener<SettingChangedFilter>* listenForAllSettingChanges(
        geode::Function<void(std::shared_ptr<SettingV3>)> callback,
        Mod* mod = getMod()
    ) {
        return listenForAllSettingChangesV3(std::move(callback), mod);
    }
}
