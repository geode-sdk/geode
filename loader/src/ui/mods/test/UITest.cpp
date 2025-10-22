#if 1

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <server/Server.hpp>
#include "../sources/ModListSource.hpp"
#include <Geode/utils/Signals.hpp>

using namespace geode::prelude;

using StrTask = Task<std::string>;

class GUITestPopup : public Popup<> {
protected:
    CCLabelBMFont* m_rawTaskState;
    CCMenuItemSpriteExtra* m_cancelTaskBtn;
    CCMenuItemSpriteExtra* m_cancelServerTaskBtn;
    EventListener<web::WebTask> m_rawListener;
    EventListener<StrTask> m_strListener;
    EventListener<server::ServerRequest<server::ServerModsList>> m_serListener;
    EventListener<server::ServerRequest<server::ServerModsList>> m_serListener2;

    bool setup() override {
        m_noElasticity = true;
        this->setTitle("GUI Test Popup");

        auto startPromiseSpr = ButtonSprite::create(
            "Promise Test", "bigFont.fnt", "GJ_button_05.png", .8f
        );
        startPromiseSpr->setScale(.5f);
        auto startPromiseBtn = CCMenuItemSpriteExtra::create(
            startPromiseSpr, this, menu_selector(GUITestPopup::onPromiseTest)
        );
        m_buttonMenu->addChildAtPosition(startPromiseBtn, Anchor::Center, ccp(-40, 40));

        auto cancelPromiseSpr = ButtonSprite::create(
            "Cancel", "bigFont.fnt", "GJ_button_06.png", .8f
        );
        cancelPromiseSpr->setScale(.5f);
        m_cancelTaskBtn = CCMenuItemSpriteExtra::create(
            cancelPromiseSpr, this, menu_selector(GUITestPopup::onPromiseCancel)
        );
        m_cancelTaskBtn->setVisible(false);
        m_buttonMenu->addChildAtPosition(m_cancelTaskBtn, Anchor::Center, ccp(50, 40));

        m_rawTaskState = CCLabelBMFont::create("Task not started", "bigFont.fnt");
        m_rawTaskState->setScale(.5f);
        m_mainLayer->addChildAtPosition(m_rawTaskState, Anchor::Center, ccp(0, 10));

        auto serverPromiseSpr = ButtonSprite::create(
            "Server Request", "bigFont.fnt", "GJ_button_05.png", .8f
        );
        serverPromiseSpr->setScale(.5f);
        auto serverPromiseBtn = CCMenuItemSpriteExtra::create(
            serverPromiseSpr, this, menu_selector(GUITestPopup::onServerReq)
        );
        m_buttonMenu->addChildAtPosition(serverPromiseBtn, Anchor::Center, ccp(-40, -40));

        auto cancelServerPromiseSpr = ButtonSprite::create(
            "Cancel", "bigFont.fnt", "GJ_button_06.png", .8f
        );
        cancelServerPromiseSpr->setScale(.5f);
        m_cancelServerTaskBtn = CCMenuItemSpriteExtra::create(
            cancelServerPromiseSpr, this, menu_selector(GUITestPopup::onServerCancel)
        );
        m_cancelServerTaskBtn->setVisible(false);
        m_buttonMenu->addChildAtPosition(m_cancelServerTaskBtn, Anchor::Center, ccp(50, -40));

        auto clearServerCacheSpr = ButtonSprite::create(
            "Clear Caches", "bigFont.fnt", "GJ_button_01.png", .8f
        );
        clearServerCacheSpr->setScale(.5f);
        auto clearServerCacheBtn = CCMenuItemSpriteExtra::create(
            clearServerCacheSpr, this, menu_selector(GUITestPopup::onServerCacheClear)
        );
        m_buttonMenu->addChildAtPosition(clearServerCacheBtn, Anchor::Center, ccp(0, -70));

        m_rawListener.bind(this, &GUITestPopup::onRawTask);
        m_strListener.bind(this, &GUITestPopup::onStrTask);
        m_serListener.bind(this, &GUITestPopup::onServerTask);
        m_serListener2.bind(this, &GUITestPopup::onServerTask);

        return true;
    }

    void onRawTask(web::WebTask::Event* event) {
        m_cancelTaskBtn->setVisible(event->getProgress());

        if (event->isCancelled()) {
            m_rawTaskState->setString("Cancelled!");
        }
        if (auto value = event->getValue()) {
            m_rawTaskState->setString(fmt::format("Finished with code {}", value->code()).c_str());
        }
        if (auto progress = event->getProgress()) {
            m_rawTaskState->setString(fmt::format(
                "Progress: {}/{}", progress->downloaded(), progress->downloadTotal()
            ).c_str());
        }
    }
    void onStrTask(StrTask::Event* event) {
        if (event->isCancelled()) {
            log::info("str task cancelled :(");
        }
        else if (auto value = event->getValue()) {
            log::info("str task done: {}", *value);
        }
    }
    void onServerTask(server::ServerRequest<server::ServerModsList>::Event* event) {
        m_cancelServerTaskBtn->setVisible(event->getProgress());

        if (auto value = event->getValue()) {
            if (value->isOk()) {
                auto mods = value->unwrap();
                log::info("got a mods list with {}/{} mods!!!!", mods.mods.size(), mods.totalModCount);
            }
            else {
                log::info("epic mods list fail L: {}", value->unwrapErr().details);
            }
        }
        else if (auto prog = event->getProgress()) {
            log::info("mods list progress: {}", prog->message);
        }
        else if (event->isCancelled()) {
            log::info("mods list cancelled L");
        }
    }

