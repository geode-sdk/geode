#pragma once
#include <cocos2d.h>

namespace geode {
    // Enum for progress bar style
    enum class ProgressBarStyle {
        Level = 0, // Gameplay style
        Solid = 1, // Thick style
    };
    GEODE_DLL const char* baseEnumToString(ProgressBarStyle);

    // Custom class for the progress bar
    class GEODE_DLL ProgressBar final : public cocos2d::CCNode {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        ProgressBar();
        virtual ~ProgressBar();

        // Reloads the style of the progress bar
        void reloadStyle();

        bool init() override;

    public:
        // Create a custom progress bar
        static ProgressBar* create();

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
        float getProgress() const;

        /**
         * Get the progress percentage text label node
         */
        cocos2d::CCLabelBMFont* getProgressLabel() const;

        /**
         * Get the current style of the progress bar
         */
        ProgressBarStyle getStyle() const;

        /**
         * Get the current color of the fill of the progress bar
         */
        cocos2d::ccColor3B getFillColor() const;

        /**
         * Get the precision of the percentage label
         */
        size_t getPrecision() const;
    };
};