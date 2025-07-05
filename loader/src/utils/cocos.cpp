#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <matjson.hpp>
#include <charconv>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GameManager.hpp>

using namespace geode::prelude;

Result<cocos2d::ccColor3B, std::string> matjson::Serialize<ccColor3B>::fromJson(matjson::Value const& value) {
    if (value.isArray()) {
        auto arr = GEODE_UNWRAP(value.asArray());
        if (arr.size() == 3) {
            auto r = GEODE_UNWRAP(arr[0].asInt());
            auto g = GEODE_UNWRAP(arr[1].asInt());
            auto b = GEODE_UNWRAP(arr[2].asInt());
            return Ok(cocos2d::ccc3(r, g, b));
        }
        return Err("Expected color array to have 3 items");
    }
    if (value.isObject()) {
        auto r = GEODE_UNWRAP(GEODE_UNWRAP(value.get("r")).asInt());
        auto g = GEODE_UNWRAP(GEODE_UNWRAP(value.get("g")).asInt());
        auto b = GEODE_UNWRAP(GEODE_UNWRAP(value.get("b")).asInt());
        return Ok(cocos2d::ccc3(r, g, b));
    }
    if (value.isString()) {
        auto hex = GEODE_UNWRAP(value.asString());
        auto res = cc3bFromHexString(hex);
        if (!res) {
            return Err("Invalid hex color string: {}", res.unwrapErr());
        }
        return Ok(res.unwrap());
    }
    return Err("Expected color to be array, object or hex string");
}
matjson::Value matjson::Serialize<ccColor3B>::toJson(cocos2d::ccColor3B const& value) {
    return matjson::makeObject({
        { "r", value.r },
        { "g", value.g },
        { "b", value.b }
    });
}

Result<cocos2d::ccColor4B, std::string> matjson::Serialize<ccColor4B>::fromJson(matjson::Value const& value) {
    if (value.isArray()) {
        auto arr = GEODE_UNWRAP(value.asArray());
        if (arr.size() == 4) {
            auto r = GEODE_UNWRAP(arr[0].asInt());
            auto g = GEODE_UNWRAP(arr[1].asInt());
            auto b = GEODE_UNWRAP(arr[2].asInt());
            auto a = GEODE_UNWRAP(arr[3].asInt());
            return Ok(cocos2d::ccc4(r, g, b, a));
        }
        return Err("Expected color array to have 4 items");
    }
    if (value.isObject()) {
        auto r = GEODE_UNWRAP(GEODE_UNWRAP(value.get("r")).asInt());
        auto g = GEODE_UNWRAP(GEODE_UNWRAP(value.get("g")).asInt());
        auto b = GEODE_UNWRAP(GEODE_UNWRAP(value.get("b")).asInt());
        auto a = GEODE_UNWRAP(GEODE_UNWRAP(value.get("a")).asInt());
        return Ok(cocos2d::ccc4(r, g, b, a));
    }
    if (value.isString()) {
        auto hex = GEODE_UNWRAP(value.asString());
        auto res = cc4bFromHexString(hex);
        if (!res) {
            return Err("Invalid hex color string: {}", res.unwrapErr());
        }
        return Ok(res.unwrap());
    }
    return Err("Expected color to be array, object or hex string");
}

matjson::Value matjson::Serialize<ccColor4B>::toJson(cocos2d::ccColor4B const& value) {
    return matjson::makeObject({
        { "r", value.r },
        { "g", value.g },
        { "b", value.b },
        { "a", value.a }
    });
}

