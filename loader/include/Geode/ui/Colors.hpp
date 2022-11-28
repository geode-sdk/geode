#pragma once

#include "../DefaultInclude.hpp"
#include "../loader/Mod.hpp"
#include <cocos2d.h>

namespace geode {
    // Credit to https://github.com/Ikszyon/UI-Recolor for many of these addresses!

    /**
     * Hardcoded GD colors
     */
    enum class GDColor {
        NormalModeProgressBar,
        PracticeModeProgressBar,

        ProfilePostBG,
    };

    class GEODE_DLL ColorManager {
    protected:
        struct Value {
            cocos2d::ccColor3B value;
            Mod* setter;
        };

        std::unordered_map<GDColor, std::vector<Value>> m_colors;

        ColorManager();
    
    public:
        static ColorManager* get();

        cocos2d::ccColor3B getColor(GDColor color) const;
        void setColor(GDColor color, Mod* setter, cocos2d::ccColor3B const& value);
        void resetColor(GDColor color, Mod* setter);

        template<class = void>
        void setColor(GDColor color, cocos2d::ccColor3B const& value) {
            this->setColor(color, Mod::get(), value);
        }
        template<class = void>
        void resetColor(GDColor color) {
            this->resetColor(color, Mod::get());
        }
    };
}
