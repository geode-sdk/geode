#include <Geode/ui/PopupManager.hpp>
#include <Geode/Geode.hpp>

#include <asp/time/SystemTime.hpp>
#include <asp/data/Cow.hpp>

using namespace geode::prelude;
using namespace asp::time;
using namespace asp::data;

namespace geode {

static const auto FIELDS_ID = "ManagedPopup-fields"_spr;
static constexpr int MANAGED_ALERT_TAG = 154385390;

struct ManagedPopup::Data {
    std::optional<asp::time::Instant> shownAt;
    asp::time::Duration blockClosingFor;
    bool persist = false;
    bool priority = false;
};

ManagedPopup::ManagedPopup(FLAlertLayer* layer) : inner(layer) {}
ManagedPopup::ManagedPopup() : inner(nullptr) {}

ManagedPopup::operator FLAlertLayer*() const {
    return inner;
}

FLAlertLayer* ManagedPopup::operator->() const {
    return inner;
}

FLAlertLayer* ManagedPopup::getInner() const {
    return inner;
}

void ManagedPopup::setPersistent(bool state) {
    this->getFields().persist = state;
}

void ManagedPopup::setPriority(bool state) {
    this->getFields().priority = state;
}

void ManagedPopup::blockClosingFor(const asp::time::Duration& dur) {
    this->getFields().blockClosingFor = dur;
}

void ManagedPopup::blockClosingFor(int durMillis) {
    return this->blockClosingFor(Duration::fromMillis(durMillis));
}

void ManagedPopup::showInstant() {
    this->doShow(false);
}

void ManagedPopup::showQueue() {
    bool back = !this->getFields().priority;
    PopupManager::get().queuePopup(*this, back);
}

bool ManagedPopup::isShown() {
    return this->inner->getParent();
}

bool ManagedPopup::shouldPreventClosing() {
    auto& fields = this->getFields();

    if (!fields.shownAt || fields.blockClosingFor.isZero()) {
        return false;
    }

    auto expiry = fields.shownAt.value() + fields.blockClosingFor;
    return expiry > Instant::now();
}

void ManagedPopup::doShow(bool reshowing) {
    if (!reshowing && inner->getParent()) {
        return;
    }

    inner->setTag(MANAGED_ALERT_TAG);
    inner->show();

    auto& fields = this->getFields();
    fields.shownAt = Instant::now();
}

ManagedPopup::Data& ManagedPopup::getFields() {
    using DataT = ObjWrapper<ManagedPopup::Data>;

    auto obj = typeinfo_cast<DataT*>(inner->getUserObject(FIELDS_ID));

    if (!obj) {
        obj = DataT::create(Data {});
        inner->setUserObject(FIELDS_ID, obj);
    }

    return obj->getValue();
}

bool ManagedPopup::hasFields() const {
    using DataT = ObjWrapper<ManagedPopup::Data>;

    return typeinfo_cast<DataT*>(inner->getUserObject(FIELDS_ID));
}

// PopupManager

PopupManager::PopupManager() {
    CCScheduler::get()->scheduleUpdateForTarget(this, 0, false);
}

PopupManager& PopupManager::get() {
    static PopupManager instance;
    return instance;
}

ManagedPopup PopupManager::alert(
    ZStringView title,
    const std::string& content,
    ZStringView btn1,
    ZStringView btn2,
    float width
) {
    auto alert = FLAlertLayer::create(
        nullptr,
        title.c_str(),
        content,
        btn1.c_str(),
        btn2.empty() ? nullptr : btn2.c_str(),
        width
    );
    return this->manage(alert);
}

ManagedPopup PopupManager::quickPopup(
    ZStringView title,
    const std::string& content,
    ZStringView btn1,
    ZStringView btn2,
    geode::Function<void (FLAlertLayer*, bool)> callback,
    float width
) {
    auto alert = createQuickPopup(
        title.c_str(),
        content,
        btn1.c_str(),
        btn2.empty() ? nullptr : btn2.c_str(),
        std::move(callback),
        false
    );
    return this->manage(alert);
}

ManagedPopup PopupManager::manage(FLAlertLayer* alert) {
    return ManagedPopup{alert};
}

bool PopupManager::isManaged(FLAlertLayer* alert) {
    return this->manage(alert).hasFields();
}

void PopupManager::queuePopup(const ManagedPopup& popup, bool back) {
    if (back) {
        m_queuedPopups.push_back(popup);
    } else {
        m_queuedPopups.push_front(popup);
    }
}

bool PopupManager::hasPendingPopups() const {
    return !m_queuedPopups.empty();
}

void PopupManager::update(float dt) {
    auto scene = CCScene::get();
    if (!scene) return;

    if (scene != m_prevScene) {
        this->changedScene(scene);
    }

    // check if we are eligible to show a queued popup

    // if there aren't any in the queue, there is nothing to show
    if (m_queuedPopups.empty()) {
        return;
    }

    // check if we are transitioning 🏳️‍⚧️
    if (m_isTransitioning) {
        return;
    }

    // if there's no current layer, don't show anything
    auto children = scene->getChildrenExt();
    if (children.empty()) return;
    auto layer = children[0];

    // if we are in loadinglayer, don't show anything
    if (typeinfo_cast<LoadingLayer*>(layer)) {
        return;
    }

    // if we are playing and are unpaused, only show priority popups
    bool playing = false;
    auto gjbgl = PlayLayer::get();
    if (gjbgl && !gjbgl->m_isPaused) {
        playing = true;
    }

    bool didPause = false;
    while (!m_queuedPopups.empty()) {
        auto popup = m_queuedPopups.front();
        bool isPrio = popup.getFields().priority;

        if (playing && !isPrio) {
            // we are currently playing a level, so we don't want to show non-prio popups
            // since prio popups are always put to the front of the queue, we know there's no more prio popups left
            break;
        }

        if (playing && isPrio && !didPause) {
            // pause the level
            if (gjbgl) {
                gjbgl->pauseGame(false);
            }
            didPause = true;
        }

        // show the popup
        popup.doShow();
        m_queuedPopups.pop_front();
    }
}

void PopupManager::changedScene(CCScene* newScene) {
    m_prevScene = newScene;

    auto trans = typeinfo_cast<CCTransitionScene*>(newScene);

    // if we are transitioning, let's save all our popups and bring them to the other scene if needed
    if (trans) {
        m_isTransitioning = true;

        std::vector<Ref<FLAlertLayer>> alerts;

        auto oldScene = trans->m_pOutScene;

        for (auto child : oldScene->getChildrenExt()) {
            if (child->m_nTag == MANAGED_ALERT_TAG) {
                if (auto alert = typeinfo_cast<FLAlertLayer*>(child)) {
                    alerts.push_back(alert);
                }
            }
        }

        for (auto& alert : alerts) {
            alert->removeFromParentAndCleanup(false);
        }

        m_savedAlerts = std::move(alerts);
    } else if (m_isTransitioning) {
        m_isTransitioning = false;

        // if we were transitioning but now switched to the actual scene, restore the popups
        for (auto& fla : m_savedAlerts) {
            auto alert = this->manage(fla);
            if (!alert.hasFields()) continue;

            // only re-show this alert if persistent it enabled or if it has been visible for way too short
            auto& fields = alert.getFields();
            auto sinceShown = fields.shownAt.value_or(Instant{}).elapsed();
            if (sinceShown < Duration::fromSecsF32(1.0f) || fields.persist) {
                alert.doShow(true);

                // reset the ref in the vector
                fla = {};
            }
        }

        for (auto& fla : m_savedAlerts) {
            // cleanup those alerts that were not shown again
            if (fla) {
                fla->cleanup();
            }
        }

        m_savedAlerts.clear();
    }
}

}