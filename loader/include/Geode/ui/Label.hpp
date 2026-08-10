#pragma once

#include <cocos2d.h>
#include <Geode/utils/cocos.hpp>

namespace geode {
    /// A bitmap font cache that acts similarly to cocos2d::CCBMFontConfiguration, but is more performant.
    class GEODE_DLL BitmapFont final {
    public:
        BitmapFont();
        ~BitmapFont();

        /// Loads a bitmap font from a .fnt file. Caches for future reuse.
        /// @param fntFile File name of the .fnt file
        /// @return Pointer to the loaded bitmap font, or nullptr if failed
        static BitmapFont* load(ZStringView fntFile);

        /// Purges a bitmap font from the cache.
        /// @param fntFile File name of the .fnt file
        static void purgeFont(ZStringView fntFile);

        /// Purges a bitmap font from the cache.
        /// @param font Pointer to the bitmap font to purge
        static void purgeFont(BitmapFont const* font);

        /// Purges the entire font cache. Called automatically on textures reload.
        static void purgeAllFonts();

        using KerningPair = std::pair<uint32_t, uint32_t>;

        struct KerningValue {
            float amount;
            float scaled;
        };

        struct KerningPairHash {
            size_t operator()(KerningPair const& pair) const noexcept {
                return std::hash<uint64_t>()(static_cast<uint64_t>(pair.first) << 32 | pair.second);
            }
        };

        using KerningMap = std::unordered_map<KerningPair, KerningValue, KerningPairHash>;

        struct CharDef final {
            uint32_t codepoint = 0;
            cocos2d::CCRect rect{};
            float xOffset = 0.f;
            float yOffset = 0.f;
            float xAdvance = 0.f;

            /// Values pre-calculated with CCDirector::getContentScaleFactor()
            cocos2d::CCRect uv{};
            cocos2d::CCSize sizeScaled{};
            float xOffsetScaled = 0.f;
            float yOffsetScaled = 0.f;
            float xAdvanceScaled = 0.f;
        };

        struct Padding final {
            int32_t left = 0;
            int32_t right = 0;
            int32_t top = 0;
            int32_t bottom = 0;
        };

        /// Loads the font file from a given path.
        /// @param fntFile File name of the .fnt file
        /// @return True if successfully parsed, false otherwise
        bool initWithFile(ZStringView fntFile);

        /// Loads the font file using file contents.
        /// @param text Raw contents of the .fnt file
        /// @note Atlas path relies on knowing the font file path.
        /// Call setFontFile() separately to resolve the absolute path instead of storing the relative file name.
        /// @return True if successfully parsed, false otherwise
        bool initWithContents(std::string_view text);

        /// Initializes extra cached fields (dividing by content scale factor, texture UVs).
        /// @note Invoked automatically by BitmapFont::load(). Must be called manually
        /// if constructing a font via initWithFile() or initWithContents().
        void initBakedValues();

        void setFontFile(std::string file) noexcept;
        void setAtlasName(std::string name) noexcept;

        ZStringView getFontFile() const noexcept;
        ZStringView getAtlasName() const noexcept;

        std::unordered_map<uint32_t, CharDef> const& getCharDefs() const noexcept;
        KerningMap const& getKernings() const noexcept;
        Padding getPadding() const noexcept;
        cocos2d::CCSize getAtlasSize() const noexcept;
        float getCommonHeight() const noexcept;

        /// @return Same as getCommonHeight(), but divided by content scale factor.
        float getCommonHeightScaled() const noexcept;

        float getKerning(char32_t first, char32_t second) const noexcept;

    private:
        std::string m_atlasName;
        std::string m_fntFilename;
        std::unordered_map<uint32_t, CharDef> m_characters;
        KerningMap m_kerning;
        Padding m_padding{};
        cocos2d::CCSize m_atlasSize{};
        float m_commonHeight = 0.f;
        float m_commonHeightScaled = 0.f;
    };

    /// Container that stores mappings for emojis/custom nodes used by Label
    class GEODE_DLL EmojiRegistry {
    public:
        using NodeFactory = Function<cocos2d::CCNode*()>;
        using NodeFactoryParams = Function<cocos2d::CCNode*(std::u32string_view str, size_t& index)>;

        struct EmojiEntry final {
            ZStringView frameName;

            EmojiEntry(ZStringView frameName) noexcept;

            cocos2d::CCSpriteFrame* getFrame() const;
            void invalidate() noexcept;

        private:
            mutable cocos2d::CCSpriteFrame* cachedFrame = nullptr;
        };

        using Entry = std::variant<EmojiEntry, NodeFactory, NodeFactoryParams>;

