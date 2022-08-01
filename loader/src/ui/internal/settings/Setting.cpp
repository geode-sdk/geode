// #include <settings/Setting.hpp>
// #include <unordered_map>

// USE_GEODE_NAMESPACE();

// Setting* Setting::from(std::string const& id, nlohmann::json const& json) {
// 	if (json.is_object()) {

// 	} else {

// 	}

// 	auto ctrl = json["control"];
// 	if (!ctrl.is_string()) {
// 		FLAlertLayer::create(
// 			"Failed to load settings",
// 			"JSON error: 'control' key is not a string (or doesn't exist)!",
// 			"OK"
// 		)->show();
// 		return nullptr;
// 	}
// 	std::string control = ctrl;

// 	Setting* out = nullptr;
// 	/*EventCenter::get()->broadcast(Event(
// 		events::getSetting(id),
// 		&out,
// 		Mod::get()
// 	));*/
// 	#pragma message("Event")

// 	if (out == nullptr) {
// 		FLAlertLayer::create(
// 			"Failed to load settings",
// 			std::string("No known setting control '") + control + "'",
// 			"OK"
// 		)->show();
// 	}

// 	return out;
// }

// bool SettingManager::hasSettings() {
// 	return !this->m_settings.empty();
// }

// SettingManager* SettingManager::with(Mod* m) {
//     static std::unordered_map<Mod*, SettingManager*> managers;
//     if (!managers.count(m)) {
//         managers[m] = new SettingManager(m);
// 	}
//     return managers[m];
// }

// SettingManager::SettingManager(Mod* m) {
// 	m_mod = m;
// 	auto root = m_mod->getDataStore()["settings"];

// 	if (!root.is_object()) {
// 		return;
// 	}

// 	for (auto [id, setting] : root.items()) {
// 		m_settings[id] = Setting::from(id, setting);
// 	}
// }

// Setting* SettingManager::getSetting(std::string id) {
// 	return m_settings[id];
// }

// void SettingManager::updateSetting(std::string id) {
// 	// m_mod->getDataStore()["settings"][id] = this->getSetting(id)->saveJSON();
// }

// CCNode* Setting::createControl() const {
// 	return nullptr;
// }

// std::vector<CCNode*> SettingManager::generateSettingNodes() {
// 	std::vector<CCNode*> out;
// 	for (auto [k, v] : this->m_settings) {
// 		out.push_back(v->createControl());
// 	}
// 	return out;
// }
