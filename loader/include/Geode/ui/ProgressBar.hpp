#pragma once
#include <cocos2d.h>

namespace geode {
    // Enum for progress bar style
    enum class ProgressBarStyle {
        Level = 0, // Gameplay style
        LevelGold = 1, // Gameplay style but gold
        Solid = 2, // Thick style
    };

    // Custom class for the progress bar
    class GEODE_DLL ProgressBar final : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        ProgressBar();
        ~ProgressBar();

        /**
         * Set up `Level`/`LevelGold` style
         *
         * @note This is mainly to avoid repeating code since the only difference between both is one sprite name
         *
         * @param outlineSpr Sprite of the outline
         */
        void setupLevelStyle(const char* outlineSpr);

        // Set up `Solid` style
        void setupSolidStyle();

        // Reloads the style of the progress bar
        void reloadStyle();

        bool init(ProgressBarStyle style);

    public:
        /**
         * Create a custom progress bar
         *
         * @param style Style of the progress bar
         * @param goldVariant Switch to gold outline if using `Level` style
         */
        static ProgressBar* create(ProgressBarStyle style = ProgressBarStyle::Level);

        /**
         * Set the style of the progress bar
         *
         * @param style Style to switch to
         */
        void setStyle(ProgressBarStyle style);

        /**
         * Set the color of the fill of the bar
         *
         * @param color RGB color object
         */
        void setFillColor(cocos2d::ccColor3B color);

        /**
         * Set the precision of the percentage label
         *
         * @param precision Number of decimal places to show in the percentage label
         */
        void setPrecision(size_t precision);

        /**
         * Update the size of the fill of the bar
         *
         * @param value A float from 0 to 100
         */
        virtual void updateProgress(float value);

        /**
         * Show the label displaying the current percentage of progress
         *
         * @param bool Whether to toggle visibility
         */
        void showProgressLabel(bool show);

        /**
         * Get the current progress percentage of the bar
         */
        float getProgress() const noexcept;

        /**
         * Get the progress percentage text label node
         */
        cocos2d::CCLabelBMFont* getProgressLabel() const noexcept;

        /**
         * Get the current style of the progress bar
         */
        ProgressBarStyle getStyle() const noexcept;

        /**
         * Get the current color of the fill of the progress bar
         */
        cocos2d::ccColor3B getFillColor() const noexcept;

        /**
         * Get the precision of the percentage label
         */
        size_t getPrecision() const noexcept;
    };
};