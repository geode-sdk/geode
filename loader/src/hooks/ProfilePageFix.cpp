#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/GJUserScore.hpp>

using namespace geode::prelude;

// Fixes the names not showing up on profile links
struct CustomProfilePage : Modify<CustomProfilePage, ProfilePage> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("ProfilePage fix")

    virtual TodoReturn getUserInfoFinished(GJUserScore* info) {
        ProfilePage::getUserInfoFinished(info);
        m_usernameLabel->setString(info->m_userName.c_str());
    }
};