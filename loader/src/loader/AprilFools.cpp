#include <Geode/Geode.hpp>
#include "ui/mods/ModsLayer.hpp"

using namespace geode::prelude;

namespace sapphire {
    class CountdownTimer : public CCNode {
    public:
        CCLabelBMFont* m_timer;

        static CountdownTimer* create() {
            auto ret = new CountdownTimer();
            if (ret->init()) {
                ret->autorelease();
                ret->schedule(schedule_selector(CountdownTimer::updateTimer), 0.f);
                ret->updateTimer(0.f);
                return ret;
            }
            delete ret;
            return nullptr;
        }

        bool init() {
            this->setLayout(SimpleColumnLayout::create()->setCrossAxisAlignment(CrossAxisAlignment::Start)->setGap(5.f));
            this->setContentSize(CCSize{250, 50});

            auto activate = CCLabelBMFont::create("Activate Sapphire Pro", "chatFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight);
            activate->setScale(1.2f);
            activate->setColor({ 255, 255, 255 });
            activate->setOpacity(200);
            this->addChild(activate);

            auto sapphirePro = CCLabelBMFont::create("Go to Settings to activate Sapphire Pro.", "chatFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight);
            sapphirePro->setScale(0.8f);
            sapphirePro->setColor({ 255, 255, 255 });
            sapphirePro->setOpacity(200);
            this->addChild(sapphirePro);

            m_timer = CCLabelBMFont::create("", "chatFont.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentRight);
            m_timer->setScale(0.8f);
            m_timer->setColor({ 255, 255, 255 });
            m_timer->setOpacity(200);
            this->addChild(m_timer);

            this->updateLayout();

            return true;
        }

        void updateTimer(float dt) {
            if (MenuLayer::get() || CCScene::get()->getChildByType<ModsLayer*>(0)) {
                this->setVisible(true);
            }
            else {
                this->setVisible(false);
            }
            if (!this->isVisible()) return;

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
                auto hours = static_cast<int>(difference->hours());
                auto minutes = static_cast<int>(difference->minutes() % 60);
                auto seconds = static_cast<int>(difference->seconds() % 60);
                m_timer->setString(
                    fmt::format(
                        "Evaluation period: {:02}:{:02}:{:02}",
                        hours, minutes, seconds
                    ).c_str()
                );
            } else {
                Mod::get()->getSaveContainer().erase("sapphire-style");
                Mod::get()->getSaveContainer().erase("show-april-popup-2026");

                FLAlertLayer::create(
                    "Happy April Fools!",
                    "Don't worry, Geode SDK will forever remain free for everyone :D",
                    "OK"
                )->show();

                this->removeFromParentAndCleanup(true);
            }

            this->updateLayout();
        }
    };
}


$on_game(Loaded) {
    if (Mod::get()->getSavedValue("show-april-popup-2026", false)) {
        Mod::get()->getSaveContainer().erase("sapphire-style");
        Mod::get()->getSaveContainer().erase("show-april-popup-2026");
        FLAlertLayer::create(
            "Happy April Fools!",
            "Don't worry, Geode SDK will forever remain free for everyone :D",
            "OK"
        )->show();
    }

    if (!Mod::get()->getSavedValue("sapphire-style", false)) return;

    FLAlertLayer::create(
        "Sapphire Pro",
        "Geode SDK has been recently acquired by Sapphire Enterprise Inc. "
        "and now requires a premium subscription to keep using your favorite mods! "
        "You may keep using the free version of Sapphire SDK which provides a limited time trial.",
        "OK"
    )->show();

    auto timer = sapphire::CountdownTimer::create();
    if (!timer) return;
    auto winsize = CCDirector::get()->getWinSize();

    timer->setAnchorPoint({1.f, 0.f});
    timer->setScale(0.75f);
    timer->setPosition(CCSize{winsize.width - 10.f, 10.f});

    OverlayManager::get()->addChild(timer);
}