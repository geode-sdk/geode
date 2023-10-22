#include <Geode/ui/Colors.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

using namespace geode::prelude;

ColorManager::ColorManager() : m_colors({
    { GDColor::NormalModeProgressBar,   {{ ccColor3B { 0,   255, 0   }, Mod::get() }} },
    { GDColor::PracticeModeProgressBar, {{ ccColor3B { 0,   255, 255 }, Mod::get() }} },
    { GDColor::ProfilePostBG,           {{ ccColor3B { 191, 114, 62  }, Mod::get() }} },
}) {}

ColorManager* ColorManager::get() {
    static auto inst = new ColorManager;
    return inst;
}

ccColor3B ColorManager::getColor(GDColor color) const {
    return m_colors.at(color).back().value;
}

void ColorManager::setColor(GDColor color, Mod* setter, ccColor3B const& value) {
    m_colors.at(color).push_back({ value, setter });
}

void ColorManager::resetColor(GDColor color, Mod* setter) {
    ranges::remove(m_colors.at(color), [setter](Value const& value) {
        return value.setter == setter;
    });
}
