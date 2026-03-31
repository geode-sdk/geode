#include <Geode/Geode.hpp>

using namespace geode::prelude;

static bool isAprilFools() {
    auto now = std::chrono::system_clock::now();
    auto timeNow = std::chrono::system_clock::to_time_t(now);
    auto tm_local = asp::localtime(timeNow);

    return tm_local.tm_mon == 3 && tm_local.tm_mday == 1;
}

namespace sapphire {
    class CountdownTimer : public CCLabelBMFont {
    public:
        static CountdownTimer* create() {
            auto ret = new CountdownTimer();
            if (ret->initWithString("", "chatFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight)) {
                ret->autorelease();
                ret->schedule(schedule_selector(CountdownTimer::updateTimer), 1.f);
                ret->updateTimer(0.f);
                return ret;
            }
            delete ret;
            return nullptr;
        }

        void updateTimer(float dt) {
            static auto aprilSecond = []() {
                auto now = std::chrono::system_clock::now();
                auto timeNow = std::chrono::system_clock::to_time_t(now);
                auto tm_local = asp::localtime(timeNow);

                tm_local.tm_mon = 3;
                tm_local.tm_mday = 2;
                tm_local.tm_hour = 0;
                tm_local.tm_min = 0;
                tm_local.tm_sec = 0;

                return asp::SystemTime::fromUnix(std::mktime(&tm_local));
            }();

            auto now = asp::SystemTime::now();
            auto difference = aprilSecond - now;

            if (difference.has_value()) {
                auto minutes = static_cast<int>(difference->minutes());
                auto seconds = static_cast<int>(difference->seconds() % 60);
                this->setString(
                    fmt::format(
                        "Sapphire Pro Trial Edition\nRemaining time: {:02}:{:02}",
                        minutes, seconds
                    ).c_str()
                );
            } else {
                FLAlertLayer::create(
                    "Happy April Fools!",
                    "Don't worry, Geode SDK will forever remain free for everyone :D",
                    "OK"
                )->show();

                this->removeFromParentAndCleanup(true);
            }
        }
    };
}


$on_game(Loaded) {
    if (!isAprilFools()) return;

    FLAlertLayer::create(
        "Sapphire Pro",
        "Geode SDK has been recently acquired by Sapphire Enterprise Inc. "
        "and now requires a premium subscription to keep using your favorite mods! "
        "You may keep using the free version of Sapphire SDK which provides a limited time trial.",
        "OK"
    )->show();

    auto timer = sapphire::CountdownTimer::create();
    if (!timer) return;

    timer->setScale(0.75f);
    timer->setAnchorPoint({1.f, 1.f});
    timer->setPosition(CCDirector::get()->getWinSize() - CCSize{5.f, 5.f});

    OverlayManager::get()->addChild(timer);
}