Result<ccColor3B> geode::cocos::cc3bFromHexString(std::string const& rawHexValue, bool permissive) {
    if (permissive && rawHexValue.empty()) {
        return Ok(ccc3(255, 255, 255));
    }
    auto hexValue = rawHexValue;
    if (hexValue[0] == '#') {
        hexValue.erase(hexValue.begin());
    }
    if (hexValue.size() > 6) {
        return Err("Hex value too large");
    }
    auto res = numFromString<uint32_t>(hexValue, 16);
    if (!res) {
        return Err("Invalid hex value '{}'", hexValue);
    }
    auto numValue = res.unwrap();
    switch (hexValue.size()) {
        case 6: {
            auto r = static_cast<uint8_t>((numValue & 0xFF0000) >> 16);
            auto g = static_cast<uint8_t>((numValue & 0x00FF00) >> 8);
            auto b = static_cast<uint8_t>((numValue & 0x0000FF));
            return Ok(ccc3(r, g, b));
        } break;

        case 3: {
            auto r = static_cast<uint8_t>(((numValue & 0xF00) >> 8) * 17);
            auto g = static_cast<uint8_t>(((numValue & 0x0F0) >> 4) * 17);
            auto b = static_cast<uint8_t>(((numValue & 0x00F)) * 17);
            return Ok(ccc3(r, g, b));
        } break;

        case 2: {
            if (!permissive) {
                return Err("Invalid hex pattern, expected RGB or RRGGBB");
            }
            auto num = static_cast<uint8_t>(numValue);
            return Ok(ccc3(num, num, num));
        } break;

        case 1: {
            if (!permissive) {
                return Err("Invalid hex pattern, expected RGB or RRGGBB");
            }
            auto num = static_cast<uint8_t>(numValue) * 17;
            return Ok(ccc3(num, num, num));
        } break;

        default: {
            if (permissive) {
                return Err("Invalid hex pattern, expected R, RR, RGB, or RRGGBB");
            }
            else {
                return Err("Invalid hex pattern, expected RGB or RRGGBB");
            }
        }
    }
}

Result<ccColor4B> geode::cocos::cc4bFromHexString(std::string const& rawHexValue, bool requireAlpha, bool permissive) {
    if (permissive && rawHexValue.empty()) {
        return Ok(ccc4(255, 255, 255, 255));
    }
    auto hexValue = rawHexValue;
    if (hexValue[0] == '#') {
        hexValue.erase(hexValue.begin());
    }
    if (hexValue.size() > 8) {
        return Err("Hex value too large");
    }
    auto res = numFromString<uint32_t>(hexValue, 16);
    if (!res) {
        return Err("Invalid hex value '{}'", hexValue);
    }
    auto numValue = res.unwrap();
    switch (hexValue.size()) {
        case 8: {
            auto r = static_cast<uint8_t>((numValue & 0xFF000000) >> 24);
            auto g = static_cast<uint8_t>((numValue & 0x00FF0000) >> 16);
            auto b = static_cast<uint8_t>((numValue & 0x0000FF00) >> 8);
            auto a = static_cast<uint8_t>((numValue & 0x000000FF));
            return Ok(ccc4(r, g, b, a));
        } break;

        case 6: {
            if (requireAlpha) {
                return Err("Alpha component is required, got only RRGGBB");
            }
            auto r = static_cast<uint8_t>((numValue & 0xFF0000) >> 16);
            auto g = static_cast<uint8_t>((numValue & 0x00FF00) >> 8);
            auto b = static_cast<uint8_t>((numValue & 0x0000FF));
            return Ok(ccc4(r, g, b, 255));
        } break;

        case 4: {
            auto r = static_cast<uint8_t>(((numValue & 0xF000) >> 12) * 17);
            auto g = static_cast<uint8_t>(((numValue & 0x0F00) >> 8) * 17);
            auto b = static_cast<uint8_t>(((numValue & 0x00F0) >> 4) * 17);
            auto a = static_cast<uint8_t>(((numValue & 0x000F)) * 17);
            return Ok(ccc4(r, g, b, a));
        } break;

        case 3: {
            if (requireAlpha) {
                return Err("Alpha component is required, got only RGB");
            }
            auto r = static_cast<uint8_t>(((numValue & 0xF00) >> 8) * 17);
            auto g = static_cast<uint8_t>(((numValue & 0x0F0) >> 4) * 17);
            auto b = static_cast<uint8_t>(((numValue & 0x00F)) * 17);
            return Ok(ccc4(r, g, b, 255));
        } break;

        case 2: {
            if (!permissive) {
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA");
            }
            if (requireAlpha) {
                return Err("Alpha component is required, specify full RRGGBBAA");
            }
            auto num = static_cast<uint8_t>(numValue);
            return Ok(ccc4(num, num, num, 255));
        } break;

        case 1: {
            if (!permissive) {
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA");
            }
            if (requireAlpha) {
                return Err("Alpha component is required, specify full RGBA");
            }
            auto num = static_cast<uint8_t>(numValue) * 17;
            return Ok(ccc4(num, num, num, 255));
        } break;

        default: {
            if (requireAlpha) {
                return Err("Invalid hex pattern, expected RGBA or RRGGBBAA");
            }
            else if (permissive) {
                return Err("Invalid hex pattern, expected R, RR, RGB, RGBA, RRGGBB, or RRGGBBAA");
            }
            else {
                return Err("Invalid hex pattern, expected RGB, RGBA, RRGGBB, or RRGGBBAA");
            }
        }
    }
}

