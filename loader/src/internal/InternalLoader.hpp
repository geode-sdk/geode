#pragma once

#include <mutex>
#include <unordered_map>
#include <vector>
#include "FileWatcher.hpp"
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/Result.hpp>
#include <unordered_set>
#include <Geode/utils/json.hpp>
#include <optional>
#include "../index/Index.hpp"

USE_GEODE_NAMESPACE();

/**
 * Internal extension of Loader for private information
 * @class InternalLoader
 */
class InternalLoader : public Loader {
protected:
	std::vector<std::function<void(void)>> m_gdThreadQueue;
	mutable std::mutex m_gdThreadMutex;
	bool m_platformConsoleOpen = false;
	std::unordered_set<std::string> m_shownInfoAlerts;

	void saveInfoAlerts(nlohmann::json& json);
	void loadInfoAlerts(nlohmann::json& json);

	void downloadLoaderResources(IndexUpdateCallback callback);

	InternalLoader();
	~InternalLoader();

	friend class Loader;

public:
	static InternalLoader* get();

	bool setup();

	bool loadHooks();

	/**
	 * Check if a one-time event has been shown to the user, 
	 * and set it to true if not. Will return the previous 
	 * state of the event before setting it to true
	 */
	bool shownInfoAlert(std::string const& key);

	void queueInGDThread(ScheduledFunction func);
	void executeGDThreadQueue();

	void logConsoleMessage(std::string const& msg);
	bool platformConsoleOpen() const;
	void openPlatformConsole();
	void closePlatformConsole();
	static void platformMessageBox(const char* title, std::string const& info);

	bool verifyLoaderResources(IndexUpdateCallback callback);
	
	friend int geodeEntry(void* platformData);
};
