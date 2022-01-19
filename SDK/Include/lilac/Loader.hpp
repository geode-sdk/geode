#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>
#include <helpers/Result.hpp>

class Lilac;

namespace lilac {
    #pragma warning(disable: 4251)

    static constexpr const std::string_view lilac_directory          = "lilac";
    static constexpr const std::string_view lilac_mod_directory      = "mods";
    static constexpr const std::string_view lilac_resource_directory = "resources";
    static constexpr const std::string_view lilac_temp_directory     = "temp";
    static constexpr const std::string_view lilac_mod_extension      = ".lilac";

    class Mod;
    class Hook;
    class LogStream;
    class LogMessage;
    struct ModInfo;

    class LILAC_DLL Loader {
    protected:
        std::unordered_map<std::string, Mod*> m_mods;
        std::vector<LogMessage*> m_logs;
        LogStream* m_logStream;
        bool m_isSetup = false;

        /**
         * Lowest supported mod version.
         * Any mod targeting a lilac version 
         * lower than this will not be loaded, 
         * as they will be considered out-of-date.
         */
        static constexpr const int s_supportedSchemaMin = 1;
        /**
         * Highest support mod version.
         * Any mod targeting a lilac version 
         * higher than this will not be loaded, 
         * as a higher version means that 
         * the user's lilac is out-of-date, 
         * or that the user is a time traveller 
         * and has downloaded a mod from the 
         * future.
         */
        static constexpr const int s_supportedSchemaMax = 1;

        /**
         * This function is to avoid ridiculous 
         * indentation in `checkMetaInformation`
         * 
         * The json parameter is void* because 
         * I don't want to force-include json 
         * for every lilac user and forward-
         * declaring the template-full basic_json 
         * class looks horrifying
         * 
         * This function is only used in one place 
         * anyway, only by me who knows how to 
         * use it, so who cares
         */
        template<int Schema>
        Result<Mod*> checkBySchema(std::string const& path, void* json);

        Result<std::string> createTempDirectoryForMod(ModInfo const& info);
        Result<Mod*> loadModFromFile(std::string const& file);
        void createDirectories();

        void updateAllDependencies();

        friend class Mod;
        friend class CustomLoader;
        friend class Lilac;
        friend struct ModInfo;
        
    public:
        Loader();
        virtual ~Loader();

        static Loader* get();
        bool setup();
        size_t updateMods();

        LogStream& logStream();
        void log(LogMessage* log);
        void deleteLog(LogMessage* log);
        std::vector<LogMessage*> const& getLogs() const;
        std::vector<LogMessage*> getLogs(
            std::initializer_list<Severity> severityFilter
        );

        bool isModLoaded(std::string const& id, bool resolved = true) const;
        Mod* getLoadedMod(std::string const& id, bool resolved = true) const;
        std::vector<Mod*> getLoadedMods() const;
        void unloadMod(Mod* mod);

        void queueInGDThread(std::function<void(void)> func);
    };

}