std::string geode::cocos::cc3bToHexString(ccColor3B const& color) {
    static constexpr auto digits = "0123456789ABCDEF";
    std::string output;
    output += digits[color.r >> 4 & 0xF];
    output += digits[color.r & 0xF];
    output += digits[color.g >> 4 & 0xF];
    output += digits[color.g & 0xF];
    output += digits[color.b >> 4 & 0xF];
    output += digits[color.b & 0xF];
    return output;
}

std::string geode::cocos::cc4bToHexString(ccColor4B const& color) {
    static constexpr auto digits = "0123456789ABCDEF";
    std::string output;
    output += digits[color.r >> 4 & 0xF];
    output += digits[color.r & 0xF];
    output += digits[color.g >> 4 & 0xF];
    output += digits[color.g & 0xF];
    output += digits[color.b >> 4 & 0xF];
    output += digits[color.b & 0xF];
    output += digits[color.a >> 4 & 0xF];
    output += digits[color.a & 0xF];
    return output;
}

bool WeakRefController::isManaged() {
    WeakRefPool::get()->check(m_obj);
    return m_obj;
}

void WeakRefController::swap(CCObject* other) {
    WeakRefPool::get()->check(m_obj);
    m_obj = other;
    WeakRefPool::get()->check(m_obj);
}

CCObject* WeakRefController::get() const {
    return m_obj;
}

WeakRefPool* WeakRefPool::get() {
    static auto inst = new WeakRefPool();
    return inst;
}

void WeakRefPool::check(CCObject* obj) {
    // if this object's only reference is the WeakRefPool aka only weak
    // references exist to it, then release it
    if (obj && obj->retainCount() == 1) {
        this->forget(obj);
    }
}

void WeakRefPool::forget(CCObject* obj) {
    if (!obj || !m_pool.contains(obj)) {
        return;
    }

    // set delegates to null because those aren't retained!
    if (auto input = typeinfo_cast<CCTextInputNode*>(obj)) {
        input->m_delegate = nullptr;
    }

    obj->release();
    // log::info("nullify {}", m_pool.at(obj).get());
    m_pool.at(obj)->m_obj = nullptr;
    m_pool.erase(obj);
}

std::shared_ptr<WeakRefController> WeakRefPool::manage(CCObject* obj) {
    if (!obj) {
        return std::shared_ptr<WeakRefController>();
    }

    if (!m_pool.contains(obj)) {
        obj->retain();
        auto controller = std::make_shared<WeakRefController>();
        controller->m_obj = obj;
        m_pool.insert({ obj, controller });
    }
    // log::info("get {} for {}", m_pool.at(obj).get(), obj);
    return m_pool.at(obj);
}

