#include <Geode/modify/LoadingLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

void cocos2d::to_json(nlohmann::json& json, ccColor3B const& color) {
    json = nlohmann::json{
        {"r", color.r},
        {"g", color.g},
        {"b", color.b},
    };
}

void cocos2d::from_json(nlohmann::json const& json, ccColor3B& color) {
    // array
    if (json.is_array()) {
        if (json.size() == 3) {
            json.at(0).get_to(color.r);
            json.at(1).get_to(color.g);
            json.at(2).get_to(color.b);
        }
        else {
            throw nlohmann::json::type_error::create(0, "Expected color array to have 3 items", json);
        }
    }
    // object
    else if (json.is_object()) {
        json.at("r").get_to(color.r);
        json.at("g").get_to(color.g);
        json.at("b").get_to(color.b);
    }
    // hex string
    else if (json.is_string()) {
        std::string str = json;
        if (str[0] == '#') {
            str.erase(str.begin());
        }
        if (str.size() > 6) {
            throw nlohmann::json::type_error::create(0, "Hex string for color too long", json);
        }
        auto c = cc3bFromHexString(str);
        if (!c) {
            throw nlohmann::json::type_error::create(
                0, "Invalid color hex string: " + c.unwrapErr(), json
            );
        }
        color = c.unwrap();
    }
    // bad
    else {
        throw nlohmann::json::type_error::create(
            0, "Expected color to be array, object or hex string", json
        );
    }
}

void cocos2d::to_json(nlohmann::json& json, ccColor4B const& color) {
    json = nlohmann::json{
        {"r", color.r},
        {"g", color.g},
        {"b", color.b},
        {"a", color.a},
    };
}

void cocos2d::from_json(nlohmann::json const& json, ccColor4B& color) {
    // array
    if (json.is_array()) {
        if (json.size() == 4) {
            json.at(0).get_to(color.r);
            json.at(1).get_to(color.g);
            json.at(2).get_to(color.b);
            json.at(3).get_to(color.a);
        }
        else {
            throw nlohmann::json::type_error::create(0, "Expected color array to have 4 items", json);
        }
    }
    // object
    else if (json.is_object()) {
        json.at("r").get_to(color.r);
        json.at("g").get_to(color.g);
        json.at("b").get_to(color.b);
        json.at("a").get_to(color.a);
    }
    // hex string
    else if (json.is_string()) {
        std::string str = json;
        if (str[0] == '#') {
            str.erase(str.begin());
        }
        if (str.size() > 8) {
            throw nlohmann::json::type_error::create(0, "Hex string for color too long", json);
        }
        auto c = cc4bFromHexString(str);
        if (!c) {
            throw nlohmann::json::type_error::create(
                0, "Invalid color hex string: " + c.unwrapErr(), json
            );
        }
        color = c.unwrap();
    }
    // bad
    else {
        throw nlohmann::json::type_error::create(
            0, "Expected color to be array, object or hex string", json
        );
    }
}

Result<ccColor3B> geode::cocos::cc3bFromHexString(std::string const& hexValue) {
    if (hexValue.empty()) {
        return Ok(ccc3(255, 255, 255));
    }
    if (hexValue.size() > 6) {
        return Err("Hex value too large");
    }
    int numValue;
    try {
        numValue = std::stoi(hexValue, 0, 16);
    }
    catch (...) {
        return Err("Invalid hex value");
    }
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
            auto num = static_cast<uint8_t>(numValue);
            return Ok(ccc3(num, num, num));
        } break;

        case 1: {
            auto num = static_cast<uint8_t>(numValue) * 17;
            return Ok(ccc3(num, num, num));
        } break;

        default: return Err("Invalid hex size, expected 1, 2, 3, or 6");
    }
}

Result<ccColor4B> geode::cocos::cc4bFromHexString(std::string const& hexValue) {
    if (hexValue.empty()) {
        return Ok(ccc4(255, 255, 255, 255));
    }
    if (hexValue.size() > 8) {
        return Err("Hex value too large");
    }
    int numValue;
    try {
        numValue = std::stoi(hexValue, 0, 16);
    }
    catch (...) {
        return Err("Invalid hex value");
    }
    switch (hexValue.size()) {
        case 8: {
            auto r = static_cast<uint8_t>((numValue & 0xFF000000) >> 24);
            auto g = static_cast<uint8_t>((numValue & 0x00FF0000) >> 16);
            auto b = static_cast<uint8_t>((numValue & 0x0000FF00) >> 8);
            auto a = static_cast<uint8_t>((numValue & 0x000000FF));
            return Ok(ccc4(r, g, b, a));
        } break;

        case 6: {
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
            auto r = static_cast<uint8_t>(((numValue & 0xF00) >> 8) * 17);
            auto g = static_cast<uint8_t>(((numValue & 0x0F0) >> 4) * 17);
            auto b = static_cast<uint8_t>(((numValue & 0x00F)) * 17);
            return Ok(ccc4(r, g, b, 255));
        } break;

        case 2: {
            auto num = static_cast<uint8_t>(numValue);
            return Ok(ccc4(num, num, num, 255));
        } break;

        case 1: {
            auto num = static_cast<uint8_t>(numValue) * 17;
            return Ok(ccc4(num, num, num, 255));
        } break;

        default: return Err("Invalid hex size, expected 1, 2, 3, 4, 6, or 8");
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
    for (auto child : CCArrayExt<CCNode>(nodes)) {
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

void geode::cocos::limitNodeSize(cocos2d::CCNode* spr, cocos2d::CCSize const& size, float def, float min) {
    spr->setScale(1.f);
    auto [cwidth, cheight] = spr->getContentSize();

    float scale = def;
    if (size.height && size.height < cheight) {
        scale = size.height / cheight;
    }
    if (size.width && size.width < cwidth) {
        if (size.width / cwidth < scale) scale = size.width / cwidth;
    }
    if (def && def < scale) {
        scale = def;
    }
    if (min && scale < min) {
        scale = min;
    }
    spr->setScale(scale);
}

bool geode::cocos::nodeIsVisible(cocos2d::CCNode* node) {
    if (!node->isVisible()) return false;
    if (node->getParent()) return nodeIsVisible(node->getParent());
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

struct LoadingFinished : Modify<LoadingFinished, LoadingLayer> {
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
        AppDelegate::get()->m_runningScene = scene;
        CCDirector::get()->replaceScene(scene);
        // Don't overwrite behaviour next time
        LOADING_FINISHED_SCENE = nullptr;
    }
};
