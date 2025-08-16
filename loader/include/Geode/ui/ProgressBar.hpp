#pragma once

#include <cocos2d.h>

namespace geode {
    // Custom class for the progress bar
    class ProgressBar : public cocos2d::CCNode {
    protected:
        Ref<CCSprite> m_progressBar = nullptr; // Progress bar outline
        CCSprite* m_progressBarFill = nullptr; // Progress bar fill

        float m_progress = 0.f; // Current progress bar fill percentage ranging from 0 to 100

        float m_progressBarFillMaxWidth = 0.f; // Max width for the progress fill bar node
        float m_progressBarFillMaxHeight = 0.f; // Max height for the progress fill bar node

        bool init() override;

    public:
        // Create a custom progress bar
        static ProgressBar* create();

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
        float getProgress();
    };
}