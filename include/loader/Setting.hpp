#pragma once

#include "../base/Macros.hpp"
#include "Types.hpp"
// #include <Base.hpp> // Geode
#include <ccTypes.h>
#include <string>
#undef snprintf
#include <utils/json.hpp>
#include <utils/Result.hpp>
//#include <ccTypes.h>
#include <limits>
#undef min
#undef max
#include <utils/general.hpp>

namespace cocos2d {
	class CCNode;
}

struct TableViewCell;

namespace geode {
	class BoolSetting;
	class IntSetting;
	class FloatSetting;
	class StringSetting;
	class ColorSetting;
	class ColorAlphaSetting;
	class PathSetting;
	class StringSelectSetting;

	enum class SettingType {
		Bool,
		Int,
		Float,
		String,
		Color,
		ColorAlpha,
		Path,
		StringSelect,
		Custom,
	};

	class GEODE_DLL Setting {
	protected:
		std::string m_key;
		Mod* m_mod = nullptr;

		friend class Loader;

	public:
		virtual SettingType getType() = 0;
		virtual ~Setting() = default;

		std::string getKey() const { return m_key; }

		virtual Result<> save(nlohmann::json& json) const = 0;
		virtual Result<> load(nlohmann::json const& json) = 0;

		static Result<Setting*> parseFromJSON(nlohmann::json const& json);

		virtual void resetToDefault() = 0;
		void update();

		template<typename T>
		Result<T> getValueAs();
	};

	template<class SettingClass>
	class GeodeSetting : public Setting {
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
		std::string getDescription() const { return m_description; }

		static Result<SettingClass*> parse(nlohmann::json const& json);
	};

	template<typename T, class Friend, bool Slider, bool Input, bool Arrows>
	class INumericSetting {
	protected:
		T m_min = std::numeric_limits<T>::min();
		T m_max = std::numeric_limits<T>::max();
		T m_step = 1;
		bool m_slider = Slider;
		bool m_input = Input;
		bool m_arrows = Arrows;

		friend class GeodeSetting<Friend>;
	
	public:
		T getMin() { return m_min; }
		T getMax() { return m_max; }
		T getStep() { return m_step; }
		bool hasSlider() { return m_slider; }
		bool hasInput() { return m_input; }
		bool hasArrows() { return m_arrows; }
	};

	template<typename T, class SettingClass>
	class SingleSetting : public GeodeSetting<SettingClass> {
	protected: 
		T m_value = T();
		T m_default = T();

	public:
		T getValue() const { return m_value; }
		T getDefault() const { return m_default; }

		using value_type_t = T;

		void setValue(T val) {
			m_value = val;
			this->update();
		}

		void resetToDefault() override {
			this->setValue(m_default);
		}
	};

	template<typename T, class SettingClass>
	class SelectSetting : public GeodeSetting<SettingClass> {
	protected:
		size_t m_value;
		size_t m_default;
		std::vector<T> m_options;
	
	public:
		size_t getIndex() const { return m_value; }
		size_t getDefaultIndex() const { return m_default; }
		T getValue() const { return m_options.at(m_value); }
		T getDefault() const { return m_options.at(m_default); }
		T getValueAt(size_t ix) const { return m_options.at(ix); }
		std::vector<T> getOptions() const { return m_options; };

		using value_type_t = size_t;
		
		void setIndex(size_t value) {
			m_value = utils::clamp<size_t>(value, 0, m_options.size() - 1);
			this->update();
		}
		void incrementIndex(long increment) {
			auto newValue = m_value + increment;
			if (newValue < 0) m_value = 0;
			else if (newValue > m_options.size() - 1) m_value = m_options.size() - 1;
			else m_value = newValue;
			this->update();
		}
		void incrementIndexWrap(long increment) {
			long newValue = m_value + increment;
			if (newValue < 0) m_value = m_options.size() - 1;
			else if (newValue > static_cast<long>(m_options.size() - 1)) m_value = 0;
			else m_value = newValue;
			this->update();
		}

		void resetToDefault() override {
			this->setIndex(m_default);
		}
	};

	class BoolSetting : public SingleSetting<bool, BoolSetting> {
	protected:
		friend class GeodeSetting<BoolSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::Bool; }
	};

	class IntSetting : public SingleSetting<int, IntSetting>, public INumericSetting<int, IntSetting, false, true, true> {
	protected:
		friend class GeodeSetting<IntSetting>;

		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
	
	public:
		inline virtual SettingType getType() override { return SettingType::Int; }
	};

	class FloatSetting : public SingleSetting<float, FloatSetting>, public INumericSetting<int, FloatSetting, true, true, true> {
	protected:
		size_t m_precision = 0;

		friend class GeodeSetting<FloatSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::Float; }

		size_t getPrecision() const { return m_precision; }
	};

	class StringSetting : public SingleSetting<std::string, StringSetting> {
	protected:
		std::string m_filter;

		friend class GeodeSetting<StringSetting>;

		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		std::string getFilter() const { return m_filter; }

		static bool replaceWithBuiltInFilter(std::string& filter);
		inline virtual SettingType getType() override { return SettingType::String; }
	};

	class ColorSetting : public SingleSetting<cocos2d::ccColor3B, ColorSetting> {
	protected:
		Result<cocos2d::ccColor3B> parseColor(nlohmann::json const& json);

		friend class GeodeSetting<ColorSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::Color; }
	};

	class ColorAlphaSetting : public SingleSetting<cocos2d::ccColor4B, ColorAlphaSetting> {
	protected:
		Result<cocos2d::ccColor4B> parseColor(nlohmann::json const& json);

		friend class GeodeSetting<ColorAlphaSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::ColorAlpha; }
	};

	class PathSetting : public SingleSetting<ghc::filesystem::path, PathSetting> {
	protected:
		friend class GeodeSetting<PathSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::Path; }
	};

	class StringSelectSetting : public SelectSetting<std::string, StringSelectSetting> {
	protected:
		friend class GeodeSetting<StringSelectSetting>;
	
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
	public:
		inline virtual SettingType getType() override { return SettingType::StringSelect; }
	};

	class CustomSettingPlaceHolder : public Setting {
	protected:
		Result<> save(nlohmann::json& json) const override;
		Result<> load(nlohmann::json const& json) override;
		
		void resetToDefault() override {}
		
	public:
		inline virtual SettingType getType() override { return SettingType::Custom; }
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

