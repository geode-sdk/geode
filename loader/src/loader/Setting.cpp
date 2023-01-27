#include "../ui/internal/settings/GeodeSettingNode.hpp"

#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/loader/SettingJsonTest.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <re2/re2.h>

USE_GEODE_NAMESPACE();

template<class T>
static void parseCommon(T& sett, JsonMaybeObject<ModJson>& obj) {
    obj.has("name").into(sett.name);
    obj.has("description").into(sett.description);
    obj.has("default").into(sett.defaultValue);
}

Result<BoolSetting> BoolSetting::parse(JsonMaybeObject<ModJson>& obj) {
    BoolSetting sett;
    parseCommon(sett, obj);
    return Ok(sett);
}

Result<IntSetting> IntSetting::parse(JsonMaybeObject<ModJson>& obj) {
    IntSetting sett;
    parseCommon(sett, obj);
    obj.has("min").into(sett.min);
    obj.has("max").into(sett.max);
    if (auto controls = obj.has("control").obj()) {
        controls.has("arrows").into(sett.controls.arrows);
        controls.has("big-arrows").into(sett.controls.bigArrows);
        controls.has("arrow-step").into(sett.controls.arrowStep);
        controls.has("big-arrow-step").into(sett.controls.bigArrowStep);
        controls.has("slider").into(sett.controls.slider);
        controls.has("slider-step").into(sett.controls.sliderStep);
        controls.has("input").into(sett.controls.input);
    }
    return Ok(sett);
}

Result<FloatSetting> FloatSetting::parse(JsonMaybeObject<ModJson>& obj) {
    FloatSetting sett;
    parseCommon(sett, obj);
    obj.has("min").into(sett.min);
    obj.has("max").into(sett.max);
    if (auto controls = obj.has("control").obj()) {
        controls.has("arrows").into(sett.controls.arrows);
        controls.has("big-arrows").into(sett.controls.bigArrows);
        controls.has("arrow-step").into(sett.controls.arrowStep);
        controls.has("big-arrow-step").into(sett.controls.bigArrowStep);
        controls.has("slider").into(sett.controls.slider);
        controls.has("slider-step").into(sett.controls.sliderStep);
        controls.has("input").into(sett.controls.input);
    }
    return Ok(sett);
}

Result<StringSetting> StringSetting::parse(JsonMaybeObject<ModJson>& obj) {
    StringSetting sett;
    parseCommon(sett, obj);
    obj.has("match").into(sett.match);
    return Ok(sett);
}

Result<FileSetting> FileSetting::parse(JsonMaybeObject<ModJson>& obj) {
    FileSetting sett;
    parseCommon(sett, obj);
    if (auto controls = obj.has("control").obj()) {
        for (auto& item : controls.has("filters").iterate()) {
            if (auto iobj = item.obj()) {
                Filter filter;
                iobj.has("description").into(filter.description);

                std::vector<json::Value> files;
                iobj.has("files").into(files);

                for (auto& i : files) {
                    filter.files.insert(i.as<std::string>());
                }

                sett.controls.filters.push_back(filter);
            }
        }
    }
    return Ok(sett);
}

Result<ColorSetting> ColorSetting::parse(JsonMaybeObject<ModJson>& obj) {
    ColorSetting sett;
    parseCommon(sett, obj);
    return Ok(sett);
}

Result<ColorAlphaSetting> ColorAlphaSetting::parse(JsonMaybeObject<ModJson>& obj) {
    ColorAlphaSetting sett;
    parseCommon(sett, obj);
    return Ok(sett);
}

