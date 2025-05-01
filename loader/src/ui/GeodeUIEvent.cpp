#include "GeodeUIEvent.hpp"

ModPopupUIEvent::ModPopupUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {}
ModPopupUIEvent::~ModPopupUIEvent() = default;

FLAlertLayer* ModPopupUIEvent::getPopup() const {
    return m_impl->popup;
}
std::string ModPopupUIEvent::getModID() const {
    return m_impl->popup->getSource().getID();
}
std::optional<Mod*> ModPopupUIEvent::getMod() const {
    auto mod = m_impl->popup->getSource().asMod();
    return mod ? std::optional(mod) : std::nullopt;
}

ModItemUIEvent::ModItemUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {}
ModItemUIEvent::~ModItemUIEvent() = default;

CCNode* ModItemUIEvent::getItem() const {
    return m_impl->item;
}
std::string ModItemUIEvent::getModID() const {
    return m_impl->item->getSource().getID();
}
std::optional<Mod*> ModItemUIEvent::getMod() const {
    auto mod = m_impl->item->getSource().asMod();
    return mod ? std::optional(mod) : std::nullopt;
}

ModLogoUIEvent::ModLogoUIEvent(std::unique_ptr<Impl>&& impl) : m_impl(std::move(impl)) {}
ModLogoUIEvent::~ModLogoUIEvent() = default;

CCNode* ModLogoUIEvent::getSprite() const {
    return m_impl->sprite;
}
std::string ModLogoUIEvent::getModID() const {
    return m_impl->modID;
}
std::optional<Mod*> ModLogoUIEvent::getMod() const {
    if (auto mod = Loader::get()->getInstalledMod(m_impl->modID)) {
        return mod;
    }
    return std::nullopt;
}

// $execute {
//     new EventListener<EventFilter<ModLogoUIEvent>>(+[](ModLogoUIEvent* event) {
//         if (event->getModID() == "geode.loader") {
//             auto fart = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
//             fart->setScaleX(5);
//             fart->setScaleY(3);
//             event->getSprite()->addChildAtPosition(fart, Anchor::Center);
//         }
//         return ListenerResult::Propagate;
//     });
//     new EventListener<EventFilter<ModItemUIEvent>>(+[](ModItemUIEvent* event) {
//         if (event->getModID() == "geode.loader") {
//             auto fart = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
//             fart->setScaleX(4);
//             fart->setScaleY(2);
//             if (auto dev = event->getItem()->querySelector("developers-button")) {
//                 dev->addChildAtPosition(fart, Anchor::Center, ccp(-15, 0));
//             }
//         }
//         return ListenerResult::Propagate;
//     });
// }