        EmojiRegistry();
        ~EmojiRegistry();

        EmojiRegistry(EmojiRegistry const&);
        EmojiRegistry(EmojiRegistry&&) noexcept;
        EmojiRegistry& operator=(EmojiRegistry const&);
        EmojiRegistry& operator=(EmojiRegistry&&) noexcept;

        /// Inserts a new replacement mapping to the registry.
        /// @param sequence A UTF-32 view to a sequence that's going to be replaced by a custom node.
        /// @param factory Function that returns a newly created node.
        /// @note The sequence view is expected to point to static memory and never be freed.
        void insert(std::u32string_view sequence, NodeFactory factory);

        /// Inserts a new replacement mapping to the registry.
        /// @param sequence A UTF-32 view to a sequence that's going to be replaced by a custom node.
        /// @param factory Function that returns a newly created node.
        /// @note The sequence view is expected to point to static memory and never be freed.
        void insert(std::u32string_view sequence, NodeFactoryParams factory);

        /// Inserts a new replacement mapping to the registry.
        /// @param sequence A UTF-32 view to a sequence that's going to be replaced by a sprite.
        /// @param frameName Frame name for the provided emoji.
        /// @note The sequence view is expected to point to static memory and never be freed.
        void insert(std::u32string_view sequence, ZStringView frameName);

        /// Inserts a sequence of mappings to the registry.
        /// @note All sequence views are expected to point to static memory and never be freed.
        void insert(std::span<std::tuple<std::u32string_view, Entry>> entries);

        /// Removes an entry from the registry by provided sequence.
        void erase(std::u32string_view sequence);

        Entry* match(std::u32string_view str, size_t& index);

    private:
        friend class Label;

        struct Impl;
        std::shared_ptr<Impl> m_impl;

        EmojiRegistry(std::shared_ptr<Impl> impl);
    };

    /// A drop-in replacement for CCLabelBMFont with improved performance
    /// and support for emojis, Unicode, fallback fonts and color tags.
    class GEODE_DLL Label : public cocos2d::CCNode, public cocos2d::CCRGBAProtocol, public cocos2d::CCLabelProtocol, public cocos2d::CCBlendProtocol {
    public:
        Label();
        ~Label() override;

        enum class Alignment : uint8_t {
            Left,
            Center,
            Right,
            Justify
        };

        /// Create an empty Label with a default font.
        /// @param font Pointer to the default font
        static Label* create(BitmapFont* font);

        /// Create an empty Label with a default font file.
        /// @param font Filename of the .fnt file (e.g. "bigFont.fnt")
        static Label* create(ZStringView font);

        /// Create a Label with the specified text and font.
        /// @param text The text to display
        /// @param font The font to use
        static Label* create(std::string text, BitmapFont* font);

        /// Create a Label with the specified text and font file.
        /// @param text The text to display
        /// @param font The font file to use (e.g. "bigFont.fnt")
        static Label* create(std::string text, ZStringView font);

        /// Create a Label with the specified rich-text and font.
        /// @param text Rich text that may contain color tags
        /// @param font The font to use
        static Label* createRich(std::string text, BitmapFont* font);

        /// Create a Label with the specified rich-text and font file.
        /// @param text Rich text that may contain color tags
        /// @param font The font file to use (e.g. "bigFont.fnt")
        static Label* createRich(std::string text, ZStringView font);

        /// Forces a relayout/redraw of internal quads based on currently active dirty flags.
        /// Runs automatically every frame before rendering or when fetching content size.
        void validate();

        /// Marks the text as dirty. Used to force a full pass of text shaping and layout.
        /// Examples: text, font, or kerning parameter changes.
        void setTextDirty(bool dirty) noexcept;
        bool isTextDirty() noexcept;

        /// Marks the layout as dirty. Used to redraw all quads without triggering Unicode reshaping.
        /// Examples: text alignment, line spacing or wrap width changes.
        void setLayoutDirty(bool dirty) noexcept;
        bool isLayoutDirty() noexcept;

        /// Marks the quads as dirty. Used to modify quad colors efficiently.
        void setQuadsDirty(bool dirty) noexcept;
        bool isQuadsDirty() noexcept;

        /// Registers a fallback font used when glyphs are missing from the primary font.
        /// @return True if font was added successfully.
        bool registerFont(ZStringView font);
        bool registerFont(BitmapFont* font);

        /// Sets the text of the label.
        void setText(std::string text) noexcept;
        ZStringView getText() noexcept;

        void setRichText(std::string text);

        /// Sets the label string. Provided by CCLabelProtocol.
        /// @note Prefer using Label::setText to avoid a string copy.
        void setString(char const* label) override;
        char const* getString() override;