Result<Setting> Setting::parse(
    std::string const& key,
    JsonMaybeValue<ModJson>& value
) {
    auto sett = Setting();
    sett.m_key = key;
    if (auto obj = value.obj()) {
        std::string type;
        obj.needs("type").into(type);
        if (type.size()) {
            switch (hash(type.c_str())) {
                case hash("bool"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, BoolSetting::parse(obj));
                } break;

                case hash("int"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, IntSetting::parse(obj));
                } break;

                case hash("float"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, FloatSetting::parse(obj));
                } break;

                case hash("string"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, StringSetting::parse(obj));
                } break;

                case hash("rgb"): case hash("color"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, ColorSetting::parse(obj));
                } break;

                case hash("rgba"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, ColorAlphaSetting::parse(obj));
                } break;

                case hash("path"): case hash("file"): {
                    GEODE_UNWRAP_INTO(sett.m_kind, FileSetting::parse(obj));
                } break;

                case hash("custom"): {
                    sett.m_kind = CustomSetting {
                        .json = std::make_shared<ModJson>(obj.json())
                    };
                    // skip checking unknown keys
                    return Ok(sett);
                } break;

                default: return Err("Unknown setting type \"" + type + "\"");
            }
        }
        obj.checkUnknownKeys();
    }
    // if the type wasn't an object or a string, the JsonChecker that gave the 
    // JsonMaybeValue will fail eventually so we can continue on
    return Ok(sett);
}

Setting::Setting(std::string const& key, SettingKind const& kind)
  : m_key(key), m_kind(kind) {}

std::unique_ptr<SettingValue> Setting::createDefaultValue() const {
    return std::visit(makeVisitor {
        [&](BoolSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<BoolSettingValue>(m_key, sett);
        },
        [&](IntSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<IntSettingValue>(m_key, sett);
        },
        [&](FloatSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<FloatSettingValue>(m_key, sett);
        },
        [&](StringSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<StringSettingValue>(m_key, sett);
        },
        [&](FileSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<FileSettingValue>(m_key, sett);
        },
        [&](ColorSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<ColorSettingValue>(m_key, sett);
        },
        [&](ColorAlphaSetting const& sett) -> std::unique_ptr<SettingValue> {
            return std::make_unique<ColorAlphaSettingValue>(m_key, sett);
        },
        [&](auto const& sett) -> std::unique_ptr<SettingValue> {
            return nullptr;
        },
    }, m_kind);
}

bool Setting::isCustom() const {
    return std::holds_alternative<CustomSetting>(m_kind);
}

std::string Setting::getDisplayName() const {
    return std::visit(makeVisitor {
        [&](CustomSetting const& sett) {
            return std::string();
        },
        [&](auto const& sett) {
            return sett.name.value_or(m_key);
        },
    }, m_kind);
}

std::optional<std::string> Setting::getDescription() const {
    return std::visit(makeVisitor {
        [&](CustomSetting const& sett) -> std::optional<std::string> {
            return std::nullopt;
        },
        [&](auto const& sett) {
            return sett.description;
        },
    }, m_kind);
}

// SettingValue

SettingValue::SettingValue(std::string const& key) : m_key(key) {}

std::string SettingValue::getKey() const {
    return m_key;
}

// GeodeSettingValue & SettingValueSetter specializations

#define IMPL_NODE_AND_SETTERS(type_) \
    template<>                                                          \
    SettingNode* GeodeSettingValue<                                     \
        type_##Setting                                                  \
    >::createNode(float width) {                                        \
        return type_##SettingNode::create(this, width);                 \
    }                                                                   \
    template<>                                                          \
    void GeodeSettingValue<                                             \
        type_##Setting                                                  \
    >::setValue(ValueType const& value) {                               \
        m_value = this->toValid(value).first;                           \
    }                                                                   \
    template<>                                                          \
    Result<> GeodeSettingValue<                                         \
        type_##Setting                                                  \
    >::validate(ValueType const& value) const {                         \
        auto reason = this->toValid(value).second;                      \
        if (reason.has_value()) {                                       \
            return Err(static_cast<std::string>(reason.value()));       \
        }                                                               \
        return Ok();                                                    \
    }                                                                   \
    template<>                                                          \
    typename type_##Setting::ValueType SettingValueSetter<              \
        typename type_##Setting::ValueType                              \
    >::get(SettingValue* setting) {                                     \
        if (auto b = typeinfo_cast<type_##SettingValue*>(setting)) {    \
            return b->getValue();                                       \
        }                                                               \
        return typename type_##Setting::ValueType();                    \
    }                                                                   \
    template<>                                                          \
    void SettingValueSetter<                                            \
        typename type_##Setting::ValueType                              \
    >::set(                                                             \
        SettingValue* setting,                                          \
        typename type_##Setting::ValueType const& value                 \
    ) {                                                                 \
        if (auto b = typeinfo_cast<type_##SettingValue*>(setting)) {    \
            b->setValue(value);                                         \
        }                                                               \
    }

