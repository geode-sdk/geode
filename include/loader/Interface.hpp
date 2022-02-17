#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <vector>
#include <utils/Result.hpp>
#include "Log.hpp"
#include "Mod.hpp"

namespace geode {
	class Mod;
	class Hook;

	namespace log {
		class LogPtr;
	}

	/**
	 * For developing your own mods, it is 
	 * often convenient to be able to do things 
	 * like create hooks using statically 
	 * initialized global classes.
	 * 
	 * At that point however, your mod has not 
	 * yet received the Mod* to create hooks 
	 * through.
	 * 
	 * For situations like that, you can instead 
	 * inherit from Interface to create your own 
	 * mod interface and create hooks through that; 
	 * calling `init` with the Mod* you receive 
	 * from geode will automatically create all 
	 * scheduled hooks, logs, etc.
	 * 
	 * Interface also provides a handy & 
	 * standardized way to store access to your 
	 * Mod*; you can just define a `get` function 
	 * and a getter for the Mod* stored in the 
	 * Interface.
	 * 
	 * @class Interface
	 */
	class Interface {
	protected:
		struct ScheduledHook {
			std::string_view m_displayName;
			void* m_address;
			void* m_detour;
		};

		struct ScheduledLog {
			std::string m_info;
			Severity m_severity;
		};

		Mod* m_mod = nullptr;
		std::vector<ScheduledHook> m_scheduledHooks;
		std::vector<ScheduledLog> m_scheduledLogs;
	
	public:

		static inline GEODE_HIDDEN Interface* get() {
			static Interface ret;
			return &ret;
		}

		static inline GEODE_HIDDEN Mod* mod() {
			return Interface::get()->m_mod;
		}

		GEODE_DLL void init(Mod*);

        /**
         * Create a hook at an address. This function can 
		 * be used at static initialization time, as it 
		 * doesn't require the Mod* to be set -- it will 
		 * create the hooks later when the Mod* is set.
		 * 
         * @param address The absolute address of 
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
		 * 
         * @returns Successful result containing the 
         * Hook handle (or nullptr if Mod* is not loaded 
		 * yet), errorful result with info on error
         */
        GEODE_DLL Result<Hook*> addHook(void* address, void* detour);

        /**
         * The same as addHook(void*, void*), but also provides 
         * a display name to show it in the list of the loader.
         * Mostly for internal use but if you don't like your
         * hooks showing up like base + 0x123456 it can be useful
         */
        GEODE_DLL Result<Hook*> addHook(std::string_view displayName, void* address, void* detour);

        /**
         * Log an information. Equivalent to 
         * ```
         * Mod::log() << Severity::severity << info << geode::endl.
         * ```
         * @param info Log information
         * @param severity Log severity
         */
        GEODE_DLL void logInfo(std::string const& info, Severity severity);
	};

    inline const char* operator"" _sprite(const char* str, size_t) {
        return Interface::mod()->expandSpriteName(str);
    }
}