bool geode::cocos::isSpriteFrameName(CCNode* node, const char* name) {
    if (!node) return false;

    auto cache = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(name);
    if (!cache) return false;

    auto* texture = cache->getTexture();
    auto rect = cache->getRect();

    if (auto* spr = typeinfo_cast<CCSprite*>(node)) {
        if (spr->getTexture() == texture && spr->getTextureRect() == rect) {
            return true;
        }
    } else if (auto* btn = typeinfo_cast<CCMenuItemSprite*>(node)) {
        auto* img = btn->getNormalImage();
        if (auto* spr = typeinfo_cast<CCSprite*>(img)) {
            if (spr->getTexture() == texture && spr->getTextureRect() == rect) {
                return true;
            }
        }
    }
    return false;
}

CCNode* geode::cocos::getChildBySpriteFrameName(CCNode* parent, const char* name) {
    for (auto child : CCArrayExt<CCNode*>(parent->getChildren())) {
        if (::isSpriteFrameName(static_cast<CCNode*>(child), name)) {
            return child;
        }
    }
    return nullptr;
}

bool geode::cocos::isSpriteName(CCNode* node, const char* name) {
    if (!node) return false;

    auto texture = CCTextureCache::sharedTextureCache()->textureForKey(name);
    if (!texture) return false;

    if (auto* spr = typeinfo_cast<CCSprite*>(node)) {
        if (spr->getTexture() == texture) {
            return true;
        }
    }
    else if (auto* btn = typeinfo_cast<CCMenuItemSprite*>(node)) {
        auto* img = btn->getNormalImage();
        if (auto* spr = typeinfo_cast<CCSprite*>(img)) {
            if (spr->getTexture() == texture) {
                return true;
            }
        }
    }
    return false;
}

CCNode* geode::cocos::getChildBySpriteName(CCNode* parent, const char* name) {
    for (auto child : CCArrayExt<CCNode*>(parent->getChildren())) {
        if (::isSpriteName(static_cast<CCNode*>(child), name)) {
            return child;
        }
    }
    return nullptr;
}

CCRect geode::cocos::calculateNodeCoverage(std::vector<CCNode*> const& nodes) {
    CCRect coverage;
    for (auto child : nodes) {
        auto pos = child->getPosition() - child->getScaledContentSize() * child->getAnchorPoint();
        auto csize = child->getPosition() +
            child->getScaledContentSize() * (CCPoint{1.f, 1.f} - child->getAnchorPoint());
        if (pos.x < coverage.origin.x) {
            coverage.origin.x = pos.x;
        }
        if (pos.y < coverage.origin.y) {
            coverage.origin.y = pos.y;
        }
        if (csize.x > coverage.size.width) {
            coverage.size.width = csize.x;
        }
        if (csize.y > coverage.size.height) {
            coverage.size.height = csize.y;
        }
    }
    return coverage;
}

CCRect geode::cocos::calculateNodeCoverage(CCArray* nodes) {
    CCRect coverage;
    for (auto child : CCArrayExt<CCNode*>(nodes)) {
        auto pos = child->getPosition() - child->getScaledContentSize() * child->getAnchorPoint();
        auto csize = child->getPosition() +
            child->getScaledContentSize() * (CCPoint{1.f, 1.f} - child->getAnchorPoint());
        if (pos.x < coverage.origin.x) {
            coverage.origin.x = pos.x;
        }
        if (pos.y < coverage.origin.y) {
            coverage.origin.y = pos.y;
        }
        if (csize.x > coverage.size.width) {
            coverage.size.width = csize.x;
        }
        if (csize.y > coverage.size.height) {
            coverage.size.height = csize.y;
        }
    }
    return coverage;
}

CCRect geode::cocos::calculateChildCoverage(CCNode* parent) {
    return calculateNodeCoverage(parent->getChildren());
}

void geode::cocos::limitNodeSize(CCNode* spr, CCSize const& size, float def, float min) {
    spr->setScale(std::clamp(std::min(size.height / spr->getContentHeight(), size.width / spr->getContentWidth()), min, def));
}

