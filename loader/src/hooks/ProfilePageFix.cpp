#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/GJUserScore.hpp>

using namespace geode::prelude;

// Fixes the names not showing up on profile links
struct CustomProfilePage : Modify<CustomProfilePage, ProfilePage> {
    virtual TodoReturn getUserInfoFinished(GJUserScore* info) {
        ProfilePage::getUserInfoFinished(info);
        m_usernameLabel->setString(info->m_userName.c_str());
    }
};