        void setAlignment(Alignment alignment) noexcept;
        Alignment getAlignment() noexcept;

        /// Sets the maximum line width and enables line wrapping mode (or disables if 0.f).
        void setMaxWidth(float maxWidth) noexcept;
        float getMaxWidth() noexcept;

        /// Sets whether line wrapping should also break words in parts to fit the maximum line width.
        /// @note This is only relevant if line wrapping is enabled
        void setBreakWords(bool breakWords) noexcept;
        bool isBreakWords() noexcept;

        /// Sets the maximum label content width and automatically scales it down when needed.
        /// @param width Target content width
        /// @param maxScale The default scale to use when label fits normally
        /// @param minScale Minimum allowed scale
        void setLimitLabelWidth(float width, float maxScale = 1.f, float minScale = 0.f) noexcept;

        /// Sets the maximum label content height and automatically scales it down when needed.
        /// @param height Target content height
        /// @param maxScale The default scale to use when label fits normally
        /// @param minScale Minimum allowed scale
        void setLimitLabelHeight(float height, float maxScale = 1.f, float minScale = 0.f) noexcept;

        /// Sets the maximum label content size and automatically scales it down when needed.
        /// @param size Target content size
        /// @param maxScale The default scale to use when label fits normally
        /// @param minScale Minimum allowed scale
        void setLimitLabelSize(cocos2d::CCSize size, float maxScale = 1.f, float minScale = 0.f) noexcept;

        /// Disables automatic content size scaling.
        void removeLabelSizeLimit() noexcept;

       /// Acts similarly to setLimitLabelSize, except instead of letterboxing it attempts to
       /// automatically pick the best line wrap width, filling more area with largest possible scale.
        void setFitBox(cocos2d::CCSize size, float maxScale = 1.f) noexcept;
        void disableFitBox() noexcept;

        /// Sets extra kerning between each character.
        void setExtraKerning(float kerning) noexcept;
        float getExtraKerning() noexcept;

        /// Sets extra line spacing between each line.
        void setLineSpacing(float lineSpacing) noexcept;
        float getLineSpacing() noexcept;

        /// Sets the color for a specified character index (ignores whitespace).
        /// @note You have to call validate() before attempting to change colors, otherwise the changes will be overwritten.
        void setCharColor(uint32_t index, cocos2d::ccColor4B color);

        /// Gets total amount of characters currently visible (excluding whitespace, emojis and custom nodes).
        size_t getCharCount() noexcept;

        /// Assigns the emoji registry container and enables the rendering of emojis.
        void setEmojiRegistry(EmojiRegistry const& registry);
        EmojiRegistry getEmojiRegistry();

        void setColor(cocos2d::ccColor3B const& color) override;
        cocos2d::ccColor3B const& getColor() override;
        cocos2d::ccColor3B const& getDisplayedColor() override;

        void setOpacity(GLubyte opacity) override;
        GLubyte getOpacity() override;
        GLubyte getDisplayedOpacity() override;

        /// Set whether emojis should inherit the label color or remain white. Defaults to false.
        void setUseColoredEmojis(bool value) noexcept;
        bool getUseColoredEmojis() noexcept;

        /// Set whether custom embedded nodes should inherit the label color or remain the original color. Defaults to false.
        void setUseColoredNodes(bool value) noexcept;
        bool getUseColoredNodes() noexcept;

        std::vector<Ref<CCNode>> const& getEmbeddedNodes() const noexcept;

        void setOpacityModifyRGB(bool bValue) override;
        bool isOpacityModifyRGB() override;

        void setCascadeColorEnabled(bool cascadeColorEnabled) override;
        bool isCascadeColorEnabled() override;

        void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) override;
        bool isCascadeOpacityEnabled() override;

        void updateDisplayedColor(cocos2d::ccColor3B const& color) override;
        void updateDisplayedOpacity(GLubyte opacity) override;

        void setBlendFunc(cocos2d::ccBlendFunc blendFunc) override;
        cocos2d::ccBlendFunc getBlendFunc() override;

        cocos2d::CCSize const& getContentSize() const override;
        cocos2d::CCSize getScaledContentSize() override;

        void draw() override;

    protected:
        bool initWithFont(BitmapFont* font);
        bool initWithFontFile(ZStringView fntFile);
        bool initWithString(std::string text, BitmapFont* font);
        bool initWithString(std::string text, ZStringView fntFile);
        bool initWithRichText(std::string text, BitmapFont* font);
        bool initWithRichText(std::string text, ZStringView fntFile);

    private:
        struct Impl;
        Impl* m_impl = nullptr;
    };
}