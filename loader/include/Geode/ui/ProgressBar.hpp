#pragma once

#include <Geode/ui/Label.hpp>
#include <Geode/utils/function.hpp>

namespace geode {
    // Enum for progress bar style
    enum class ProgressBarStyle : uint8_t {
        Level, // Style commonly seen in classic levels
        Slider, // Style commonly seen as sliders
        Solid, // Style commonly seen in level progression UI
    };

    class GEODE_DLL ProgressBar final : public cocos2d::CCNode {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    protected:
        ProgressBar();
        ~ProgressBar();

        void reloadStyle();

        void onAnimate(float dt);

        bool init(float progress, ProgressBarStyle style);

    public:
        using AnimationCallback = Function<void(ProgressBar* self, bool completed)>;

        /// Create a custom progress bar with a pre-determined progress state
        /// @param style Style of the progress bar
        /// @param progress A float from 0 to 100 for the amount of progress to start the bar with upon creation
        static ProgressBar* create(float progress, ProgressBarStyle style = ProgressBarStyle::Level);

        /// Create a custom progress bar
        /// @param style Style of the progress bar
        static ProgressBar* create(ProgressBarStyle style = ProgressBarStyle::Level);

        /// Set the style of the progress bar
        /// @param style Style to switch to
        void setStyle(ProgressBarStyle style);

        /// Set the color of the fill of the bar
        /// @param color RGB color object
        void setFillColor(cocos2d::ccColor3B color);

        /// Set the precision of the percentage label
        /// @param precision Number of decimal places to show in the percentage label
        void setPrecision(size_t precision);

        /// Update the size of the fill of the bar
        /// @param value A float from 0 to 100
        virtual void updateProgress(float value);

        /// Update the size of the fill of the bar with an animation
        /// @param value A float from 0 to 100
        /// @param duration Amount of seconds the animation will last
        /// @param ease Animation easing type
        /// @param callback Optional callback that fires when the animation ends
        virtual void updateProgress(float value, float duration, EasingType ease = EasingType::None, AnimationCallback callback = nullptr);

        /// Stop the running progress animation, if any
        void stopProgressAnimation();

        /// Show the label displaying the current percentage of progress
        /// @param bool Whether to toggle visibility
        void showProgressLabel(bool show);

        /// Get the current progress percentage of the bar
        float getProgress() const noexcept;

        /// Get the progress percentage text label node directly
        Label* getProgressLabel() const noexcept;

        /// Get the progress bar fill node directly
        cocos2d::CCSprite* getBarFill() const noexcept;

        /// Get the current style of the progress bar
        ProgressBarStyle getStyle() const noexcept;

        /// Get the current color of the fill of the progress bar
        cocos2d::ccColor3B getFillColor() const noexcept;

        /// Get the precision of the percentage label
        size_t getPrecision() const noexcept;
    };
};