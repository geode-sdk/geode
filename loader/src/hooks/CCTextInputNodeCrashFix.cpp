#include <Geode/Geode.hpp>
#include <Geode/modify/CCTextInputNode.hpp>

using namespace geode::prelude;

/// CCTextInputNode uses CCTextFieldDelegate, which is handled in Ctor and Dtor rather than onEnter
/// and onExit. Due to that, it means it can still call methods when it is no longer part of the
/// scene tree. If m_delegate is set to some node that is destroyed but CCTextInputNode survives,
/// even for one frame and is then destroyed, then onTextFieldDetachWithIME will be called with an
/// invalid m_delegate. This can cause a crash. An example that can cause this crash sometimes is
/// focusing a CCTextInputNode and closing the game.
/// 
/// Since controlling the lifetime of CCTextFieldDelegate isn't an option here, a more hacky fix is
/// to set m_delegate to nullptr when the node is exited, and restore it if it does get entered
/// again. This is to ensure that nothing is ever called on an invalid m_delegate if the
/// CCTextInputNode's lifetime is longer than it. This can change functionality if you rely on a
/// CCTextInputNode that is not in the node tree, but who is going to do that, right?

struct CCTextInputNodeCrashFix : Modify<CCTextInputNodeCrashFix, CCTextInputNode> {
    struct Fields {
        TextInputDelegate* m_delegate = nullptr;
    };

    bool init(float width, float height, char const* placeholder, char const* textFont, int fontSize, char const* labelFont) {
        auto fields = m_fields.self();

        addOnEnterCallback([this, fields] {
            if (m_delegate && fields->m_delegate != m_delegate) {
                fields->m_delegate = m_delegate;
            }
            m_delegate = fields->m_delegate;
        });

        addOnExitCallback([this, fields] {
            fields->m_delegate = m_delegate;
            m_delegate = nullptr;
        });

        return CCTextInputNode::init(width, height, placeholder, textFont, fontSize, labelFont);
    }
};