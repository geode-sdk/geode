#pragma once

#include <cocos2d.h>
#include <Geode/ui/ProgressBar.hpp>

namespace geode {
    // Custom class for the Normal/Practice mode style progress bar
    class ProgressBarSolid : public ProgressBar {
    protected:
        CCLabelBMFont* m_progressPercentLabel = nullptr; // The text label displaying the percentage

        bool init() override;

    public:
        // Create a custom progress bar
        static ProgressBarSolid* create();

        /**
         * Update the size of the fill of the bar and the percentage label
         *
         * @param value A float from 0 to 100
         */
        void updateProgress(float value) override;

        /**
         * Get the progress percentage text label node
         */
        CCLabelBMFont* getProgressLabel();
    };
}