#define IMPL_TO_VALID(type_) \
    template<>                                          \
    typename GeodeSettingValue<type_##Setting>::Valid   \
    GeodeSettingValue<type_##Setting>::toValid(         \
        typename type_##Setting::ValueType const& value \
    ) const

// instantiate value setters

namespace geode {
    template struct SettingValueSetter<typename BoolSetting::ValueType>;
    template struct SettingValueSetter<typename IntSetting::ValueType>;
    template struct SettingValueSetter<typename FloatSetting::ValueType>;
    template struct SettingValueSetter<typename StringSetting::ValueType>;
    template struct SettingValueSetter<typename FileSetting::ValueType>;
    template struct SettingValueSetter<typename ColorSetting::ValueType>;
    template struct SettingValueSetter<typename ColorAlphaSetting::ValueType>;
}

// instantiate values

namespace geode {
    template class GeodeSettingValue<BoolSetting>;
    template class GeodeSettingValue<IntSetting>;
    template class GeodeSettingValue<FloatSetting>;
    template class GeodeSettingValue<StringSetting>;
    template class GeodeSettingValue<FileSetting>;
    template class GeodeSettingValue<ColorSetting>;
    template class GeodeSettingValue<ColorAlphaSetting>;
}

IMPL_TO_VALID(Bool) {
    return { value, std::nullopt };
}

IMPL_TO_VALID(Int) {
    if (m_definition.min && value < m_definition.min) {
        return { m_definition.min.value(), fmt::format(
            "Value must be more than or equal to {}",
            m_definition.min.value()
        ) };
    }
    if (m_definition.max && value > m_definition.max) {
        return { m_definition.max.value(), fmt::format(
            "Value must be less than or equal to {}",
            m_definition.max.value()
        ) };
    }
    return { value, std::nullopt };
}

IMPL_TO_VALID(Float) {
    if (m_definition.min && value < m_definition.min) {
        return { m_definition.min.value(), fmt::format(
            "Value must be more than or equal to {}",
            m_definition.min.value()
        ) };
    }
    if (m_definition.max && value > m_definition.max) {
        return { m_definition.max.value(), fmt::format(
            "Value must be less than or equal to {}",
            m_definition.max.value()
        ) };
    }
    return { value, std::nullopt };
}

IMPL_TO_VALID(String) {
    if (m_definition.match) {
        if (!re2::RE2::FullMatch(value, m_definition.match.value())) {
            return {
                m_definition.defaultValue,
                fmt::format(
                    "Value must match regex {}",
                    m_definition.match.value()
                )
            };
        }
    }
    return { value, std::nullopt };
}

IMPL_TO_VALID(File) {
    return { value, std::nullopt };
}

IMPL_TO_VALID(Color) {
    return { value, std::nullopt };
}

IMPL_TO_VALID(ColorAlpha) {
    return { value, std::nullopt };
}

IMPL_NODE_AND_SETTERS(Bool);
IMPL_NODE_AND_SETTERS(Int);
IMPL_NODE_AND_SETTERS(Float);
IMPL_NODE_AND_SETTERS(String);
IMPL_NODE_AND_SETTERS(File);
IMPL_NODE_AND_SETTERS(Color);
IMPL_NODE_AND_SETTERS(ColorAlpha);

// SettingChangedEvent

SettingChangedEvent::SettingChangedEvent(Mod* mod, SettingValue* value)
  : mod(mod), value(value) {}

// SettingChangedFilter

ListenerResult SettingChangedFilter::handle(
    std::function<Callback> fn, SettingChangedEvent* event
) {
    if (m_modID == event->mod->getID() &&
        (!m_targetKey || m_targetKey.value() == event->value->getKey())
    ) {
        fn(event->value);
    }
    return ListenerResult::Propagate;
}

SettingChangedFilter::SettingChangedFilter(
    std::string const& modID,
    std::optional<std::string> const& settingKey
) : m_modID(modID), m_targetKey(settingKey) {}
