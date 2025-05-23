// #define GEODE_UI_TEST
#ifdef GEODE_UI_TEST

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/web.hpp>
#include <server/Server.hpp>
#include "../sources/ModListSource.hpp"

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
        clearAllModListSourceCaches();
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
        GUITestPopup::create()->show();
    }
};

#endif
