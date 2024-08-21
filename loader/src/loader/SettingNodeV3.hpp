#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TitleSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<TitleSettingV3> setting, float width);

    void onCommit() override;

public:
    static TitleSettingNodeV3* create(std::shared_ptr<TitleSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<TitleSettingV3> getSetting() const;
};

class BoolSettingNodeV3 : public SettingNodeV3 {
protected:
    CCMenuItemToggler* m_toggle;

    bool init(std::shared_ptr<BoolSettingV3> setting, float width);

    void onCommit() override;

    void onToggle(CCObject*);

public:
    static BoolSettingNodeV3* create(std::shared_ptr<BoolSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<BoolSettingV3> getSetting() const;
};

class IntSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<IntSettingV3> setting, float width);

    void onCommit() override;

public:
    static IntSettingNodeV3* create(std::shared_ptr<IntSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<IntSettingV3> getSetting() const;
};

class FloatSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<FloatSettingV3> setting, float width);

    void onCommit() override;

public:
    static FloatSettingNodeV3* create(std::shared_ptr<FloatSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<FloatSettingV3> getSetting() const;
};

class StringSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<StringSettingV3> setting, float width);

    void onCommit() override;

public:
    static StringSettingNodeV3* create(std::shared_ptr<StringSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<StringSettingV3> getSetting() const;
};

class FileSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<FileSettingV3> setting, float width);

    void onCommit() override;

public:
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<FileSettingV3> getSetting() const;
};

class Color3BSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<Color3BSettingV3> setting, float width);

    void onCommit() override;

public:
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<Color3BSettingV3> getSetting() const;
};

class Color4BSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<Color4BSettingV3> setting, float width);

    void onCommit() override;

public:
    static Color4BSettingNodeV3* create(std::shared_ptr<Color4BSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<Color4BSettingV3> getSetting() const;
};

class UnresolvedCustomSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<LegacyCustomSettingV3> setting, float width);

    void onCommit() override;

public:
    static UnresolvedCustomSettingNodeV3* create(std::shared_ptr<LegacyCustomSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;

    std::shared_ptr<LegacyCustomSettingV3> getSetting() const;
};

// If these classes do get exposed in headers, this SHOULD NOT BE EXPOSED!!!!!! DO NOT DO THAT!!!!

class LegacyCustomSettingToV3Node : public SettingNodeV3 {
protected:
    SettingNode* m_original;

    bool init(std::shared_ptr<LegacyCustomSettingV3> original, float width);

    void onCommit() override;

public:
    static LegacyCustomSettingToV3Node* create(std::shared_ptr<LegacyCustomSettingV3> original, float width);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void resetToDefault() override;
};