void geode::cocos::limitNodeWidth(CCNode* spr, float width, float def, float min) {
    spr->setScale(std::clamp(width / spr->getContentSize().width, min, def));
}

void geode::cocos::limitNodeHeight(CCNode* spr, float height, float def, float min) {
    spr->setScale(std::clamp(height / spr->getContentSize().height, min, def));
}

bool geode::cocos::nodeIsVisible(CCNode* node) {
    if (!node->isVisible()) {
        return false;
    }
    if (auto parent = node->getParent()) {
        return nodeIsVisible(parent);
    }
    return true;
}

CCNode* geode::cocos::getChildByTagRecursive(cocos2d::CCNode* node, int tag) {
    if (node->getTag() == tag) return node;
    auto children = node->getChildren();
    for (int i = 0; i < children->count(); ++i) {
        auto child = getChildByTagRecursive((cocos2d::CCNode*)children->objectAtIndex(i), tag);
        if (child) return child;
    }

    return nullptr;
}

bool geode::cocos::fileExistsInSearchPaths(char const* filename) {
    auto utils = CCFileUtils::sharedFileUtils();
    return utils->isFileExist(utils->fullPathForFilename(filename, false));
}

CCScene* geode::cocos::switchToScene(CCLayer* layer) {
    auto scene = CCScene::create();
    scene->addChild(layer);
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, scene));
    return scene;
}

static CreateLayerFunc LOADING_FINISHED_SCENE = nullptr;

void geode::cocos::reloadTextures(CreateLayerFunc returnTo) {
    LOADING_FINISHED_SCENE = returnTo;
    GameManager::get()->reloadAll(false, false, true);
}

void GEODE_DLL geode::cocos::handleTouchPriorityWith(cocos2d::CCNode* node, int priority, bool force) {
    if (node == nullptr) return;
    if (node->getChildrenCount() == 0) return;

    for (auto child : CCArrayExt<CCNode*>(node->getChildren())) {
        if (auto delegate = typeinfo_cast<CCTouchDelegate*>(child)) {
            if (auto handler = CCTouchDispatcher::get()->findHandler(delegate)) {
                if (!force && handler->m_nPriority < priority) {
                    handleTouchPriorityWith(child, handler->m_nPriority - 1, force);
                    continue;
                }
                else {
                    CCTouchDispatcher::get()->setPriority(priority, delegate);
                }
            }
        }
        handleTouchPriorityWith(child, priority, force);
    }
}
void GEODE_DLL geode::cocos::handleTouchPriority(cocos2d::CCNode* node, bool force) {
    Loader::get()->queueInMainThread([node = Ref(node), force]() {
        if (auto delegate = typeinfo_cast<CCTouchDelegate*>(*node)) {
            if (auto handler = CCTouchDispatcher::get()->findHandler(delegate)) {
                return handleTouchPriorityWith(node, handler->m_nPriority - 1, force);
            }
        }
        handleTouchPriorityWith(node, 0, force);
    });
}

struct LoadingFinished : Modify<LoadingFinished, LoadingLayer> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("geode::cocos::reloadTextures disabled")
    void loadAssets() {
        // loadFinished is inlined on Macchew OS :sob:

        if (m_loadStep < 14) {
            return LoadingLayer::loadAssets();
        }

        // Default behaviour
        if (!LOADING_FINISHED_SCENE) {
            return LoadingLayer::loadAssets();
        }
        // Create custom layer
        auto layer = LOADING_FINISHED_SCENE();
        // If failed, default behaviour
        if (!layer) {
            return LoadingLayer::loadAssets();
        }
        auto scene = CCScene::create();
        scene->addChild(layer);
        // TODO: readd this
        // AppDelegate::get()->m_runningScene = scene;
        CCDirector::get()->replaceScene(scene);
        // Don't overwrite behaviour next time
        LOADING_FINISHED_SCENE = nullptr;
    }
};
