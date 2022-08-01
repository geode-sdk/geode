/*#include "ModSettingsList.hpp"
#include <settings/SettingNodeManager.hpp>
#include <settings/SettingNode.hpp>
#include <utils/WackyGeodeMacros.hpp>
#include "ModSettingsLayer.hpp"

bool ModSettingsList::init(Mod* mod, ModSettingsLayer* layer, float width, float height) {
	m_mod = mod;
	m_settingsLayer = layer;

	m_scrollLayer = ScrollLayer::create({ width, height });
	this->addChild(m_scrollLayer);

	if (mod->getSettings().size()) {
		float offset = 0.f;
		bool coloredBG = false;
		std::vector<CCNode*> gen;
		for (auto const& sett : mod->getSettings()) {
			auto node = SettingNodeManager::get()->generateNode(mod, sett, width);
			if (node) {
				m_settingNodes.push_back(node);
				node->m_list = this;
				if (coloredBG) {
					node->m_backgroundLayer->setColor({ 0, 0, 0 });
					node->m_backgroundLayer->setOpacity(50);
				}
				node->setPosition(
					0.f, offset - node->getScaledContentSize().height
				);
				m_scrollLayer->m_contentLayer->addChild(node);
				
				auto separator = CCLayerColor::create({ 0, 0, 0, 50 }, width, 1.f);
				separator->setPosition(0.f, offset - .5f);
				m_scrollLayer->m_contentLayer->addChild(separator);
				gen.push_back(separator);

				offset -= node->m_height;
				coloredBG = !coloredBG;
				gen.push_back(node);
			}
		}
		auto separator = CCLayerColor::create({ 0, 0, 0, 50 }, width, 1.f);
		separator->setPosition(0.f, offset);
		m_scrollLayer->m_contentLayer->addChild(separator);
		gen.push_back(separator);

		offset = -offset;
		// to avoid needing to do moveToTopWithOffset, 
		// just set the content size to the viewport 
		// size if its less
		if (offset < height) offset = height;
		for (auto& node : gen) {
			node->setPositionY(node->getPositionY() + offset);
		}
		m_scrollLayer->m_contentLayer->setContentSize({ width, offset });
		m_scrollLayer->moveToTop();
	} else {
		auto label = CCLabelBMFont::create("This mod has no settings", "bigFont.fnt");
		label->setPosition(width / 2, height / 2);
		label->setScale(.5f);
		m_scrollLayer->m_contentLayer->addChild(label);
		m_scrollLayer->setContentSize({ width, height });
		m_scrollLayer->moveToTop();
	}

	CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
	m_scrollLayer->registerWithTouchDispatcher();

	return true;
}

bool ModSettingsList::hasUnsavedModifiedSettings() const {
	for (auto& setting : m_settingNodes) {
		if (setting->hasUnsavedChanges()) {
			return true;
		}
	}
	return false;
}

void ModSettingsList::applyChanges() {
	for (auto& setting : m_settingNodes) {
		if (setting->hasUnsavedChanges()) {
			setting->commitChanges();
		}
	}
}

void ModSettingsList::updateList() {
	m_settingsLayer->updateState();
}

void ModSettingsList::resetAllToDefault() {
	for (auto& setting : m_settingNodes) {
		setting->resetToDefault();
	}
}

ModSettingsList* ModSettingsList::create(
	Mod* mod, ModSettingsLayer* layer, float width, float height
) {
	auto ret = new ModSettingsList();
	if (ret && ret->init(mod, layer, width, height)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}*/
