#include <Geode/Geode.hpp>
#include <Geode/modify/CCConfiguration.hpp>

using namespace geode::prelude;

class $modify(CCConfigurationFix, CCConfiguration) {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("CCConfiguration fix")

    void gatherGPUInfo() {
        CCConfiguration::gatherGPUInfo();

        static std::string extensions;
        if (m_pGlExtensions) {
            extensions = m_pGlExtensions;
        }

        m_pGlExtensions = extensions.data();
    }
};