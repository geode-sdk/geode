#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <Base.hpp> // Cacao
#include <ccTypes.h>
#include <string>
#undef snprintf
#include <helpers/json.hpp>
#include <helpers/Result.hpp>
//#include <ccTypes.h>
#include <limits>
#undef min
#undef max

namespace cocos2d {
	class CCNode;
}

class BoolSettingNode;
class IntSettingNode;
class FloatSettingNode;
class StringSettingNode;
class ColorSettingNode;
class ColorAlphaSettingNode;
class PathSettingNode;
class StringSelectSettingNode;

class TableViewCell;

namespace lilac {

	class BoolSetting;
	class IntSetting;
	class FloatSetting;
	class StringSetting;
	class ColorSetting;
	class ColorAlphaSetting;
	class PathSetting;
	class StringSelectSetting;

	class Setting {
	protected:
		std::string m_key;

		friend class Loader;

	public:
		virtual TableViewCell* generate(float width) = 0;
		virtual ~Setting() = default;

		std::string getKey() const { return m_key; }

		virtual void save(nlohmann::json& json) const;
		virtual void load(nlohmann::json const& json);

		static Result<Setting*> parseFromJSON(nlohmann::json const& json);

		template<typename T>
		Result<T> getValueAs();
	};

	template<class SettingClass>
	class LilacSetting : public Setting {
	protected:
		std::string m_name = "";
		std::string m_description = "";

		Result<> parseFields(nlohmann::json const& json) {
			if (json.contains("name")) {
				if (json["name"].is_string()) {
					this->m_name = json["name"];
				} else {
					return Err<>("Setting has \"name\" but it is not a string");
				}
			}
			if (json.contains("description")) {
				if (json["description"].is_string()) {
					this->m_description = json["description"];
				} else {
					return Err<>("Setting has \"description\" but it is not a string");
				}
			}
			return Ok<>();
		}

	public:
		std::string getName() const { return m_name; }

		static Result<SettingClass*> parse(nlohmann::json const& json);
	};

	template<typename T, class SettingClass>
	class SingleSetting : public LilacSetting<SettingClass> {
	protected:
		T m_value;
		T m_default;

	public:
		T getValue() const { return m_value; }
		T getDefault() const { return m_default; }
	};

	template<typename T, class SettingClass>
	class SelectSetting : public LilacSetting<SettingClass> {
	protected:
		size_t m_value;
		size_t m_default;
		std::vector<T> m_options;
	
	public:
		size_t getIndex() const { return m_value; }
		size_t getDefaultIndex() const { return m_default; }
		T getValue() const { return m_options.at(m_value); }
		T getDefault() const { return m_options.at(m_default); }
	};

	class BoolSetting : public SingleSetting<bool, BoolSetting> {
	protected:
		friend class LilacSetting<BoolSetting>;
		friend class BoolSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class IntSetting : public SingleSetting<int, IntSetting> {
	protected:
		int m_min = std::numeric_limits<int>::min();
		int m_max = std::numeric_limits<int>::max();
		bool m_slider = false;
		bool m_input = true;
		bool m_arrows = true;

		friend class LilacSetting<IntSetting>;
		friend class IntSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class FloatSetting : public SingleSetting<float, FloatSetting> {
	protected:
		float m_min = std::numeric_limits<float>::min();
		float m_max = std::numeric_limits<float>::max();
		bool m_slider = true;
		bool m_input = true;
		bool m_arrows = true;

		friend class LilacSetting<FloatSetting>;
		friend class FloatSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class StringSetting : public SingleSetting<std::string, StringSetting> {
	protected:
		std::string m_filter;

		friend class LilacSetting<StringSetting>;
		friend class StringSettingNode;

	public:
		static bool replaceWithBuiltInFilter(std::string& filter);

		TableViewCell* generate(float width) override;
	};

	class ColorSetting : public SingleSetting<cocos2d::ccColor3B, ColorSetting> {
	protected:
		Result<cocos2d::ccColor3B> parseColor(nlohmann::json const& json);

		friend class LilacSetting<ColorSetting>;
		friend class ColorSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class ColorAlphaSetting : public SingleSetting<cocos2d::ccColor4B, ColorAlphaSetting> {
	protected:
		Result<cocos2d::ccColor4B> parseColor(nlohmann::json const& json);

		friend class LilacSetting<ColorAlphaSetting>;
		friend class ColorAlphaSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class PathSetting : public SingleSetting<ghc::filesystem::path, PathSetting> {
	protected:
		friend class LilacSetting<PathSetting>;
		friend class PathSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class StringSelectSetting : public SelectSetting<std::string, StringSelectSetting> {
	protected:
		friend class LilacSetting<StringSelectSetting>;
		friend class StringSelectSettingNode;

	public:
		TableViewCell* generate(float width) override;
	};

	class CustomSettingPlaceHolder : public Setting {
	public:
		TableViewCell* generate(float width) override;
	};

	template<typename T>
	Result<T> Setting::getValueAs() {
		if (std::is_same_v<T, bool>) {
			auto self = dynamic_cast<BoolSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a bool");
		}
		if (std::is_same_v<T, int>) {
			auto self = dynamic_cast<IntSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a int");
		}
		if (std::is_same_v<T, float>) {
			auto self = dynamic_cast<FloatSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a float");
		}
		if (std::is_same_v<T, size_t>) {
			auto self = dynamic_cast<SelectSetting<T, BoolSetting>*>(this); // BoolSetting works as a stub here
			return self ? Ok<T>(self->getIndex()) : Err<>("Not a select");
		}
		if (std::is_same_v<T, std::string>) {
			auto self = dynamic_cast<StringSetting*>(this);
			if (self) {
				return Ok<T>(self->getValue());
			} else {
				auto self2 = dynamic_cast<StringSelectSetting*>(this);
				if (self2) {
					return Ok<T>(self2->getValue());
				}
			}
			return Err<>("Not a string");
		}
		if (std::is_same_v<T, cocos2d::ccColor3B>) {
			auto self = dynamic_cast<ColorSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a ccColor3B");
		}
		if (std::is_same_v<T, cocos2d::ccColor4B>) {
			auto self = dynamic_cast<ColorAlphaSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a ccColor4B");
		}
		if (std::is_same_v<T, ghc::filesystem::path>) {
			auto self = dynamic_cast<PathSetting*>(this);
			return self ? Ok<T>(self->getValue()) : Err<>("Not a path");
		}
		return Err<>("Unsupported type");
	}
}

