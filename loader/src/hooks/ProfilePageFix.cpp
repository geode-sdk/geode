#include <Geode/modify/ProfilePage.hpp>
#include <Geode/binding/GJUserScore.hpp>

using namespace geode::prelude;

struct CustomProfilePage : Modify<CustomProfilePage, ProfilePage> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("ProfilePage Fix")

    TodoReturn getUserInfoFinished(GJUserScore* info) override {
        m_usernameLabel->setString(info->m_userName);
        m_usernameLabel->limitLabelWidth(info->m_modBadge > 0 ? 140.0f : 160.0f, 0.8f, 0.0f);
        return ProfilePage::getUserInfoFinished(info);
    }
};
