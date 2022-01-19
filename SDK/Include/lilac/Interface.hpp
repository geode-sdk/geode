#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <vector>
#include <helpers/Result.hpp>

namespace lilac {
	class Mod;
	class Hook;
	class LogMessage;

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
	 * from lilac will automatically create all 
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
	class LILAC_DLL Interface {
	protected:
		struct ScheduledHook {
			void* m_address;
			void* m_detour;
		};

		Mod* m_mod = nullptr;
		std::vector<ScheduledHook> m_scheduledHooks;
		std::vector<LogMessage*> m_scheduledLogs;
	
	public:
		void init(Mod*);

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
        Result<Hook*> addHook(void* address, void* detour);

        /**
         * Throw an error. Equivalent to 
         * ```
         * Mod::log() << Severity::severity << info << lilac::endl.
         * ```
         * @param info Error infomration
         * @param severity Error severity
         */
        void throwError(
            std::string const& info,
            Severity severity
        );
	};
}
