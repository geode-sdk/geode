#pragma once

#include <cocos2d.h>

namespace geode {
    // Enum for progress bar style
    enum class ProgressBarStyle {
        Level = 0,
        Solid = 1,
    };

    // Custom class for the progress bar
    class ProgressBar : public cocos2d::CCNode {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        ProgressBar();
        ~ProgressBar();

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
        void setProgressBarFillColor(ccColor3B color);

        /**
         * Update the size of the fill of the bar
         *
         * @param value A float from 0 to 100
         */
        virtual void updateProgress(float value);

        /**
         * Get the current progress percentage of the bar
         */
        float getProgress() const;

        /**
         * Get the progress percentage text label node
         */
        CCLabelBMFont* getProgressLabel() const;

        /**
         * Get the current style of the progress bar
         */
        ProgressBarStyle getProgressBarStyle() const;

        /**
         * Show the label displaying the current percentage of progress
         *
         * @param bool Whether to toggle visibility
         */
        void showProgressLabel(bool show);
    };
}