    void makeRequest() {
        auto task = web::WebRequest().get("https://api.geode-sdk.org/");
        m_rawListener.setFilter(task);
        m_strListener.setFilter(task.map(
            [](auto* result) {
                return fmt::format("finish with code {} :3", result->code());
            },
            [](auto* progress) {
                return std::monostate();
            }
        ));
    }

    void onPromiseCancel(CCObject*) {
        m_rawListener.getFilter().cancel();
    }
    void onPromiseTest(CCObject*) {
        m_cancelTaskBtn->setVisible(true);
        this->makeRequest();
    }
    void onServerReq(CCObject*) {
        m_cancelServerTaskBtn->setVisible(true);
        m_serListener.setFilter(server::getMods(server::ModsQuery()));
        m_serListener2.setFilter(server::getMods(server::ModsQuery()));
    }
    void onServerCancel(CCObject*) {
        m_serListener.getFilter().cancel();
    }
    void onServerCacheClear(CCObject*) {
        server::clearServerCaches(true);
    }

public:
    static GUITestPopup* create() {
        auto ret = new GUITestPopup();
        if (ret->initAnchored(320, 280)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class SignalsTestPopup : public Popup<> {
protected:
    Signal<int> m_count = 0;
    DerivedSignal<int> m_countDoubled = DerivedSignal<int>([this]() {
        log::debug("calculating countDoubled {}", *m_count);
        return *m_count * 2;
    });
    SignalObserver m_endOfFrameObserver;
    SignalObserver m_immediateObserver;
    Signal<int> m_count2 = 0;
    Signal<int> m_count3 = 0;

    bool setup() override {
        m_noElasticity = true;
        this->setTitle("Signals Test Popup");

        this->reactToChanges([this] {
            log::debug("doubled count: {}", *m_countDoubled);
        });

        auto label = ButtonSprite::create("", "goldFont.fnt", "GJ_button_01.png", .8f);
        m_endOfFrameObserver = SignalObserver(
            [this, label]() {
                // log::debug("updating UI for count: {}", *m_count);
                label->setString(fmt::format("Clicked {} times", *m_count).c_str());
            },
            SignalObserverTime::EndOfFrame
        );
        m_immediateObserver = SignalObserver(
            [this]() {
                log::debug("immediate, count changed: {}", *m_count);
            },
            SignalObserverTime::Immediate
        );

        auto button = CCMenuItemSpriteExtra::create(
            label, this, menu_selector(SignalsTestPopup::onClick)
        );
        m_buttonMenu->addChildAtPosition(button, Anchor::Center, ccp(0, 30));

        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(
            ButtonSprite::create("-"),
            [this](auto) {
                m_count2 -= 1;
            }
        ), Anchor::Bottom, ccp(-50, 20));
        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(
            ButtonSprite::create("+"),
            [this](auto) {
                m_count2 += 1;
            }
        ), Anchor::Bottom, ccp(-20, 20));
        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(
            ButtonSprite::create("-"),
            [this](auto) {
                m_count3 -= 1;
            }
        ), Anchor::Bottom, ccp(20, 20));
        m_buttonMenu->addChildAtPosition(CCMenuItemExt::createSpriteExtra(
            ButtonSprite::create("+"),
            [this](auto) {
                m_count3 += 1;
            }
        ), Anchor::Bottom, ccp(50, 20));

        this->reactToChanges([this]() {
            // log::debug("rerendering count2");

            auto label = CCLabelBMFont::create(fmt::format("{}", *m_count2).c_str(), "bigFont.fnt");
            m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(-20, -20));

            onSignalDispose([label]() { label->removeFromParent(); });
        });
        this->reactToChanges([this]() {
            // log::debug("rerendering count3");

            auto label = CCLabelBMFont::create(fmt::format("{}", *m_count3).c_str(), "bigFont.fnt");
            m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(20, -20));

            onSignalDispose([label]() { label->removeFromParent(); });
        });
        this->reactToChanges([this]() {
            // log::debug("rerendering count2 == count3");

            auto label = CCLabelBMFont::create(
                (m_count3 == m_count2 ? "Counts are equal" : "Counts are not equal"),
                "bigFont.fnt"
            );
            if (m_count3 != m_count2) {
                label->setColor(ccRED);
            }
            label->setScale(.5f);
            m_mainLayer->addChildAtPosition(label, Anchor::Center, ccp(0, -40));

            onSignalDispose([label]() { label->removeFromParent(); });
        });

        return true;
    }

    void onClick(CCObject*) {
        // m_count += 2;
        // m_count -= 2;
        // m_count.update([](int count) { return count + 1; });
        m_count += 1;
    }

public:
    static SignalsTestPopup* create() {
        auto ret = new SignalsTestPopup();
        if (ret->initAnchored(320, 280)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

class $modify(GUILayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;

        auto btn = CCMenuItemSpriteExtra::create(
            CrossButtonSprite::create(CCLabelBMFont::create("test", "goldFont.fnt")),
            this, menu_selector(GUILayer::onTestPopup)
        );
        this->getChildByID("main-menu")->addChild(btn);
        this->getChildByID("main-menu")->updateLayout();

        return true;
    }

    void onTestPopup(CCObject*) {
        SignalsTestPopup::create()->show();
    }
};

#endif
