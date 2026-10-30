#include <Geode/ui/Label.hpp>
#include <Geode/cocos/platform/CCFileUtils.h>
#include <Geode/cocos/shaders/CCShaderCache.h>
#include <Geode/cocos/textures/CCTextureCache.h>

#include <Geode/loader/GameEvent.hpp>
#include <Geode/utils/StringMap.hpp>
#include <Geode/utils/file.hpp>
#include <asp/collections/SmallVec.hpp>
#include <simdutf/implementation.h>
#include <asp/iter.hpp>

#include <numeric>
#include <ranges>

using namespace geode::prelude;

static StringMap<BitmapFont>& GetBitmapFontsCache() {
    static StringMap<BitmapFont> cache;
    return cache;
}

BitmapFont::BitmapFont() = default;
BitmapFont::~BitmapFont() = default;
BitmapFont::BitmapFont(BitmapFont const&) = default;
BitmapFont::BitmapFont(BitmapFont&&) noexcept = default;
BitmapFont& BitmapFont::operator=(BitmapFont const&) = default;
BitmapFont& BitmapFont::operator=(BitmapFont&&) noexcept = default;

BitmapFont* BitmapFont::load(ZStringView fntFile) {
    auto& cache = GetBitmapFontsCache();
    auto it = cache.find(fntFile);
    if (it != cache.end()) {
        return &it->second;
    }

    BitmapFont font{};
    if (!font.initWithFile(fntFile)) {
        return nullptr;
    }

    font.initBakedValues();

    auto [it2, _] = cache.emplace(fntFile, std::move(font));
    return &it2->second;
}

void BitmapFont::purgeFont(ZStringView fntFile) {
    GetBitmapFontsCache().erase(fntFile);
}

void BitmapFont::purgeFont(BitmapFont const* font) {
    auto& cache = GetBitmapFontsCache();
    for (auto it = cache.begin(); it != cache.end(); ++it) {
        if (&it->second == font) {
            cache.erase(it);
            break;
        }
    }
}

void BitmapFont::purgeAllFonts() {
    GetBitmapFontsCache().clear();
}

template <typename F>
static void forEachPair(std::string_view line, F&& fn) {
    for (auto pair : asp::iter::splitAny(line, " \t")) {
        auto arr = asp::iter::split(pair, '=').collect<asp::SmallVec<std::string_view, 2>>();
        auto size = arr.size();
        if (size == 2) fn(arr[0], arr[1]);
        else if (size == 1) fn(arr[0], std::string_view(""));
    }
}

bool BitmapFont::initWithFile(ZStringView fntFile) {
#if defined(GEODE_IS_MOBILE) || !defined(NDEBUG)
    unsigned long size = 0;
    uint8_t* data = CCFileUtils::get()->getFileData(fntFile.c_str(), "rb", &size);
    if (!data || !size) {
        return false;
    }

    std::unique_ptr<uint8_t[]> buffer(data);
    auto contents = std::string_view(reinterpret_cast<char*>(data), size);
#else
    auto fullPath = CCFileUtils::get()->fullPathForFilename(fntFile.c_str(), false);
    auto res = file::readString(fullPath);
    if (res.isErr()) {
        return false;
    }

    auto contents = std::move(res).unwrap();
#endif

    m_fntFilename = fntFile;

    if (!this->initWithContents(contents)) {
        return false;
    }

    if (m_atlasName.empty()) {
        return false;
    }

    return true;
}

bool BitmapFont::initWithContents(std::string_view text) {
    for (auto line : asp::iter::lines(text)) {
        if (line.starts_with("char ")) {
            line.remove_prefix(5);

            CharDef def;
            forEachPair(line, [&def](std::string_view key, std::string_view value) {
                if (key == "id") {
                    def.codepoint = utils::numFromString<uint32_t>(value).unwrapOrDefault();
                } else if (key == "x") {
                    def.rect.origin.x = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "y") {
                    def.rect.origin.y = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "width") {
                    def.rect.size.width = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "height") {
                    def.rect.size.height = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "xoffset") {
                    def.xOffset = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "yoffset") {
                    def.yOffset = utils::numFromString<float>(value).unwrapOrDefault();
                } else if (key == "xadvance") {
                    def.xAdvance = utils::numFromString<float>(value).unwrapOrDefault();
                }
            });

            m_characters.emplace(def.codepoint, std::move(def));
        } else if (line.starts_with("kerning ")) {
            line.remove_prefix(8);

            KerningPair pair{};
            float amount = 0.f;
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (key == "first") {
                    pair.first = utils::numFromString<uint32_t>(value).unwrapOrDefault();
                } else if (key == "second") {
                    pair.second = utils::numFromString<uint32_t>(value).unwrapOrDefault();
                } else if (key == "amount") {
                    amount = utils::numFromString<float>(value).unwrapOrDefault();
                }
            });

            m_kerning.emplace(pair, KerningValue{amount, amount});
        } else if (line.starts_with("info ")) {
            line.remove_prefix(5);
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (key == "padding") {
                    int values[4] = {};

                    int index = 0;
                    for (auto p : asp::iter::split(value, ',')) {
                        if (index >= 4) break;
                        values[index++] = utils::numFromString<int>(p).unwrapOrDefault();
                    }

                    m_padding.left = values[0];
                    m_padding.top = values[1];
                    m_padding.right = values[2];
                    m_padding.bottom = values[3];
                }
            });
        } else if (line.starts_with("common ")) {
            line.remove_prefix(7);

            auto max = CCConfiguration::sharedConfiguration()->getMaxTextureSize();
            bool error = false;
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (error) return;

                if (key == "lineHeight") {
                    m_commonHeight = utils::numFromString<int>(value).unwrapOrDefault();
                } else if (key == "scaleW") {
                    auto val = utils::numFromString<int>(value).unwrapOrDefault();
                    m_atlasSize.width = val;

                    if (val > max) {
                        log::error("BitmapFont width {} exceeds max texture limit {}", val, max);
                        error = true;
                    }

                } else if (key == "scaleH") {
                    auto val = utils::numFromString<int>(value).unwrapOrDefault();
                    m_atlasSize.height = val;

                    if (val > max) {
                        log::error("BitmapFont height {} exceeds max texture limit {}", val, max);
                        error = true;
                    }
                } else if (key == "pages") {
                    auto val = utils::numFromString<int>(value).unwrapOrDefault();
                    if (val != 1) {
                        log::error("BitmapFont contains more than one atlas ({})", val);
                        error = true;
                    }
                }
            });

            if (error) {
                return false;
            }
        } else if (line.starts_with("page ")) {
            line.remove_prefix(5);
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (key == "file") {
                    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                        value = value.substr(1, value.size() - 2);
                    }

                    if (m_fntFilename.empty()) {
                        m_atlasName = value;
                    } else {
                        StringBuffer<> relPathBuf(value); // null-terminate
                        m_atlasName = CCFileUtils::get()->fullPathFromRelativeFile(relPathBuf.c_str(), m_fntFilename.c_str());
                    }
                }
            });
        } else if (line.starts_with("chars ")) {
            line.remove_prefix(6);
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (key == "count") {
                    m_characters.reserve(utils::numFromString<size_t>(value).unwrapOrDefault());
                }
            });
        } else if (line.starts_with("kernings ")) {
            line.remove_prefix(9);
            forEachPair(line, [&](std::string_view key, std::string_view value) {
                if (key == "count") {
                    m_kerning.reserve(utils::numFromString<size_t>(value).unwrapOrDefault());
                }
            });
        }
    }

    return true;
}

void BitmapFont::initBakedValues() {
    auto scale = 1.f / CCDirector::get()->getContentScaleFactor();

    for (auto& def : m_characters | std::views::values) {
        def.sizeScaled = def.rect.size * scale;
        def.xOffsetScaled = def.xOffset * scale;
        def.yOffsetScaled = def.yOffset * scale;
        def.xAdvanceScaled = def.xAdvance * scale;
        def.uv = CCRect{
            def.rect.origin.x / m_atlasSize.width, // left
            def.rect.origin.y / m_atlasSize.height, // top
            (def.rect.origin.x + def.rect.size.width) / m_atlasSize.width, // right
            (def.rect.origin.y + def.rect.size.height) / m_atlasSize.height // bottom
        };
    }

    for (auto& kerning : m_kerning | std::views::values) {
        kerning.scaled *= scale;
    }

    m_commonHeightScaled = m_commonHeight * scale;
}

void BitmapFont::setFontFile(std::string file) noexcept {
    m_fntFilename = std::move(file);
}

ZStringView BitmapFont::getFontFile() const noexcept {
    return m_fntFilename;
}

void BitmapFont::setAtlasName(std::string name) noexcept {
    m_atlasName = std::move(name);
}

ZStringView BitmapFont::getAtlasName() const noexcept {
    return m_atlasName;
}

std::unordered_map<uint32_t, BitmapFont::CharDef> const& BitmapFont::getCharDefs() const noexcept {
    return m_characters;
}

BitmapFont::KerningMap const& BitmapFont::getKernings() const noexcept {
    return m_kerning;
}

BitmapFont::Padding BitmapFont::getPadding() const noexcept {
    return m_padding;
}

CCSize BitmapFont::getAtlasSize() const noexcept {
    return m_atlasSize;
}

float BitmapFont::getCommonHeight() const noexcept {
    return m_commonHeight;
}

float BitmapFont::getCommonHeightScaled() const noexcept {
    return m_commonHeightScaled;
}

float BitmapFont::getKerning(char32_t first, char32_t second) const noexcept {
    auto it = m_kerning.find({ first, second });
    if (it != m_kerning.end()) {
        return it->second.scaled;
    }
    return 0.f;
}

struct EmojiRegistry::Impl {
    std::vector<std::u32string_view> m_keys;
    std::vector<Entry> m_entries;

    ListenerHandle m_textureReloadHandle;

    Impl() {
        this->listenForTextureReload();
    }

    Impl(Impl const& other) : m_keys(other.m_keys), m_entries(other.m_entries) {
        this->listenForTextureReload();
    }

    void listenForTextureReload() {
        m_textureReloadHandle = GameEvent(GameEventType::TexturesUnloaded).listen([this]() {
            for (auto& entry : m_entries) {
                if (auto* emoji = entry.getEmoji()) {
                    emoji->invalidate();
                }
            }
        });
    }

    Entry const* find(std::u32string_view sequence) const {
        auto it = std::ranges::lower_bound(m_keys, sequence);
        if (it != m_keys.end() && *it == sequence) {
            return &m_entries[static_cast<size_t>(it - m_keys.begin())];
        }
        return nullptr;
    }

    Entry* match(std::u32string_view str, size_t& index) {
        if (index >= str.size() || m_keys.empty()) {
            return nullptr;
        }

        std::u32string_view slice = str.substr(index);

        auto it = std::ranges::upper_bound(m_keys, slice);

        for (auto r = std::make_reverse_iterator(it); r != m_keys.rend(); ++r) {
            std::u32string_view key = *r;

            if (key.size() <= slice.size() && slice.starts_with(key)) {
                size_t idx = std::distance(m_keys.begin(), r.base() - 1);
                index += key.size() - 1;
                return &m_entries[idx];
            }

            if (!key.empty() && key[0] < slice[0]) {
                break;
            }
        }

        return nullptr;
    }

    void insert(std::u32string_view sequence, Entry entry) {
        auto it = std::ranges::lower_bound(m_keys, sequence);
        auto index = std::distance(m_keys.begin(), it);

        if (it != m_keys.end() && *it == sequence) {
            m_entries[index] = std::move(entry);
        } else {
            m_keys.insert(it, sequence);
            m_entries.insert(m_entries.begin() + index, std::move(entry));
        }
    }

    void insert(std::span<std::tuple<std::u32string_view, Entry> const> entries) {
        if (entries.empty()) return;

        size_t newSize = m_keys.size() + entries.size();
        m_keys.reserve(newSize);
        m_entries.reserve(newSize);

        for (auto& item : entries) {
            m_keys.push_back(std::get<0>(item));
            m_entries.push_back(std::move(std::get<1>(item)));
        }

        std::vector<size_t> indices(m_keys.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            indices[i] = i;
        }

        std::ranges::sort(indices, [this](size_t a, size_t b) {
            return m_keys[a] < m_keys[b];
        });

        std::vector<std::u32string_view> sortedKeys;
        std::vector<Entry> sortedEntries;
        sortedKeys.reserve(m_keys.size());
        sortedEntries.reserve(m_entries.size());

        for (size_t idx : indices) {
            sortedKeys.push_back(m_keys[idx]);
            sortedEntries.push_back(std::move(m_entries[idx]));
        }

        m_keys = std::move(sortedKeys);
        m_entries = std::move(sortedEntries);
    }

    void erase(std::u32string_view sequence) {
        auto it = std::ranges::lower_bound(m_keys, sequence);
        auto index = std::distance(m_keys.begin(), it);
        if (it != m_keys.end() && *it == sequence) {
            m_entries.erase(m_entries.begin() + index);
            m_keys.erase(it);
        }
    }
};

class SharedIndexBuffer {
public:
    static SharedIndexBuffer& get() {
        static SharedIndexBuffer instance;
        return instance;
    }

    GLuint getIBO(size_t quads) {
        if (m_ibo == 0) {
            glGenBuffers(1, &m_ibo);
        }

        if (quads > m_capacity) {
            size_t newCapacity = std::max(quads, m_capacity + m_capacity / 2 + 128);

            std::vector<uint32_t> indices(newCapacity * 6);
            for (size_t i = 0; i < newCapacity; ++i) {
                auto v = static_cast<uint32_t>(i * 4);
                indices[i * 6 + 0] = v + 0;
                indices[i * 6 + 1] = v + 1;
                indices[i * 6 + 2] = v + 2;
                indices[i * 6 + 3] = v + 2;
                indices[i * 6 + 4] = v + 1;
                indices[i * 6 + 5] = v + 3;
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                indices.size() * sizeof(uint32_t),
                indices.data(),
                GL_STATIC_DRAW
            );
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            m_capacity = newCapacity;
        }

        return m_ibo;
    }

    void purge() {
        if (m_ibo != 0) {
            glDeleteBuffers(1, &m_ibo);
            m_ibo = 0;
            m_capacity = 0;
        }
    }

private:
    GLuint m_ibo = 0;
    size_t m_capacity = 0;
};

$on_game(TexturesUnloaded) {
    BitmapFont::purgeAllFonts();
    SharedIndexBuffer::get().purge();
}

struct LabelFontBatch {
    Ref<CCTexture2D> texture;
    std::vector<ccV2F_C4B_T2F_Quad> quads;
    size_t vboCapacity = 0;
    GLuint vbo = 0;
    bool isDirty = true;

    LabelFontBatch() = default;
    LabelFontBatch(CCTexture2D* tex) : texture(tex) {}
    LabelFontBatch(LabelFontBatch const&) = delete;
    LabelFontBatch(LabelFontBatch&& o) noexcept
        : texture(std::move(o.texture)),
          quads(std::move(o.quads)),
          vboCapacity(o.vboCapacity),
          vbo(o.vbo), isDirty(o.isDirty)
    {
        o.vbo = 0;
        o.vboCapacity = 0;
    }

    ~LabelFontBatch() {
        if (vbo) glDeleteBuffers(1, &vbo);
    }

    LabelFontBatch& operator=(LabelFontBatch const&) = delete;
    LabelFontBatch& operator=(LabelFontBatch&& o) noexcept {
        if (this != &o) {
            if (vbo) glDeleteBuffers(1, &vbo);
            texture = std::move(o.texture);
            quads = std::move(o.quads);
            vboCapacity = std::exchange(o.vboCapacity, 0);
            vbo = std::exchange(o.vbo, 0);
            isDirty = o.isDirty;
        }
        return *this;
    }

    void upload() {
        if (!isDirty || quads.empty()) return;

        if (vbo == 0) glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        if (quads.size() > vboCapacity) {
            vboCapacity = quads.size() + quads.size() / 2 + 16;
        }

        glBufferData(GL_ARRAY_BUFFER, vboCapacity * sizeof(ccV2F_C4B_T2F_Quad), nullptr, GL_STREAM_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, quads.size() * sizeof(ccV2F_C4B_T2F_Quad), quads.data());

        isDirty = false;
    }

    void draw() const {
        if (quads.empty()) return;

        ccGLBindTexture2D(texture->getName());
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(
            kCCVertexAttrib_Position,
            2, GL_FLOAT, GL_FALSE,
            sizeof(ccV2F_C4B_T2F),
            reinterpret_cast<void*>(offsetof(ccV2F_C4B_T2F, vertices))
        );
        glVertexAttribPointer(
            kCCVertexAttrib_Color,
            4, GL_UNSIGNED_BYTE, GL_TRUE,
            sizeof(ccV2F_C4B_T2F),
            reinterpret_cast<void*>(offsetof(ccV2F_C4B_T2F, colors))
        );
        glVertexAttribPointer(
            kCCVertexAttrib_TexCoords,
            2, GL_FLOAT, GL_FALSE,
            sizeof(ccV2F_C4B_T2F),
            reinterpret_cast<void*>(offsetof(ccV2F_C4B_T2F, texCoords))
        );

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(quads.size() * 6),
            GL_UNSIGNED_INT,
            nullptr
        );
    }
};

struct CharQuadRef {
    uint32_t batchIndex : 8;
    uint32_t charIndex : 24;
};

struct EmojiQuadRef {
    uint32_t batchIndex;
    uint32_t quadIndex;
    EmojiRegistry::EmojiEntry const* emoji;
};

struct ShapedItem {
    enum class Kind : uint8_t {
        Glyph,
        Emoji,
        Newline,
        Space,
        Node,
    } kind;
    uint8_t fontIndex;
    float advance;

    union {
        BitmapFont::CharDef const* def; // glyph
        CCNode* node; // embedded
        EmojiRegistry::EmojiEntry const* emoji; // emoji
    };

    uint32_t srcIndex = 0;
    uint32_t srcLength = 1;
};

struct WordSpan {
    uint32_t start, end;
    float width;
    bool isWhitespace;
};

struct LineSpan {
    uint32_t start, end;
    float width;
    uint32_t whitespaceCount;
};

struct StyleSpan {
    uint32_t start, end;
    std::optional<ccColor3B> color;
};

struct Label::Impl {
    std::string m_text;
    asp::SmallVec<BitmapFont*, 2> m_fonts;
    asp::SmallVec<LabelFontBatch, 2> m_batches;
    std::vector<CharQuadRef> m_chars;
    std::vector<EmojiQuadRef> m_emojiQuads;

    Label* m_label;
    std::shared_ptr<EmojiRegistry::Impl> m_emojiRegistry;
    asp::SmallVec<LabelFontBatch, 1> m_emojiBatches;
    std::vector<Ref<CCNode>> m_embeddedNodes;

    std::vector<ShapedItem> m_shaped;
    asp::SmallVec<WordSpan, 3> m_words;
    asp::SmallVec<LineSpan, 1> m_lines;

    std::vector<StyleSpan> m_styles;

    size_t m_codepointCount = 0;
    std::vector<CCPoint> m_itemPens;
    std::vector<uint8_t> m_penCovered;
    std::vector<GlyphInfo> m_glyphs;

    float m_spaceWidth = 0.f;
    float m_maxLineWidth = 0.f;
    float m_extraKerning = 0.f;
    float m_extraLineSpacing = 0.f;

    Alignment m_alignment = Alignment::Left;
    bool m_wordBreak = false;
    bool m_lineBreak = false;
    bool m_isOpacityModifyRGB = true;

    ccBlendFunc m_blendFunc = {CC_BLEND_SRC, CC_BLEND_DST};
    ccColor3B m_color = {255, 255, 255};
    GLubyte m_opacity = 255;
    ccColor3B m_displayedColor = {255, 255, 255};
    GLubyte m_displayedOpacity = 255;
    bool m_cascadeColorEnabled = false;
    bool m_cascadeOpacityEnabled = false;

    bool m_emojisUseColors = false;
    bool m_customNodesColors = false;

    CCSize m_limitSize{};
    float m_defaultScale = 1.f;
    float m_minScale = 0.f;
    bool m_limitChanged = false;
    bool m_fitLabelSize = false;

    bool m_useRichText = false;

    bool m_textDirty = true;
    bool m_layoutDirty = true;
    bool m_quadsDirty = true;
    bool m_sizeDirty = true;

    Impl(Label* label) : m_label(label) {}

    float getFontScale(size_t fontIndex) const {
        if (fontIndex == 0 || m_fonts.empty()) return 1.0f;

        float baseHeight = m_fonts.data()[0]->getCommonHeight();
        float fallbackHeight = m_fonts.data()[fontIndex]->getCommonHeight();

        if (fallbackHeight <= 0.0f) return 1.0f;
        return baseHeight / fallbackHeight;
    }

    ccColor4B getEffectiveColor(ccColor3B color) const {
        if (m_isOpacityModifyRGB) {
            return {
                static_cast<GLubyte>(color.r * m_displayedOpacity / 255),
                static_cast<GLubyte>(color.g * m_displayedOpacity / 255),
                static_cast<GLubyte>(color.b * m_displayedOpacity / 255),
                static_cast<GLubyte>(m_displayedOpacity)
            };
        }

        return { color.r, color.g, color.b, m_displayedOpacity };
    }

    ccColor4B getEffectiveColor() const {
        return this->getEffectiveColor(m_displayedColor);
    }

    ccColor4B getEmojiColor() const {
        if (m_emojisUseColors) {
            return { m_displayedColor.r, m_displayedColor.g, m_displayedColor.b, m_displayedOpacity };
        }
        return { 255, 255, 255, m_displayedOpacity };
    }

    ccColor4B getEmojiColorTint(EmojiRegistry::EmojiEntry const* entry) const {
        if (entry->tint != ccColor3B{255, 255, 255}) {
            auto emojiColor = this->getEmojiColor();
            emojiColor.r = ((emojiColor.r / 255.f) * (entry->tint.r / 255.f)) * 255;
            emojiColor.g = ((emojiColor.g / 255.f) * (entry->tint.g / 255.f)) * 255;
            emojiColor.b = ((emojiColor.b / 255.f) * (entry->tint.b / 255.f)) * 255;
            return emojiColor;
        }
        return this->getEmojiColor();
    }

    cocos2d::CCPoint endPen() const {
        float lineHeight = m_fonts.empty() ? 0.f :
            m_fonts.data()[0]->getCommonHeightScaled() + m_extraLineSpacing;

        if (m_shaped.empty()) {
            return { 0.f, lineHeight };
        }

        auto const& last = m_shaped.back();
        auto const& pen = m_itemPens.back();

        if (last.kind == ShapedItem::Kind::Newline) {
            return { 0.f, pen.y - lineHeight };
        }

        return { pen.x + last.advance, pen.y };
    }

    uint8_t getOrCreateBatch(CCTexture2D* texture) {
        for (size_t i = 0; i < m_emojiBatches.size(); i++) {
            if (m_emojiBatches[i].texture == texture) {
                return static_cast<uint8_t>(i);
            }
        }

        m_emojiBatches.emplace_back(LabelFontBatch{texture});
        return static_cast<uint8_t>(m_emojiBatches.size() - 1);
    }

    bool tryMatchEmojiOrNode(std::u32string_view str, size_t& index) {
        if (!m_emojiRegistry || m_fonts.empty()) return false;

        size_t srcIndex = index;
        auto entry = m_emojiRegistry->match(str, index);
        if (!entry) return false;

        float baseHeight = m_fonts.data()[0]->getCommonHeightScaled();

        if (auto* emoji = entry->getEmoji()) {
            if (auto frame = emoji->getFrame()) {
                auto size = frame->getRect().size;
                auto batchIndex = this->getOrCreateBatch(frame->getTexture());

                float frameH = size.height > 0.f ? size.height : 1.f;
                float frameW = size.width;
                float scale = baseHeight / frameH * emoji->scale;

                m_shaped.push_back(ShapedItem{
                    .kind = ShapedItem::Kind::Emoji,
                    .fontIndex = batchIndex,
                    .advance = frameW * scale,
                    .emoji = emoji,
                    .srcIndex = static_cast<uint32_t>(srcIndex),
                    .srcLength = static_cast<uint32_t>(index - srcIndex + 1),
                });
            }
            return true;
        }

        CCNode* node = nullptr;
        if (auto* factory = entry->getNodeFactory()) {
            node = (*factory)();
        } else if (auto* factory = entry->getNodeFactoryParams()) {
            node = (*factory)(str, index);
        }

        if (node) {
            auto size = node->getContentSize();

            m_shaped.push_back(ShapedItem{
                .kind = ShapedItem::Kind::Node,
                .fontIndex = 0,
                .advance = size.width * (baseHeight / (size.height > 0.f ? size.height : 1.f)),
                .node = node,
                .srcIndex = static_cast<uint32_t>(srcIndex),
                .srcLength = static_cast<uint32_t>(index - srcIndex + 1),
            });

            m_label->addChild(node);
            m_embeddedNodes.push_back(node);
        }

        return true;
    }

    std::tuple<BitmapFont::CharDef const*, uint8_t> findGlyphDef(char32_t& ch) {
        char32_t upper = (ch >= U'a' && ch <= U'z') ? (ch - U'a' + U'A') : ch; // backwards compat with CCLabelBMFont

        for (uint8_t j = 0; j < m_fonts.size(); ++j) {
            auto const& chars = m_fonts[j]->getCharDefs();
            auto it = chars.find(ch);
            if (it != chars.end()) {
                return { &it->second, j };
            }

            if (ch != upper) {
                auto it2 = chars.find(upper);
                if (it2 != chars.end()) {
                    ch = upper;
                    return {&it2->second, j};
                }
            }
        }

        return {nullptr, 0};
    }

    void appendGlyphItem(
        char32_t ch,
        BitmapFont::CharDef const* def,
        uint8_t fontIndex,
        uint32_t srcIndex,
        uint32_t& prevCp,
        uint8_t& prevFontIndex
    ) {
        float fontScale = this->getFontScale(fontIndex);

        float advance = def->xAdvanceScaled * fontScale;
        if (prevCp != 0 && prevFontIndex == fontIndex) {
            float kerning = m_fonts[fontIndex]->getKerning(prevCp, ch) * fontScale;
            if (kerning != 0.f && !m_shaped.empty()) {
                m_shaped.back().advance += kerning;
            }
        }
        advance += m_extraKerning;

        m_shaped.push_back(ShapedItem{
            .kind = ShapedItem::Kind::Glyph,
            .fontIndex = fontIndex,
            .advance = advance,
            .def = def,
            .srcIndex = srcIndex,
            .srcLength = 1,
        });

        prevCp = ch;
        prevFontIndex = fontIndex;
    }

    static constexpr int parseHexDigit(char32_t ch) {
        if (ch >= U'0' && ch <= U'9') return ch - U'0';
        if (ch >= U'a' && ch <= U'f') return ch - U'a' + 10;
        if (ch >= U'A' && ch <= U'F') return ch - U'A' + 10;
        return -1;
    }

    static std::optional<ccColor3B> parseHexColor(std::u32string_view str) {
        if (str.size() == 3) {
            int r = parseHexDigit(str[0]);
            int g = parseHexDigit(str[1]);
            int b = parseHexDigit(str[2]);

            if (r < 0 || g < 0 || b < 0) return std::nullopt;

            return ccColor3B{
                static_cast<GLubyte>(r * 17),
                static_cast<GLubyte>(g * 17),
                static_cast<GLubyte>(b * 17)
            };
        }

        if (str.size() == 6) {
            int r1 = parseHexDigit(str[0]);
            int r2 = parseHexDigit(str[1]);
            int g1 = parseHexDigit(str[2]);
            int g2 = parseHexDigit(str[3]);
            int b1 = parseHexDigit(str[4]);
            int b2 = parseHexDigit(str[5]);

            if (r1 < 0 || r2 < 0 || g1 < 0 || g2 < 0 || b1 < 0 || b2 < 0) return std::nullopt;

            return ccColor3B{
                static_cast<GLubyte>((r1 << 4) | r2),
                static_cast<GLubyte>((g1 << 4) | g2),
                static_cast<GLubyte>((b1 << 4) | b2),
            };
        }

        return std::nullopt;
    }

    static bool tryParseRichTags(std::u32string_view str, size_t& i, std::vector<ccColor3B>& colorStack) {
        if (str[i] != U'<') return false;

        size_t closePos = str.find(U'>', i);
        if (closePos == std::string::npos) return false;

        auto tag = str.substr(i + 1, closePos - i - 1);
        if (tag.empty()) return false;

        if (tag == U"/c") {
            if (colorStack.empty()) return false;

            colorStack.pop_back();
            i = closePos;
            return true;
        }

        if (tag[0] == U'c') {
            ccColor3B color{255, 0, 0};

            if (tag.size() == 2) {
                switch (tag[1]) {
                    case U'b': color = {74, 82, 225}; break;
                    case U'g': color = {64, 227, 72}; break;
                    case U'l': color = {96, 171, 239}; break;
                    case U'j': color = {50, 200, 255}; break;
                    case U'y': color = {255, 255, 0}; break;
                    case U'o': color = {255, 90, 75}; break;
                    case U'r': color = {255, 90, 90}; break;
                    case U'p': color = {255, 0, 255}; break;
                    case U'a': color = {149, 50, 255}; break;
                    case U'd': color = {255, 150, 255}; break;
                    case U'c': color = {255, 255, 150}; break;
                    case U'f': color = {150, 255, 255}; break;
                    case U's': color = {255, 220, 65}; break;
                    default: break;
                }
            } else if (tag.size() > 2 && tag[1] == U'-') {
                auto hex = tag.substr(2);
                if (auto parsed = parseHexColor(hex)) {
                    color = parsed.value();
                }
            }

            colorStack.push_back(color);
            i = closePos;
            return true;
        }

        return false;
    }

    void reshapeRich(std::u32string_view str) {
        uint32_t prevCp = 0;
        uint8_t prevFontIndex = 0;

        std::vector<ccColor3B> colorStack;

        auto currentColor = [&]() -> std::optional<ccColor3B> {
            return colorStack.empty() ? std::nullopt : std::optional(colorStack.back());
        };

        auto openSpan = [&](uint32_t start) {
            auto color = currentColor();

            if (!m_styles.empty() && m_styles.back().end == start && m_styles.back().color == color) {
                m_styles.back().end = std::numeric_limits<uint32_t>::max();
                return;
            }

            m_styles.push_back(StyleSpan{
                .start = start,
                .end = std::numeric_limits<uint32_t>::max(),
                .color = currentColor()
            });
        };

        auto closeSpan = [&](uint32_t end) {
            if (!m_styles.empty() && m_styles.back().end == std::numeric_limits<uint32_t>::max()) {
                if (m_styles.back().start == end) {
                    m_styles.pop_back();
                } else {
                    m_styles.back().end = end;
                }
            }
        };

        openSpan(0);

        for (size_t i = 0; i < str.length(); i++) {
            char32_t ch = str[i];
            switch (ch) {
                case '\n': {
                    m_shaped.push_back(ShapedItem{
                        .kind = ShapedItem::Kind::Newline,
                        .fontIndex = 0,
                        .advance = 0.f,
                        .def = nullptr,
                        .srcIndex = static_cast<uint32_t>(i),
                        .srcLength = 1,
                    });
                    prevCp = 0;
                    break;
                }
                case ' ':
                case '\t': {
                    float advance = m_spaceWidth + m_extraKerning;
                    if (ch == '\t') advance *= 4.f;

                    m_shaped.push_back(ShapedItem{
                        .kind = ShapedItem::Kind::Space,
                        .fontIndex = 0,
                        .advance = advance,
                        .def = nullptr,
                        .srcIndex = static_cast<uint32_t>(i),
                        .srcLength = 1,
                    });

                    prevCp = ch;
                    prevFontIndex = 0;
                    break;
                }
                default: {
                    if (this->tryMatchEmojiOrNode(str, i)) {
                        prevCp = 0;
                        continue;
                    }

                    if (this->tryParseRichTags(str, i, colorStack)) {
                        auto idx = m_shaped.size();
                        closeSpan(idx);
                        openSpan(idx);
                        continue;
                    }

                    auto [def, fontIndex] = this->findGlyphDef(ch);
                    if (!def) break;

                    this->appendGlyphItem(ch, def, fontIndex, static_cast<uint32_t>(i), prevCp, prevFontIndex);
                    break;
                }
            }
        }

        closeSpan(m_shaped.size());
    }

    void reshape() {
        for (auto& node : m_embeddedNodes) {
            node->removeFromParent();
        }

        m_embeddedNodes.clear();
        m_shaped.clear();
        m_styles.clear();

        fmt::basic_memory_buffer<char32_t> buffer;
        buffer.resize(simdutf::utf32_length_from_utf8(m_text));
        if (simdutf::convert_utf8_to_utf32(m_text.data(), m_text.size(), buffer.data()) == 0) {
            return;
        }

        auto str = std::u32string_view(buffer.data(), buffer.size());
        m_codepointCount = str.size();
        m_shaped.reserve(str.size());

        if (m_useRichText) {
            this->reshapeRich(str);
            return;
        }

        uint32_t prevCp = 0;
        uint8_t prevFontIndex = 0;

        for (size_t i = 0; i < str.length(); i++) {
            char32_t ch = str[i];
            switch (ch) {
                case '\n': {
                    m_shaped.push_back(ShapedItem{
                        .kind = ShapedItem::Kind::Newline,
                        .fontIndex = 0,
                        .advance = 0.f,
                        .def = nullptr,
                        .srcIndex = static_cast<uint32_t>(i),
                        .srcLength = 1,
                    });
                    prevCp = 0;
                    break;
                }
                case ' ':
                case '\t': {
                    float advance = m_spaceWidth + m_extraKerning;
                    if (ch == '\t') advance *= 4.f;

                    m_shaped.push_back(ShapedItem{
                        .kind = ShapedItem::Kind::Space,
                        .fontIndex = 0,
                        .advance = advance,
                        .def = nullptr,
                        .srcIndex = static_cast<uint32_t>(i),
                        .srcLength = 1,
                    });

                    prevCp = ch;
                    prevFontIndex = 0;
                    break;
                }
                default: {
                    if (this->tryMatchEmojiOrNode(str, i)) {
                        prevCp = 0;
                        continue;
                    }

                    auto [def, fontIndex] = this->findGlyphDef(ch);
                    if (!def) break;

                    this->appendGlyphItem(ch, def, fontIndex, static_cast<uint32_t>(i), prevCp, prevFontIndex);
                    break;
                }
            }
        }
    }

    void splitWords() {
        m_words.clear();
        m_words.reserve(m_shaped.size());

        uint32_t i = 0;
        while (i < m_shaped.size()) {
            auto kind = m_shaped[i].kind;

            if (kind == ShapedItem::Kind::Newline) {
                m_words.push_back(WordSpan{ i, i + 1, 0.f, false });
                ++i;
                continue;
            }

            bool isWs = kind == ShapedItem::Kind::Space;
            uint32_t start = i;
            float width = 0.f;

            while (
                i < m_shaped.size()
                && m_shaped[i].kind != ShapedItem::Kind::Newline
                && (m_shaped[i].kind == ShapedItem::Kind::Space) == isWs
            ) {
                width += m_shaped[i].advance;
                ++i;
            }

            m_words.push_back(WordSpan{ start, i, width, isWs });
        }
    }

    void breakLinesSimple() {
        m_lines.clear();

        if (m_words.empty()) return;

        uint32_t lineWordStart = 0;
        float width = 0.f;
        uint32_t whitespaceCount = 0;

        for (uint32_t wi = 0; wi < m_words.size(); ++wi) {
            auto const& w = m_words[wi];

            if (w.end - w.start == 1 && m_shaped[w.start].kind == ShapedItem::Kind::Newline) {
                m_lines.push_back(LineSpan{ lineWordStart, wi, width, whitespaceCount });
                lineWordStart = wi + 1;
                width = 0.f;
                whitespaceCount = 0;
                continue;
            }

            width += w.width;
            if (w.isWhitespace && wi > lineWordStart) ++whitespaceCount;
        }

        m_lines.push_back(LineSpan{ lineWordStart, static_cast<uint32_t>(m_words.size()), width, whitespaceCount });
    }

    void breakLongWords() {
        if (m_words.empty()) return;

        asp::SmallVec<WordSpan, 3> result;
        result.reserve(m_words.size());

        for (auto const& w : m_words) {
            if (w.isWhitespace || w.width <= m_maxLineWidth) {
                result.emplace_back(w);
                continue;
            }

            uint32_t segStart = w.start;
            float segWidth = 0.f;

            for (uint32_t i = w.start; i < w.end; ++i) {
                float advance = m_shaped[i].advance;

                if (segWidth + advance > m_maxLineWidth && i > segStart) {
                    result.push_back(WordSpan{ segStart, i, segWidth, false });
                    segStart = i;
                    segWidth = 0.f;
                }

                segWidth += advance;
            }

            result.push_back(WordSpan{ segStart, w.end, segWidth, false });
        }

        m_words = std::move(result);
    }

    void breakLinesWrapped() {
        m_lines.clear();

        if (m_words.empty()) return;

        uint32_t lineWordStart = 0;
        float lineWidth = 0.f;
        float lineWidthNoSpace = 0.f;
        uint32_t whitespaceCount = 0;

        auto pushLine = [&](uint32_t end) {
            if (end > lineWordStart && m_words[end - 1].isWhitespace) {
                --end;
            }
            m_lines.push_back(LineSpan{ lineWordStart, end, lineWidthNoSpace, whitespaceCount });
        };

        for (uint32_t wi = 0; wi < m_words.size(); ++wi) {
            auto const& w = m_words[wi];

            if (w.end - w.start == 1 && m_shaped[w.start].kind == ShapedItem::Kind::Newline) {
                pushLine(wi);
                lineWordStart = wi + 1;
                lineWidth = 0.f;
                lineWidthNoSpace = 0.f;
                whitespaceCount = 0;
                continue;
            }

            if (wi != lineWordStart && lineWidth + w.width > m_maxLineWidth) {
                pushLine(wi);
                lineWordStart = wi;

                // strip leading whitespace
                while (lineWordStart < m_words.size() && m_words[lineWordStart].isWhitespace) {
                    auto const& w1 = m_words[lineWordStart];
                    if (w1.end - w1.start == 1 && m_shaped[w1.start].kind == ShapedItem::Kind::Newline) {
                        break;
                    }
                    ++lineWordStart;
                }

                lineWidth = 0.f;
                lineWidthNoSpace = 0.f;
                whitespaceCount = 0;
            }

            lineWidth += w.width;
            if (!w.isWhitespace) {
                lineWidthNoSpace = lineWidth;
                if (wi > lineWordStart && m_words[wi - 1].isWhitespace) {
                    ++whitespaceCount;
                }
            }
        }

        pushLine(m_words.size());
    }

    void breakLines() {
        if (m_lineBreak && m_maxLineWidth > 0.f) {
            if (m_wordBreak) {
                this->breakLongWords();
            }
            this->breakLinesWrapped();
        } else {
            this->breakLinesSimple();
        }
    }

    void emitQuads() {
        for (auto& batch : m_batches) {
            batch.quads.clear();
            batch.isDirty = true;
        }

        for (auto& batch : m_emojiBatches) {
            batch.quads.clear();
            batch.isDirty = true;
        }

        m_chars.clear();
        m_chars.reserve(m_shaped.size());
        m_emojiQuads.clear();
        m_emojiQuads.reserve(m_shaped.size());

        m_glyphs.clear();
        m_glyphs.reserve(m_shaped.size());
        m_itemPens.assign(m_shaped.size(), CCPoint{});
        m_penCovered.assign(m_shaped.size(), 0);

        if (m_lines.empty() || m_fonts.empty()) {
            m_label->setContentSize({0.f, 0.f});
            return;
        }

        float alignWidth = 0.f;
        for (auto const& line : m_lines) alignWidth = std::max(alignWidth, line.width);

        auto color = this->getEffectiveColor();
        float commonHeight = m_fonts[0]->getCommonHeightScaled();
        float lineHeight = commonHeight + m_extraLineSpacing;

        float maxLineWidth = 0.f;
        float totalHeight = lineHeight * m_lines.size();
        float y = totalHeight;

        size_t styleIdx = 0;
        auto getColor = [&](uint32_t i) -> ccColor4B {
            if (!m_useRichText) return color;

            while (styleIdx + 1 < m_styles.size() && i >= m_styles[styleIdx].end) {
                ++styleIdx;
            }

            if (styleIdx < m_styles.size() && m_styles[styleIdx].color) {
                return this->getEffectiveColor(*m_styles[styleIdx].color);
            }

            return color;
        };

        for (uint32_t li = 0; li < m_lines.size(); ++li) {
            auto const& line = m_lines[li];
            bool isLastLine = li == m_lines.size() - 1;

            float startX = 0.f;
            float extraPerGap = 0.f;

            switch (m_alignment) {
                case Alignment::Center:
                    startX = (alignWidth - line.width) * 0.5f;
                    break;
                case Alignment::Right:
                    startX = alignWidth - line.width;
                    break;
                case Alignment::Justify:
                    if (!isLastLine && line.whitespaceCount > 0 && alignWidth > 0.f) {
                        extraPerGap = (alignWidth - line.width) / static_cast<float>(line.whitespaceCount);
                    }
                    break;
                case Alignment::Left:
                default:
                    break;
            }

            float x = startX;
            float lineMaxX = startX;

            for (uint32_t wi = line.start; wi < line.end; ++wi) {
                auto const& word = m_words[wi];

                for (uint32_t ii = word.start; ii < word.end; ++ii) {
                    auto const& item = m_shaped[ii];

                    m_itemPens[ii] = CCPoint{x, y};
                    m_penCovered[ii] = 1;

                    switch (item.kind) {
                        case ShapedItem::Kind::Glyph: {
                            auto const& def = *item.def;
                            auto& batch = m_batches[item.fontIndex];
                            float fontScale = this->getFontScale(item.fontIndex);

                            float qx = x + def.xOffsetScaled * fontScale;
                            float qy = y - def.yOffsetScaled * fontScale;
                            float w = def.sizeScaled.width * fontScale;
                            float h = def.sizeScaled.height * fontScale;

                            lineMaxX = std::max(lineMaxX, qx + w);

                            auto newColor = getColor(ii);

                            batch.quads.push_back({
                                .bl = { .vertices = { qx, qy - h }, .colors = newColor, .texCoords = { def.uv.origin.x, def.uv.size.height } },
                                .br = { .vertices = { qx + w, qy - h }, .colors = newColor, .texCoords = { def.uv.size.width, def.uv.size.height } },
                                .tl = { .vertices = { qx, qy }, .colors = newColor, .texCoords = { def.uv.origin.x, def.uv.origin.y } },
                                .tr = { .vertices = { qx + w, qy }, .colors = newColor, .texCoords = { def.uv.size.width, def.uv.origin.y } },
                            });

                            m_chars.push_back({
                                .batchIndex = item.fontIndex,
                                .charIndex = static_cast<uint32_t>(batch.quads.size() - 1)
                            });

                            m_glyphs.push_back(GlyphInfo{
                                CCRect{ qx, qy - h, w, h },
                                item.srcIndex, item.srcLength, GlyphKind::Glyph,
                            });
                            break;
                        }
                        case ShapedItem::Kind::Node: {
                            auto size = item.node->getContentSize();
                            float scale = size.height > 0.f ? commonHeight / size.height : 1.f;
                            item.node->setScale(scale);
                            item.node->setPosition({ x + size.width * scale * 0.5f, y + commonHeight * 0.5f - commonHeight });
                            if (auto rgba = typeinfo_cast<CCRGBAProtocol*>(item.node)) {
                                if (m_customNodesColors) rgba->setColor(m_color);
                                rgba->setOpacity(m_opacity);
                            }

                            auto pos = item.node->getPosition();
                            m_glyphs.push_back(GlyphInfo{
                                CCRect{
                                    pos.x - size.width * scale * 0.5f,
                                    pos.y - size.height * scale * 0.5f,
                                    size.width * scale,
                                    size.height * scale
                                },
                                item.srcIndex, item.srcLength, GlyphKind::Node,
                            });

                            lineMaxX = std::max(lineMaxX, x + item.advance);
                            break;
                        }
                        case ShapedItem::Kind::Emoji: {
                            auto frame = item.emoji->getFrame();
                            auto const& rectPoints = frame->getRect();
                            auto const& rectPixels = frame->getRectInPixels();
                            bool rotated = frame->isRotated();

                            auto& batch = m_emojiBatches[item.fontIndex];
                            auto texture = batch.texture.data();
                            float texW = static_cast<float>(texture->getPixelsWide());
                            float texH = static_cast<float>(texture->getPixelsHigh());

                            float blU, blV, brU, brV, tlU, tlV, trU, trV;

                            if (!rotated) {
                                float left = rectPixels.origin.x / texW;
                                float right = (rectPixels.origin.x + rectPixels.size.width) / texW;
                                float top = rectPixels.origin.y / texH;
                                float bottom = (rectPixels.origin.y + rectPixels.size.height) / texH;

                                blU = left; blV = bottom;
                                brU = right; brV = bottom;
                                tlU = left; tlV = top;
                                trU = right; trV = top;
                            } else {
                                float left = rectPixels.origin.x / texW;
                                float right = left + rectPixels.size.height / texW;
                                float top = rectPixels.origin.y / texH;
                                float bottom = top + rectPixels.size.width / texH;

                                blU = left; blV = top;
                                brU = left; brV = bottom;
                                tlU = right; tlV = top;
                                trU = right; trV = bottom;
                            }

                            float frameH = rectPoints.size.height > 0.f ? rectPoints.size.height : 1.f;
                            float scale = commonHeight / frameH * item.emoji->scale;

                            float w = rectPoints.size.width * scale;
                            float h = rectPoints.size.height * scale;
                            float qx = x;
                            float qy = y - (commonHeight - h) * 0.5f;

                            if (item.emoji->flipX) {
                                std::swap(blU, brU);
                                std::swap(tlU, trU);
                            }

                            float offX = item.emoji->flipX ? -item.emoji->offset.x : item.emoji->offset.x;
                            qx += offX * scale;
                            qy += item.emoji->offset.y * scale;

                            auto emojiColor = this->getEmojiColorTint(item.emoji);

                            batch.quads.push_back({
                                .bl = { .vertices = { qx, qy - h }, .colors = emojiColor, .texCoords = { blU, blV } },
                                .br = { .vertices = { qx + w, qy - h }, .colors = emojiColor, .texCoords = { brU, brV } },
                                .tl = { .vertices = { qx, qy }, .colors = emojiColor, .texCoords = { tlU, tlV } },
                                .tr = { .vertices = { qx + w, qy }, .colors = emojiColor, .texCoords = { trU, trV } },
                            });

                            m_glyphs.push_back(GlyphInfo{
                                CCRect{ qx, qy - h, w, h },
                                item.srcIndex, item.srcLength, GlyphKind::Emoji,
                            });

                            m_emojiQuads.push_back({
                                .batchIndex = item.fontIndex,
                                .quadIndex = static_cast<uint32_t>(batch.quads.size() - 1),
                                .emoji = item.emoji,
                            });

                            lineMaxX = std::max(lineMaxX, x + item.advance);
                            break;
                        }
                        case ShapedItem::Kind::Space:
                        case ShapedItem::Kind::Newline:
                            break;
                    }

                    x += item.advance;
                    if (extraPerGap != 0.f && item.kind == ShapedItem::Kind::Space) {
                        x += extraPerGap;
                    }
                    lineMaxX = std::max(lineMaxX, x);
                }
            }

            maxLineWidth = std::max(maxLineWidth, lineMaxX);
            y -= lineHeight;
        }

        for (size_t i = 0; i < m_shaped.size(); ++i) {
            if (m_penCovered[i]) continue;

            if (i == 0) {
                m_itemPens[i] = CCPoint{ 0.f, totalHeight };
            } else {
                auto const& prev = m_shaped[i - 1];
                auto const& prevPen = m_itemPens[i - 1];
                if (prev.kind == ShapedItem::Kind::Newline) {
                    m_itemPens[i] = CCPoint{ 0.f, prevPen.y - lineHeight };
                } else {
                    m_itemPens[i] = CCPoint{ prevPen.x + prev.advance, prevPen.y };
                }
            }
        }

        m_label->setContentSize({ maxLineWidth, lineHeight * m_lines.size() });
        m_sizeDirty = true;
    }

    void updateQuads() {
        auto color = this->getEffectiveColor();

        if (!m_useRichText || m_styles.empty()) {
            for (auto& batch : m_batches) {
                for (auto& quad : batch.quads) {
                    quad.bl.colors = color;
                    quad.br.colors = color;
                    quad.tl.colors = color;
                    quad.tr.colors = color;
                }
                batch.isDirty = true;
            }
        } else {
            size_t charIdx = 0;
            size_t styleIdx = 0;

            for (uint32_t ii = 0; ii < m_shaped.size(); ++ii) {
                if (m_shaped[ii].kind != ShapedItem::Kind::Glyph) continue;

                while (styleIdx + 1 < m_styles.size() && ii >= m_styles[styleIdx].end) {
                    ++styleIdx;
                }

                auto const& ref = m_chars[charIdx++];
                auto& quad = m_batches[ref.batchIndex].quads[ref.charIndex];

                auto quadColor = styleIdx < m_styles.size() && m_styles[styleIdx].color
                    ? this->getEffectiveColor(*m_styles[styleIdx].color)
                    : color;

                quad.bl.colors = quadColor;
                quad.br.colors = quadColor;
                quad.tl.colors = quadColor;
                quad.tr.colors = quadColor;
            }

            for (auto& batch : m_batches) {
                batch.isDirty = true;
            }
        }

        for (auto& batch : m_emojiBatches) {
            batch.isDirty = true;
        }

        for (auto const& ref : m_emojiQuads) {
            auto& quad = m_emojiBatches[ref.batchIndex].quads[ref.quadIndex];
            auto emojiColor = this->getEmojiColorTint(ref.emoji);
            quad.bl.colors = emojiColor;
            quad.br.colors = emojiColor;
            quad.tl.colors = emojiColor;
            quad.tr.colors = emojiColor;
        }

        for (auto& node : m_embeddedNodes) {
            if (auto* rgba = typeinfo_cast<CCRGBAProtocol*>(node.data())) {
                if (m_customNodesColors) rgba->setColor(m_color);
                rgba->setOpacity(m_opacity);
            }
        }
    }

    void applyLimitScale() {
        if (!m_sizeDirty || m_fitLabelSize || m_limitSize.width <= 0.f && m_limitSize.height <= 0.f) {
            return;
        }

        m_sizeDirty = false;

        auto size = m_label->CCNode::getContentSize();
        if (size.width <= 0.f || size.height <= 0.f) {
            return;
        }

        float scale = m_defaultScale;
        if (m_limitSize.width > 0.f) {
            scale = std::min(scale, m_limitSize.width / size.width);
        }
        if (m_limitSize.height > 0.f) {
            scale = std::min(scale, m_limitSize.height / size.height);
        }
        if (m_minScale > 0.f) {
            scale = std::max(scale, m_minScale);
        }

        m_label->setScale(scale);
    }

    void applyFitBox() {
        if (m_limitSize.width <= 0.f && m_limitSize.height <= 0.f) {
            this->breakLines();
            return;
        }

        m_lineBreak = false;
        this->breakLinesSimple();

        float naturalWidth = 0.f;
        for (auto const& line : m_lines) naturalWidth = std::max(naturalWidth, line.width);

        if (m_words.empty() || naturalWidth <= 0.f) {
            m_label->setScale(m_defaultScale);
            return;
        }

        float lineHeight = m_fonts.empty() ? 0.f : m_fonts.front()->getCommonHeightScaled() + m_extraLineSpacing;

        m_lineBreak = true;

        constexpr auto ITERATIONS = 14;
        constexpr float FLOAT_MAX = std::numeric_limits<float>::max();
        constexpr float MIN_WIDTH_RATIO = 0.95f;

        auto eval = [&](float wrap, float& outMinW, float& outMaxW, float& rawW, float& rawH) {
            m_maxLineWidth = wrap;
            this->breakLines();

            float maxW = 0.f;
            float minW = FLOAT_MAX;
            for (auto const& line : m_lines) {
                maxW = std::max(maxW, line.width);
                minW = std::min(minW, line.width);
            }

            outMinW = minW;
            outMaxW = maxW;

            float h = lineHeight * m_lines.size();
            rawW = m_limitSize.width  > 0.f && maxW > 0.f ? m_limitSize.width / maxW : FLOAT_MAX;
            rawH = m_limitSize.height > 0.f && h > 0.f ? m_limitSize.height / h : FLOAT_MAX;

            return std::min({rawW, rawH, m_defaultScale});
        };

        float minW = 0.f;
        float maxW = 0.f;
        float widthRatio = 0.f;
        float heightRatio = 0.f;

        float bestWrap = std::max(naturalWidth, 1.f);
        float bestScale = eval(bestWrap, minW, maxW, widthRatio, heightRatio);
        float bestBalance = minW;

        float lo = 1.f;
        float hi = bestWrap;

        for (int i = 0; i < ITERATIONS; ++i) {
            float mid = (lo + hi) * 0.5f;
            float scale = eval(mid, minW, maxW, widthRatio, heightRatio);

            // make sure the label fills at least 95% of the available width, so it doesn't wrap too early
            bool withinTolerance = scale >= bestScale * MIN_WIDTH_RATIO;

            if (scale > bestScale) {
                bestScale = scale;
                bestWrap = mid;
                bestBalance = minW;
            } else if (withinTolerance && minW > bestBalance) {
                bestWrap = mid;
                bestBalance = minW;
            }

            if (widthRatio > heightRatio) {
                lo = mid;
            } else {
                hi = mid;
            }
        }

        m_maxLineWidth = bestWrap;
        this->breakLines();

        float h = lineHeight * m_lines.size();
        float finalW = 0.f;
        for (auto const& line : m_lines) {
            finalW = std::max(finalW, line.width);
        }

        float finalWidthRatio = m_limitSize.width  > 0.f && finalW > 0.f ? m_limitSize.width / finalW : FLOAT_MAX;
        float finalHeightRatio = m_limitSize.height > 0.f && h > 0.f ? m_limitSize.height / h : FLOAT_MAX;

        m_label->setScale(std::min({finalWidthRatio, finalHeightRatio, m_defaultScale}));
    }

    void validate() {
        if (m_limitChanged && m_fitLabelSize) {
            m_layoutDirty = true;
        }

        if (!m_textDirty && !m_layoutDirty && !m_quadsDirty) {
            if (m_limitChanged) {
                this->applyLimitScale();
                m_limitChanged = false;
            }
            return;
        }

        if (m_textDirty) {
            this->reshape();
            this->splitWords();

            if (!m_fitLabelSize) {
                this->breakLines();
            }

            m_textDirty = false;
            m_layoutDirty = true;
        }

        if (m_layoutDirty) {
            if (m_fitLabelSize) {
                this->applyFitBox();
            }

            this->emitQuads();
            m_layoutDirty = false;
            m_quadsDirty = false;
        } else if (m_quadsDirty) {
            this->updateQuads();
            m_quadsDirty = false;
        }

        m_limitChanged = false;
        this->applyLimitScale();
    }
};

EmojiRegistry::EmojiEntry::EmojiEntry(
    ZStringView frameName,
    cocos2d::CCPoint offset,
    float scale,
    cocos2d::ccColor3B tint,
    bool flipX
) noexcept : frameName(frameName), offset(offset), scale(scale), tint(tint), flipX(flipX) {}

EmojiRegistry::EmojiEntry::EmojiEntry(ZStringView frameName) noexcept : frameName(frameName) {}

CCSpriteFrame* EmojiRegistry::EmojiEntry::getFrame() const {
    if (!cachedFrame) {
        cachedFrame = CCSpriteFrameCache::get()->spriteFrameByName(frameName.c_str());
    }
    return cachedFrame;
}

void EmojiRegistry::EmojiEntry::invalidate() noexcept {
    cachedFrame = nullptr;
}

EmojiRegistry::Entry::Entry(EmojiEntry emoji) : m_storage(std::move(emoji)) {}

EmojiRegistry::Entry::Entry(NodeFactory factory)
  : m_storage(std::make_shared<NodeFactory>(std::move(factory))) {}

EmojiRegistry::Entry::Entry(NodeFactoryParams factory)
  : m_storage(std::make_shared<NodeFactoryParams>(std::move(factory))) {}

EmojiRegistry::Entry::Entry(ZStringView frameName) : m_storage(EmojiEntry(frameName)) {}

EmojiRegistry::EmojiEntry const* EmojiRegistry::Entry::getEmoji() const noexcept {
    return std::get_if<EmojiEntry>(&m_storage);
}

EmojiRegistry::EmojiEntry* EmojiRegistry::Entry::getEmoji() noexcept {
    return std::get_if<EmojiEntry>(&m_storage);
}

EmojiRegistry::NodeFactory const* EmojiRegistry::Entry::getNodeFactory() const noexcept {
    auto* fn = std::get_if<std::shared_ptr<NodeFactory>>(&m_storage);
    return fn ? fn->get() : nullptr;
}

EmojiRegistry::NodeFactory* EmojiRegistry::Entry::getNodeFactory() noexcept {
    auto* fn = std::get_if<std::shared_ptr<NodeFactory>>(&m_storage);
    return fn ? fn->get() : nullptr;
}

EmojiRegistry::NodeFactoryParams const* EmojiRegistry::Entry::getNodeFactoryParams() const noexcept {
    auto* fn = std::get_if<std::shared_ptr<NodeFactoryParams>>(&m_storage);
    return fn ? fn->get() : nullptr;
}

EmojiRegistry::NodeFactoryParams* EmojiRegistry::Entry::getNodeFactoryParams() noexcept {
    auto* fn = std::get_if<std::shared_ptr<NodeFactoryParams>>(&m_storage);
    return fn ? fn->get() : nullptr;
}

EmojiRegistry::EmojiRegistry() : m_impl(std::make_shared<Impl>()) {}
EmojiRegistry::EmojiRegistry(std::shared_ptr<Impl> impl) : m_impl(std::move(impl)) {}
EmojiRegistry::~EmojiRegistry() = default;

EmojiRegistry::EmojiRegistry(EmojiRegistry const&) = default;
EmojiRegistry::EmojiRegistry(EmojiRegistry&&) noexcept = default;
EmojiRegistry& EmojiRegistry::operator=(EmojiRegistry const&) = default;
EmojiRegistry& EmojiRegistry::operator=(EmojiRegistry&&) noexcept = default;

void EmojiRegistry::insert(std::u32string_view sequence, NodeFactory factory) {
    m_impl->insert(sequence, std::move(factory));
}

void EmojiRegistry::insert(std::u32string_view sequence, NodeFactoryParams factory) {
    m_impl->insert(sequence, std::move(factory));
}

void EmojiRegistry::insert(std::u32string_view sequence, ZStringView frameName) {
    m_impl->insert(sequence, EmojiEntry(frameName));
}

void EmojiRegistry::insert(std::span<std::tuple<std::u32string_view, Entry> const> entries) {
    m_impl->insert(entries);
}

void EmojiRegistry::insert(std::span<std::pair<std::u32string_view, Entry> const> entries) {
    for (auto const& [sequence, entry] : entries) {
        m_impl->insert(sequence, entry);
    }
}

void EmojiRegistry::merge(EmojiRegistry const& other) {
    if (!other.m_impl || other.m_impl == m_impl) {
        return;
    }

    m_impl->m_keys.reserve(m_impl->m_keys.size() + other.m_impl->m_keys.size());
    m_impl->m_entries.reserve(m_impl->m_entries.size() + other.m_impl->m_entries.size());

    for (size_t i = 0; i < other.m_impl->m_keys.size(); ++i) {
        m_impl->insert(other.m_impl->m_keys[i], other.m_impl->m_entries[i]);
    }
}

void EmojiRegistry::erase(std::u32string_view sequence) {
    m_impl->erase(sequence);
}

EmojiRegistry::Entry* EmojiRegistry::match(std::u32string_view str, size_t& index) {
    return m_impl->match(str, index);
}

EmojiRegistry EmojiRegistry::clone() const {
    if (!m_impl) {
        return EmojiRegistry();
    }
    return EmojiRegistry(std::make_shared<Impl>(*m_impl));
}

void EmojiRegistry::clear() noexcept {
    m_impl->m_keys.clear();
    m_impl->m_entries.clear();
}

bool EmojiRegistry::contains(std::u32string_view sequence) const noexcept {
    return m_impl->find(sequence) != nullptr;
}

size_t EmojiRegistry::size() const noexcept {
    return m_impl->m_keys.size();
}

bool EmojiRegistry::empty() const noexcept {
    return m_impl->m_keys.empty();
}

std::span<std::u32string_view const> EmojiRegistry::getSequences() const noexcept {
    return m_impl->m_keys;
}

EmojiRegistry::Entry const* EmojiRegistry::find(std::u32string_view sequence) const noexcept {
    return m_impl->find(sequence);
}

EmojiRegistry const& EmojiRegistry::vanillaEmojis() {
    static EmojiRegistry registry = [] {
        using E = std::pair<std::u32string_view, Entry>;
        static auto const table = std::to_array<E>({
            E{U":100:", "particle_354_001.png"},
            E{U":98:", "particle_236_001.png"},
            E{U":adore:", EmojiEntry{"particle_339_001.png", {0.f, 0.f}, 1.1f, {255, 255, 255}, false}},
            E{U":angel:", EmojiEntry{"particle_293_001.png", {0.f, 1.2f}, 1.2f, {255, 255, 255}, false}},
            E{U":angry:", "particle_261_001.png"},
            E{U":annoyed:", "particle_263_001.png"},
            E{U":arrow:", "particle_12_001.png"},
            E{U":arrow_left:", EmojiEntry{"particle_12_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U":arrow_right:", "particle_12_001.png"},
            E{U":arrowleft:", EmojiEntry{"particle_12_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U":arrowright:", "particle_12_001.png"},
            E{U":astonished:", "particle_275_001.png"},
            E{U":auto:", "particle_63_001.png"},
            E{U":ban:", EmojiEntry{"particle_203_001.png", {0.f, 0.f}, 1.f, {0xff, 0x32, 0x32}, false}},
            E{U":bawl:", "particle_267_001.png"},
            E{U":beam:", "particle_328_001.png"},
            E{U":beast1:", EmojiEntry{"particle_341_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":beast2:", EmojiEntry{"particle_342_001.png", {0.f, 0.f}, 1.1f, {255, 255, 255}, false}},
            E{U":beast3:", EmojiEntry{"particle_343_001.png", {0.f, 1.f}, 1.1f, {255, 255, 255}, false}},
            E{U":beast4:", "particle_344_001.png"},
            E{U":beast5:", "particle_345_001.png"},
            E{U":bell:", "particle_346_001.png"},
            E{U":blank:", "particle_250_001.png"},
            E{U":blob:", "particle_324_001.png"},
            E{U":blueblock:", "particle_322_001.png"},
            E{U":bluechest:", "particle_224_001.png"},
            E{U":bluehuh:", "particle_323_001.png"},
            E{U":blush:", "particle_252_001.png"},
            E{U":bolt:", EmojiEntry{"particle_151_001.png", {0.f, 0.f}, 0.85f, {0xff, 0xdc, 0x41}, false}},
            E{U":bomb:", "particle_315_001.png"},
            E{U":brick:", "particle_221_001.png"},
            E{U":bricks:", "particle_221_001.png"},
            E{U":broken_heart:", "particle_356_001.png"},
            E{U":brokenheart:", "particle_356_001.png"},
            E{U":bruh:", "particle_310_001.png"},
            E{U":chart:", EmojiEntry{"particle_336_001.png", {0.f, -1.5f}, 1.2f, {255, 255, 255}, false}},
            E{U":chartdown:", EmojiEntry{"particle_338_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U":chartup:", EmojiEntry{"particle_337_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U":check:", EmojiEntry{"particle_50_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":checkered_flag:", "particle_333_001.png"},
            E{U":checkpoint:", "particle_217_001.png"},
            E{U":chest:", "particle_223_001.png"},
            E{U":clap:", "particle_300_001.png"},
            E{U":clock:", "particle_197_001.png"},
            E{U":clown:", "particle_285_001.png"},
            E{U":coin:", "particle_311_001.png"},
            E{U":confused:", EmojiEntry{"particle_295_001.png", {0.f, 1.4f}, 1.2f, {255, 255, 255}, false}},
            E{U":cool:", "particle_268_001.png"},
            E{U":crescent_moon:", "particle_208_001.png"},
            E{U":cross:", EmojiEntry{"particle_193_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":crossed_fingers:", "particle_347_001.png"},
            E{U":crown:", "particle_313_001.png"},
            E{U":cry:", "particle_332_001.png"},
            E{U":cube:", "particle_213_001.png"},
            E{U":cubeface:", "particle_260_001.png"},
            E{U":dead:", "particle_248_001.png"},
            E{U":deathmarker:", "particle_326_001.png"},
            E{U":demonchest:", "particle_225_001.png"},
            E{U":devil:", EmojiEntry{"particle_294_001.png", {0.f, 0.5f}, 1.1f, {255, 255, 255}, false}},
            E{U":diamond:", EmojiEntry{"particle_51_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U":dislike:", "particle_49_001.png"},
            E{U":dizzy:", "particle_270_001.png"},
            E{U":dots:", "particle_219_001.png"},
            E{U":doubt:", "particle_296_001.png"},
            E{U":download:", "particle_201_001.png"},
            E{U":easy:", "particle_53_001.png"},
            E{U":easydemon:", EmojiEntry{"particle_58_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":envelope:", "particle_207_001.png"},
            E{U":epic:", "particle_228_001.png"},
            E{U":evil:", "particle_329_001.png"},
            E{U":exclaim:", EmojiEntry{"particle_10_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U":exclamation:", EmojiEntry{"particle_10_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U":exploding_head:", "particle_358_001.png"},
            E{U":extremedemon:", EmojiEntry{"particle_62_001.png", {0.f, 0.f}, 1.4f, {255, 255, 255}, false}},
            E{U":eyeroll:", "particle_348_001.png"},
            E{U":eyes:", EmojiEntry{"particle_298_001.png", {0.f, 0.f}, 0.85f, {255, 255, 255}, false}},
            E{U":ez:", "particle_235_001.png"},
            E{U":facepalm:", "particle_282_001.png"},
            E{U":favorite:", EmojiEntry{"particle_199_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":favourite:", EmojiEntry{"particle_199_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":featured:", EmojiEntry{"particle_227_001.png", {0.f, 0.f}, 0.95f, {255, 255, 255}, false}},
            E{U":fingerscrossed:", "particle_347_001.png"},
            E{U":fire:", "particle_228_001.png"},
            E{U":fireskull:", "particle_220_001.png"},
            E{U":fist:", "particle_304_001.png"},
            E{U":flag:", "particle_333_001.png"},
            E{U":flex:", "particle_303_001.png"},
            E{U":floppy_disk:", "particle_216_001.png"},
            E{U":gasp:", "particle_264_001.png"},
            E{U":gear:", "particle_200_001.png"},
            E{U":gem:", EmojiEntry{"particle_51_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U":gg:", "particle_233_001.png"},
            E{U":ghost:", EmojiEntry{"particle_152_001.png", {0.f, 0.f}, 0.95f, {255, 255, 255}, false}},
            E{U":globe:", "particle_335_001.png"},
            E{U":go:", "particle_234_001.png"},
            E{U":goldblock:", "particle_318_001.png"},
            E{U":greenblock:", "particle_319_001.png"},
            E{U":grin:", "particle_327_001.png"},
            E{U":grinning:", "particle_327_001.png"},
            E{U":gross:", "particle_277_001.png"},
            E{U":grr:", "particle_261_001.png"},
            E{U":handshake:", EmojiEntry{"particle_302_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U":happy:", "particle_242_001.png"},
            E{U":hard:", "particle_55_001.png"},
            E{U":harddemon:", EmojiEntry{"particle_60_001.png", {0.f, 0.f}, 1.17f, {255, 255, 255}, false}},
            E{U":harder:", "particle_56_001.png"},
            E{U":heart:", "particle_355_001.png"},
            E{U":heart_eyes:", "particle_254_001.png"},
            E{U":heartbubble:", "particle_231_001.png"},
            E{U":hide:", "particle_283_001.png"},
            E{U":house:", "particle_334_001.png"},
            E{U":hug:", "particle_351_001.png"},
            E{U":hugging:", "particle_351_001.png"},
            E{U":huh:", "particle_238_001.png"},
            E{U":hundred:", "particle_354_001.png"},
            E{U":hyped:", "particle_246_001.png"},
            E{U":ill:", "particle_278_001.png"},
            E{U":innocent:", EmojiEntry{"particle_293_001.png", {0.f, 1.2f}, 1.2f, {255, 255, 255}, false}},
            E{U":insane:", "particle_57_001.png"},
            E{U":insanedemon:", EmojiEntry{"particle_61_001.png", {0.f, 0.f}, 1.35f, {255, 255, 255}, false}},
            E{U":joy:", "particle_244_001.png"},
            E{U":judge:", "particle_349_001.png"},
            E{U":key:", "particle_314_001.png"},
            E{U":keydemon:", EmojiEntry{"particle_205_001.png", {0.f, 0.f}, 1.1f, {255, 255, 255}, false}},
            E{U":laugh:", "particle_243_001.png"},
            E{U":laughing:", "particle_243_001.png"},
            E{U":legendary:", EmojiEntry{"particle_229_001.png", {0.f, 0.f}, 1.05f, {255, 255, 255}, false}},
            E{U":like:", "particle_48_001.png"},
            E{U":lock:", "particle_194_001.png"},
            E{U":lol:", "particle_244_001.png"},
            E{U":love:", "particle_254_001.png"},
            E{U":mail:", "particle_207_001.png"},
            E{U":mediumdemon:", EmojiEntry{"particle_59_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":meh:", "particle_24_001.png"},
            E{U":mindblown:", "particle_358_001.png"},
            E{U":monocle:", EmojiEntry{"particle_291_001.png", {0.f, 1.6f}, 1.22f, {255, 255, 255}, false}},
            E{U":moon:", "particle_208_001.png"},
            E{U":moyai:", "particle_310_001.png"},
            E{U":muscle:", "particle_303_001.png"},
            E{U":music:", "particle_202_001.png"},
            E{U":musical_note:", "particle_202_001.png"},
            E{U":mystery:", "particle_325_001.png"},
            E{U":mythic:", EmojiEntry{"particle_230_001.png", {0.f, 0.f}, 1.1f, {255, 255, 255}, false}},
            E{U":na:", "particle_52_001.png"},
            E{U":nerd:", "particle_359_001.png"},
            E{U":nerd_face:", "particle_359_001.png"},
            E{U":neutral_face:", "particle_250_001.png"},
            E{U":ninja:", "particle_262_001.png"},
            E{U":no:", EmojiEntry{"particle_193_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":normal:", "particle_54_001.png"},
            E{U":nuke:", "particle_316_001.png"},
            E{U":ok:", "particle_305_001.png"},
            E{U":ok_hand:", "particle_305_001.png"},
            E{U":open_mouth:", "particle_264_001.png"},
            E{U":orangeblock:", "particle_321_001.png"},
            E{U":orb:", "particle_195_001.png"},
            E{U":party:", EmojiEntry{"particle_255_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":partying_face:", EmojiEntry{"particle_255_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":peace:", "particle_306_001.png"},
            E{U":pensive:", "particle_361_001.png"},
            E{U":phew:", "particle_272_001.png"},
            E{U":pink_heart:", "particle_357_001.png"},
            E{U":pinkblock:", "particle_320_001.png"},
            E{U":pinkheart:", "particle_357_001.png"},
            E{U":plead:", "particle_256_001.png"},
            E{U":pleading_face:", "particle_256_001.png"},
            E{U":pls:", "particle_256_001.png"},
            E{U":point:", "particle_307_001.png"},
            E{U":point_left:", "particle_307_001.png"},
            E{U":point_right:", EmojiEntry{"particle_307_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U":pointleft:", "particle_307_001.png"},
            E{U":pointright:", EmojiEntry{"particle_307_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U":poop:", "particle_308_001.png"},
            E{U":popcorn:", "particle_309_001.png"},
            E{U":pray:", "particle_301_001.png"},
            E{U":puke:", "particle_279_001.png"},
            E{U":puppyeyes:", "particle_253_001.png"},
            E{U":quest:", "particle_218_001.png"},
            E{U":question:", EmojiEntry{"particle_11_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U":rage:", "particle_57_001.png"},
            E{U":raised_hands:", "particle_352_001.png"},
            E{U":raisedhands:", "particle_352_001.png"},
            E{U":rate:", "particle_232_001.png"},
            E{U":redblock:", "particle_317_001.png"},
            E{U":relieved:", "particle_259_001.png"},
            E{U":rolling_eyes:", "particle_348_001.png"},
            E{U":royal:", EmojiEntry{"particle_226_001.png", {0.f, 0.f}, 0.85f, {255, 255, 255}, false}},
            E{U":sad:", "particle_240_001.png"},
            E{U":sadclown:", "particle_286_001.png"},
            E{U":salute:", "particle_284_001.png"},
            E{U":save:", "particle_216_001.png"},
            E{U":sceptical:", "particle_360_001.png"},
            E{U":scream:", "particle_274_001.png"},
            E{U":shadow:", "particle_280_001.png"},
            E{U":shocked:", "particle_274_001.png"},
            E{U":shrug:", "particle_287_001.png"},
            E{U":sick:", "particle_276_001.png"},
            E{U":silvercoin:", "particle_312_001.png"},
            E{U":sir:", EmojiEntry{"particle_292_001.png", {0.f, 1.6f}, 1.22f, {255, 255, 255}, false}},
            E{U":skeptical:", "particle_360_001.png"},
            E{U":skull:", "particle_215_001.png"},
            E{U":skullbomb:", "particle_316_001.png"},
            E{U":sleeping:", "particle_290_001.png"},
            E{U":sleepy:", "particle_288_001.png"},
            E{U":slight_smile:", "particle_239_001.png"},
            E{U":slightsmile:", "particle_239_001.png"},
            E{U":smile:", "particle_327_001.png"},
            E{U":smiley:", "particle_23_001.png"},
            E{U":smiling_imp:", EmojiEntry{"particle_294_001.png", {0.f, 0.5f}, 1.1f, {255, 255, 255}, false}},
            E{U":smirk:", "particle_257_001.png"},
            E{U":smug:", "particle_258_001.png"},
            E{U":sniff:", "particle_330_001.png"},
            E{U":snore:", "particle_290_001.png"},
            E{U":snow:", EmojiEntry{"particle_14_001.png", {0.f, 0.f}, 0.9f, {0xa0, 0xe1, 0xff}, false}},
            E{U":snowflake:", EmojiEntry{"particle_14_001.png", {0.f, 0.f}, 0.9f, {0xa0, 0xe1, 0xff}, false}},
            E{U":sob:", "particle_266_001.png"},
            E{U":sparkles:", "particle_353_001.png"},
            E{U":spike:", "particle_214_001.png"},
            E{U":star:", EmojiEntry{"particle_47_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U":star_struck:", "particle_245_001.png"},
            E{U":stare:", "particle_249_001.png"},
            E{U":starstruck:", "particle_245_001.png"},
            E{U":sun:", EmojiEntry{"particle_281_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":sunglasses:", "particle_268_001.png"},
            E{U":sus:", "particle_241_001.png"},
            E{U":swear:", "particle_237_001.png"},
            E{U":sweat:", "particle_271_001.png"},
            E{U":sweat_smile:", "particle_272_001.png"},
            E{U":tada:", EmojiEntry{"particle_255_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":teary:", "particle_331_001.png"},
            E{U":think:", "particle_251_001.png"},
            E{U":thinking:", "particle_251_001.png"},
            E{U":thug:", "particle_269_001.png"},
            E{U":thumbsdown:", "particle_49_001.png"},
            E{U":thumbsup:", "particle_48_001.png"},
            E{U":tired:", "particle_289_001.png"},
            E{U":tired_face:", "particle_289_001.png"},
            E{U":tongue:", "particle_265_001.png"},
            E{U":tools:", "particle_196_001.png"},
            E{U":trophy:", "particle_222_001.png"},
            E{U":unamused:", "particle_263_001.png"},
            E{U":usercoin:", "particle_312_001.png"},
            E{U":wave:", "particle_299_001.png"},
            E{U":wink:", "particle_247_001.png"},
            E{U":wow:", "particle_275_001.png"},
            E{U":wraith:", EmojiEntry{"particle_340_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U":x:", EmojiEntry{"particle_193_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":yay:", "particle_242_001.png"},
            E{U":yes:", EmojiEntry{"particle_50_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U":yikes:", "particle_273_001.png"},
            E{U":yum:", "particle_350_001.png"},
            E{U":zap:", EmojiEntry{"particle_151_001.png", {0.f, 0.f}, 0.85f, {0xff, 0xdc, 0x41}, false}},
            E{U":zip:", "particle_297_001.png"},
            E{U":zipper_mouth:", "particle_297_001.png"},
            E{U":zzz:", "particle_290_001.png"},
            E{U"‼", EmojiEntry{"particle_10_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U"⌚", "particle_197_001.png"},
            E{U"⌛", "particle_197_001.png"},
            E{U"⏰", "particle_197_001.png"},
            E{U"⏳", "particle_197_001.png"},
            E{U"☀", EmojiEntry{"particle_281_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U"☠", "particle_215_001.png"},
            E{U"☹", "particle_240_001.png"},
            E{U"⚙", "particle_200_001.png"},
            E{U"⚡", EmojiEntry{"particle_151_001.png", {0.f, 0.f}, 0.85f, {0xff, 0xdc, 0x41}, false}},
            E{U"⛔", EmojiEntry{"particle_203_001.png", {0.f, 0.f}, 1.f, {0xff, 0x32, 0x32}, false}},
            E{U"✅", EmojiEntry{"particle_50_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U"✉", "particle_207_001.png"},
            E{U"✊", "particle_304_001.png"},
            E{U"✌", "particle_306_001.png"},
            E{U"✔", EmojiEntry{"particle_50_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U"✨", "particle_353_001.png"},
            E{U"❄", EmojiEntry{"particle_14_001.png", {0.f, 0.f}, 0.9f, {0xa0, 0xe1, 0xff}, false}},
            E{U"❌", EmojiEntry{"particle_193_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U"❎", EmojiEntry{"particle_193_001.png", {0.f, 0.f}, 0.92f, {255, 255, 255}, false}},
            E{U"❓", EmojiEntry{"particle_11_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U"❔", EmojiEntry{"particle_11_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U"❗", EmojiEntry{"particle_10_001.png", {0.f, 0.f}, 0.92f, {0xff, 0x32, 0x32}, false}},
            E{U"❤", "particle_355_001.png"},
            E{U"➡", "particle_12_001.png"},
            E{U"⬅", EmojiEntry{"particle_12_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U"⬇", "particle_201_001.png"},
            E{U"⭐", EmojiEntry{"particle_47_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U"🌍", "particle_335_001.png"},
            E{U"🌎", "particle_335_001.png"},
            E{U"🌏", "particle_335_001.png"},
            E{U"🌙", "particle_208_001.png"},
            E{U"🌛", "particle_208_001.png"},
            E{U"🌞", EmojiEntry{"particle_281_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U"🌟", EmojiEntry{"particle_47_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U"🍿", "particle_309_001.png"},
            E{U"🎁", "particle_223_001.png"},
            E{U"🎉", EmojiEntry{"particle_255_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U"🎩", EmojiEntry{"particle_292_001.png", {0.f, 1.6f}, 1.22f, {255, 255, 255}, false}},
            E{U"🎵", "particle_202_001.png"},
            E{U"🎶", "particle_202_001.png"},
            E{U"🏁", "particle_333_001.png"},
            E{U"🏆", "particle_222_001.png"},
            E{U"🏠", "particle_334_001.png"},
            E{U"👀", EmojiEntry{"particle_298_001.png", {0.f, 0.f}, 0.85f, {255, 255, 255}, false}},
            E{U"👈", "particle_307_001.png"},
            E{U"👉", EmojiEntry{"particle_307_001.png", {0.f, 0.f}, 1.f, {255, 255, 255}, true}},
            E{U"👊", "particle_304_001.png"},
            E{U"👋", "particle_299_001.png"},
            E{U"👌", "particle_305_001.png"},
            E{U"👍", "particle_48_001.png"},
            E{U"👎", "particle_49_001.png"},
            E{U"👏", "particle_300_001.png"},
            E{U"👑", "particle_313_001.png"},
            E{U"👻", EmojiEntry{"particle_152_001.png", {0.f, 0.f}, 0.95f, {255, 255, 255}, false}},
            E{U"👿", "particle_329_001.png"},
            E{U"💀", "particle_215_001.png"},
            E{U"💎", EmojiEntry{"particle_51_001.png", {0.f, 0.f}, 1.08f, {255, 255, 255}, false}},
            E{U"💔", "particle_356_001.png"},
            E{U"💕", "particle_357_001.png"},
            E{U"💖", "particle_357_001.png"},
            E{U"💗", "particle_357_001.png"},
            E{U"💙", "particle_355_001.png"},
            E{U"💚", "particle_355_001.png"},
            E{U"💛", "particle_355_001.png"},
            E{U"💜", "particle_355_001.png"},
            E{U"💣", "particle_315_001.png"},
            E{U"💤", "particle_290_001.png"},
            E{U"💩", "particle_308_001.png"},
            E{U"💪", "particle_303_001.png"},
            E{U"💬", "particle_219_001.png"},
            E{U"💭", "particle_219_001.png"},
            E{U"💯", "particle_354_001.png"},
            E{U"💾", "particle_216_001.png"},
            E{U"📈", EmojiEntry{"particle_337_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U"📉", EmojiEntry{"particle_338_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U"📊", EmojiEntry{"particle_336_001.png", {0.f, -1.5f}, 1.2f, {255, 255, 255}, false}},
            E{U"📥", "particle_201_001.png"},
            E{U"📧", "particle_207_001.png"},
            E{U"📨", "particle_207_001.png"},
            E{U"📩", "particle_207_001.png"},
            E{U"🔑", "particle_314_001.png"},
            E{U"🔒", "particle_194_001.png"},
            E{U"🔔", "particle_346_001.png"},
            E{U"🔥", "particle_228_001.png"},
            E{U"🔧", "particle_196_001.png"},
            E{U"🔮", "particle_195_001.png"},
            E{U"🕐", "particle_197_001.png"},
            E{U"🕑", "particle_197_001.png"},
            E{U"🕒", "particle_197_001.png"},
            E{U"🕓", "particle_197_001.png"},
            E{U"🕔", "particle_197_001.png"},
            E{U"🕕", "particle_197_001.png"},
            E{U"🕖", "particle_197_001.png"},
            E{U"🕗", "particle_197_001.png"},
            E{U"🕘", "particle_197_001.png"},
            E{U"🕙", "particle_197_001.png"},
            E{U"🕚", "particle_197_001.png"},
            E{U"🕛", "particle_197_001.png"},
            E{U"🕜", "particle_197_001.png"},
            E{U"🕝", "particle_197_001.png"},
            E{U"🕞", "particle_197_001.png"},
            E{U"🕟", "particle_197_001.png"},
            E{U"🕠", "particle_197_001.png"},
            E{U"🕡", "particle_197_001.png"},
            E{U"🕢", "particle_197_001.png"},
            E{U"🕣", "particle_197_001.png"},
            E{U"🕤", "particle_197_001.png"},
            E{U"🕥", "particle_197_001.png"},
            E{U"🕦", "particle_197_001.png"},
            E{U"🕧", "particle_197_001.png"},
            E{U"🖤", "particle_355_001.png"},
            E{U"🗿", "particle_310_001.png"},
            E{U"😀", "particle_327_001.png"},
            E{U"😁", "particle_328_001.png"},
            E{U"😂", "particle_244_001.png"},
            E{U"😃", "particle_242_001.png"},
            E{U"😄", "particle_328_001.png"},
            E{U"😅", "particle_272_001.png"},
            E{U"😆", "particle_243_001.png"},
            E{U"😇", EmojiEntry{"particle_293_001.png", {0.f, 1.2f}, 1.2f, {255, 255, 255}, false}},
            E{U"😈", EmojiEntry{"particle_294_001.png", {0.f, 0.5f}, 1.1f, {255, 255, 255}, false}},
            E{U"😉", "particle_247_001.png"},
            E{U"😊", "particle_252_001.png"},
            E{U"😋", "particle_350_001.png"},
            E{U"😌", "particle_259_001.png"},
            E{U"😍", "particle_254_001.png"},
            E{U"😎", "particle_268_001.png"},
            E{U"😏", "particle_257_001.png"},
            E{U"😐", "particle_250_001.png"},
            E{U"😑", "particle_250_001.png"},
            E{U"😒", "particle_349_001.png"},
            E{U"😓", "particle_271_001.png"},
            E{U"😔", "particle_361_001.png"},
            E{U"😕", EmojiEntry{"particle_295_001.png", {0.f, 1.4f}, 1.2f, {255, 255, 255}, false}},
            E{U"😘", "particle_254_001.png"},
            E{U"😛", "particle_265_001.png"},
            E{U"😜", "particle_265_001.png"},
            E{U"😝", "particle_265_001.png"},
            E{U"😞", "particle_361_001.png"},
            E{U"😠", "particle_261_001.png"},
            E{U"😡", "particle_261_001.png"},
            E{U"😢", "particle_332_001.png"},
            E{U"😤", "particle_261_001.png"},
            E{U"😥", "particle_330_001.png"},
            E{U"😨", "particle_273_001.png"},
            E{U"😩", "particle_289_001.png"},
            E{U"😪", "particle_288_001.png"},
            E{U"😫", "particle_289_001.png"},
            E{U"😬", "particle_273_001.png"},
            E{U"😭", "particle_266_001.png"},
            E{U"😮", "particle_264_001.png"},
            E{U"😯", "particle_264_001.png"},
            E{U"😰", "particle_271_001.png"},
            E{U"😱", "particle_274_001.png"},
            E{U"😲", "particle_275_001.png"},
            E{U"😳", "particle_252_001.png"},
            E{U"😴", "particle_290_001.png"},
            E{U"😵", "particle_248_001.png"},
            E{U"😶", "particle_250_001.png"},
            E{U"😷", "particle_278_001.png"},
            E{U"🙁", "particle_240_001.png"},
            E{U"🙂", "particle_239_001.png"},
            E{U"🙃", "particle_239_001.png"},
            E{U"🙄", "particle_348_001.png"},
            E{U"🙈", "particle_283_001.png"},
            E{U"🙌", "particle_352_001.png"},
            E{U"🙏", "particle_301_001.png"},
            E{U"🚩", "particle_333_001.png"},
            E{U"🚫", EmojiEntry{"particle_203_001.png", {0.f, 0.f}, 1.f, {0xff, 0x32, 0x32}, false}},
            E{U"🛠", "particle_196_001.png"},
            E{U"🟥", "particle_317_001.png"},
            E{U"🟦", "particle_322_001.png"},
            E{U"🟧", "particle_321_001.png"},
            E{U"🟨", "particle_318_001.png"},
            E{U"🟩", "particle_319_001.png"},
            E{U"🟪", "particle_320_001.png"},
            E{U"🤍", "particle_355_001.png"},
            E{U"🤐", "particle_297_001.png"},
            E{U"🤒", "particle_276_001.png"},
            E{U"🤓", "particle_359_001.png"},
            E{U"🤔", "particle_251_001.png"},
            E{U"🤗", "particle_351_001.png"},
            E{U"🤝", EmojiEntry{"particle_302_001.png", {0.f, 0.f}, 0.9f, {255, 255, 255}, false}},
            E{U"🤞", "particle_347_001.png"},
            E{U"🤡", "particle_285_001.png"},
            E{U"🤢", "particle_277_001.png"},
            E{U"🤣", "particle_244_001.png"},
            E{U"🤦", "particle_282_001.png"},
            E{U"🤧", "particle_276_001.png"},
            E{U"🤨", "particle_241_001.png"},
            E{U"🤩", "particle_245_001.png"},
            E{U"🤪", "particle_265_001.png"},
            E{U"🤬", "particle_237_001.png"},
            E{U"🤮", "particle_279_001.png"},
            E{U"🤯", "particle_358_001.png"},
            E{U"🤷", "particle_287_001.png"},
            E{U"🥰", EmojiEntry{"particle_339_001.png", {0.f, 0.f}, 1.1f, {255, 255, 255}, false}},
            E{U"🥱", "particle_290_001.png"},
            E{U"🥲", "particle_331_001.png"},
            E{U"🥳", EmojiEntry{"particle_255_001.png", {0.f, 0.f}, 1.2f, {255, 255, 255}, false}},
            E{U"🥷", "particle_262_001.png"},
            E{U"🥹", "particle_331_001.png"},
            E{U"🥺", "particle_256_001.png"},
            E{U"🧐", EmojiEntry{"particle_291_001.png", {0.f, 1.6f}, 1.22f, {255, 255, 255}, false}},
            E{U"🧡", "particle_355_001.png"},
            E{U"🧱", "particle_221_001.png"},
            E{U"🩷", "particle_357_001.png"},
            E{U"🪙", "particle_311_001.png"},
            E{U"🫡", "particle_284_001.png"},
            E{U"🫣", "particle_283_001.png"},
            E{U"🫤", "particle_296_001.png"},
            E{U"😵‍💫", "particle_270_001.png"},
        });

        EmojiRegistry reg;
        reg.insert(table);
        return reg;
    }();
    return registry;
}

Label::Label() : m_impl(new Impl(this)) {}

Label::~Label() {
    delete m_impl;
}

Label* Label::create(BitmapFont* font) {
    auto ret = new Label();
    if (ret->initWithFont(font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Label* Label::create(ZStringView font) {
    auto ret = new Label();
    if (ret->initWithFontFile(font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Label* Label::create(std::string text, BitmapFont* font) {
    auto ret = new Label();
    if (ret->initWithString(std::move(text), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Label* Label::create(std::string text, ZStringView font) {
    auto ret = new Label();
    if (ret->initWithString(std::move(text), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Label* Label::createRich(std::string text, BitmapFont* font) {
    auto ret = new Label();
    if (ret->initWithRichText(std::move(text), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

Label* Label::createRich(std::string text, ZStringView font) {
    auto ret = new Label();
    if (ret->initWithRichText(std::move(text), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void Label::visit() {
    if (m_bVisible) {
        this->validate();
    }

    CCNode::visit();
}

void Label::draw() {
    if (!m_pShaderProgram) return;

    this->validate();
    if (m_impl->m_shaped.empty()) return;

    ccGLEnable(m_eGLServerState);
    m_pShaderProgram->use();
    m_pShaderProgram->setUniformsForBuiltins();

    ccGLBlendFunc(m_impl->m_blendFunc.src, m_impl->m_blendFunc.dst);
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    size_t maxQuads = 0;

    for (auto& batch : m_impl->m_batches) {
        batch.upload();
        maxQuads = std::max(maxQuads, batch.quads.size());
    }

    for (auto& batch : m_impl->m_emojiBatches) {
        batch.upload();
        maxQuads = std::max(maxQuads, batch.quads.size());
    }

    if (maxQuads == 0) return;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SharedIndexBuffer::get().getIBO(maxQuads));

    for (auto const& batch : m_impl->m_batches) { batch.draw(); }
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto const& batch : m_impl->m_emojiBatches) { batch.draw(); }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Label::validate() {
    m_impl->validate();
}

void Label::setTextDirty(bool dirty) noexcept {
    m_impl->m_textDirty = dirty;
}

void Label::setLayoutDirty(bool dirty) noexcept {
    m_impl->m_layoutDirty = dirty;
}

void Label::setQuadsDirty(bool dirty) noexcept {
    m_impl->m_quadsDirty = dirty;
}

bool Label::isTextDirty() noexcept {
    return m_impl->m_textDirty;
}

bool Label::isLayoutDirty() noexcept {
    return m_impl->m_layoutDirty;
}

bool Label::isQuadsDirty() noexcept {
    return m_impl->m_quadsDirty;
}

bool Label::registerFont(BitmapFont* font) {
    if (!font) return false;

    for (auto const& f : m_impl->m_fonts) {
        if (f == font) return true;
    }

    auto texture = CCTextureCache::get()->addImage(font->getAtlasName().c_str(), false);
    if (!texture) return false;

    m_impl->m_fonts.emplace_back(font);
    m_impl->m_batches.emplace_back(texture);

    if (m_impl->m_spaceWidth == 0.f) {
        auto const& chars = font->getCharDefs();
        auto it = chars.find(U' ');
        if (it != chars.end()) {
            m_impl->m_spaceWidth = it->second.xAdvanceScaled;
        }
    }

    m_impl->m_textDirty = true;

    return true;
}

bool Label::removeFont(size_t index) {
    if (index >= m_impl->m_fonts.size()) return false;

    m_impl->m_fonts.erase(m_impl->m_fonts.begin() + index);
    m_impl->m_batches.erase(m_impl->m_batches.begin() + index);

    if (index == 0) {
        m_impl->m_spaceWidth = 0.f;
        if (!m_impl->m_fonts.empty()) {
            auto const& chars = m_impl->m_fonts[0]->getCharDefs();
            auto it = chars.find(U' ');
            if (it != chars.end()) {
                m_impl->m_spaceWidth = it->second.xAdvanceScaled;
            }
        }
    }

    m_impl->m_textDirty = true;

    return true;
}

bool Label::removeFont(ZStringView font) {
    for (size_t i = 0; i < m_impl->m_fonts.size(); ++i) {
        if (m_impl->m_fonts[i]->getAtlasName() == font) {
            return this->removeFont(i);
        }
    }
    return false;
}

bool Label::setFont(BitmapFont* font, size_t index) {
    if (!font || index >= m_impl->m_fonts.size()) return false;
    if (m_impl->m_fonts[index] == font) return true;

    auto texture = CCTextureCache::get()->addImage(font->getAtlasName().c_str(), false);
    if (!texture) return false;

    m_impl->m_fonts[index] = font;
    m_impl->m_batches[index] = LabelFontBatch(texture);

    if (index == 0) {
        m_impl->m_spaceWidth = 0.f;
        auto const& chars = font->getCharDefs();
        auto it = chars.find(U' ');
        if (it != chars.end()) {
            m_impl->m_spaceWidth = it->second.xAdvanceScaled;
        }
    }

    m_impl->m_textDirty = true;

    return true;
}

bool Label::setFont(ZStringView font, size_t index) {
    return this->setFont(BitmapFont::load(font), index);
}

BitmapFont* Label::getFont(size_t index) noexcept {
    if (index >= m_impl->m_fonts.size()) return nullptr;
    return m_impl->m_fonts[index];
}

bool Label::registerFont(ZStringView font) {
    return this->registerFont(BitmapFont::load(font));
}

void Label::setAlignment(Alignment alignment) noexcept {
    if (m_impl->m_alignment == alignment) {
        return;
    }

    m_impl->m_alignment = alignment;
    m_impl->m_layoutDirty = true;
}

Label::Alignment Label::getAlignment() noexcept {
    return m_impl->m_alignment;
}

void Label::setMaxWidth(float maxWidth) noexcept {
    if (m_impl->m_maxLineWidth == maxWidth) {
        return;
    }

    m_impl->m_maxLineWidth = maxWidth;
    m_impl->m_lineBreak = maxWidth > 0.f;
    m_impl->m_textDirty = true;
}

float Label::getMaxWidth() noexcept {
    return m_impl->m_maxLineWidth;
}

void Label::setBreakWords(bool breakWords) noexcept {
    if (m_impl->m_wordBreak == breakWords) {
        return;
    }

    m_impl->m_wordBreak = breakWords;
    m_impl->m_layoutDirty = true;
}

bool Label::isBreakWords() noexcept {
    return m_impl->m_wordBreak;
}

void Label::setLimitLabelWidth(float width, float maxScale, float minScale) noexcept {
    m_impl->m_limitSize.width = width;
    m_impl->m_defaultScale = maxScale;
    m_impl->m_minScale = minScale;
    m_impl->m_limitChanged = true;
    m_impl->m_fitLabelSize = false;
}

void Label::setLimitLabelHeight(float height, float maxScale, float minScale) noexcept {
    m_impl->m_limitSize.height = height;
    m_impl->m_defaultScale = maxScale;
    m_impl->m_minScale = minScale;
    m_impl->m_limitChanged = true;
    m_impl->m_fitLabelSize = false;
}

void Label::setLimitLabelSize(CCSize size, float maxScale, float minScale) noexcept {
    m_impl->m_limitSize = size;
    m_impl->m_defaultScale = maxScale;
    m_impl->m_minScale = minScale;
    m_impl->m_limitChanged = true;
    m_impl->m_fitLabelSize = false;
}

void Label::removeLabelSizeLimit() noexcept {
    m_impl->m_limitSize = CCSize{0.f, 0.f};
    m_impl->m_limitChanged = true;
}

void Label::setFitBox(CCSize size, float maxScale) noexcept {
    m_impl->m_limitSize = size;
    m_impl->m_defaultScale = maxScale;
    m_impl->m_fitLabelSize = true;
    m_impl->m_limitChanged = true;
}

void Label::disableFitBox() noexcept {
    m_impl->m_fitLabelSize = false;
    m_impl->m_limitChanged = true;
}

void Label::setExtraKerning(float kerning) noexcept {
    m_impl->m_extraKerning = kerning;
    m_impl->m_textDirty = true;
}

float Label::getExtraKerning() noexcept {
    return m_impl->m_extraKerning;
}

void Label::setLineSpacing(float lineSpacing) noexcept {
    m_impl->m_extraLineSpacing = lineSpacing;
    m_impl->m_textDirty = true;
}

float Label::getLineSpacing() noexcept {
    return m_impl->m_extraLineSpacing;
}

void Label::setCharColor(uint32_t index, ccColor4B color) {
    auto& chars = m_impl->m_chars;
    if (index >= chars.size()) return;

    auto ref = chars[index];

    auto& quad = m_impl->m_batches[ref.batchIndex].quads[ref.charIndex];
    quad.bl.colors = color;
    quad.br.colors = color;
    quad.tl.colors = color;
    quad.tr.colors = color;

    m_impl->m_batches[ref.batchIndex].isDirty = true;
}

size_t Label::getCharCount() noexcept {
    this->validate();
    return m_impl->m_chars.size();
}

size_t Label::getGlyphCount() noexcept {
    this->validate();
    return m_impl->m_glyphs.size();
}

std::vector<Label::GlyphInfo> const& Label::getGlyphInfos() noexcept {
    this->validate();
    return m_impl->m_glyphs;
}

std::optional<Label::GlyphInfo> Label::getGlyphInfo(size_t glyphIndex) noexcept {
    this->validate();
    if (glyphIndex >= m_impl->m_glyphs.size()) {
        return std::nullopt;
    }
    return m_impl->m_glyphs[glyphIndex];
}

size_t Label::getCodepointCount() noexcept {
    this->validate();
    return m_impl->m_codepointCount;
}

float Label::getLineHeight() noexcept {
    if (m_impl->m_fonts.empty()) return 0.f;
    return m_impl->m_fonts.data()[0]->getCommonHeightScaled() + m_impl->m_extraLineSpacing;
}

std::optional<CCRect> Label::getCharBounds(size_t codepointIndex) noexcept {
    this->validate();

    if (codepointIndex > m_impl->m_codepointCount) {
        return std::nullopt;
    }

    float lineHeight = this->getLineHeight();
    auto const& shaped = m_impl->m_shaped;

    for (size_t i = 0; i < shaped.size(); ++i) {
        auto const& item = shaped[i];
        auto const& pen = m_impl->m_itemPens[i];

        if (item.srcIndex + item.srcLength <= codepointIndex) continue;

        if (item.srcIndex > codepointIndex) {
            return CCRect{ pen.x, pen.y - lineHeight, 0.f, lineHeight };
        }

        switch (item.kind) {
            case ShapedItem::Kind::Glyph:
            case ShapedItem::Kind::Emoji:
            case ShapedItem::Kind::Node: {
                auto& glyphs = m_impl->m_glyphs;
                auto it = std::ranges::lower_bound(
                    glyphs, item.srcIndex, {}, &GlyphInfo::stringIndex
                );
                if (it != glyphs.end() && it->stringIndex == item.srcIndex) {
                    return it->bounds;
                }
                return CCRect{ pen.x, pen.y - lineHeight, 0.f, lineHeight };
            }
            case ShapedItem::Kind::Space:
                return CCRect{ pen.x, pen.y - lineHeight, item.advance, lineHeight };
            case ShapedItem::Kind::Newline:
                return CCRect{ pen.x, pen.y - lineHeight, 0.f, lineHeight };
        }
    }

    auto pen = m_impl->endPen();
    return CCRect{ pen.x, pen.y - lineHeight, 0.f, lineHeight };
}

std::optional<CCPoint> Label::getCursorPosition(size_t codepointIndex) noexcept {
    this->validate();

    if (codepointIndex > m_impl->m_codepointCount) {
        return std::nullopt;
    }

    float lineHeight = this->getLineHeight();
    auto const& shaped = m_impl->m_shaped;
    for (size_t i = 0; i < shaped.size(); ++i) {
        if (shaped[i].srcIndex >= codepointIndex) {
            return CCPoint{ m_impl->m_itemPens[i].x, m_impl->m_itemPens[i].y - lineHeight };
        }
    }

    auto pen = m_impl->endPen();
    return CCPoint{ pen.x, pen.y - lineHeight };
}

void Label::setEmojiRegistry(EmojiRegistry const& registry) {
    m_impl->m_emojiRegistry = registry.m_impl;
    m_impl->m_textDirty = true;
}

EmojiRegistry Label::getEmojiRegistry() {
    if (!m_impl->m_emojiRegistry) {
        return EmojiRegistry();
    }
    return EmojiRegistry(m_impl->m_emojiRegistry);
}

void Label::setColor(ccColor3B const& color) {
    if (m_impl->m_color == color) {
        return;
    }

    m_impl->m_color = color;
    m_impl->m_displayedColor = color;

    if (m_impl->m_cascadeColorEnabled) {
        ccColor3B parentColor = {255, 255, 255};
        auto parentRGBA = typeinfo_cast<CCRGBAProtocol*>(m_pParent);
        if (parentRGBA && parentRGBA->isCascadeColorEnabled()) {
            parentColor = parentRGBA->getDisplayedColor();
        }
        this->updateDisplayedColor(parentColor);
    } else {
        m_impl->m_quadsDirty = true;
    }
}

ccColor3B const& Label::getColor() {
    return m_impl->m_color;
}

ccColor3B const& Label::getDisplayedColor() {
    return m_impl->m_displayedColor;
}

void Label::setOpacity(GLubyte opacity) {
    if (m_impl->m_opacity == opacity) {
        return;
    }

    m_impl->m_opacity = opacity;
    m_impl->m_displayedOpacity = opacity;

    if (m_impl->m_cascadeOpacityEnabled) {
        GLubyte parentOpacity = 255;
        auto parentRGBA = typeinfo_cast<CCRGBAProtocol*>(m_pParent);
        if (parentRGBA && parentRGBA->isCascadeOpacityEnabled()) {
            parentOpacity = parentRGBA->getDisplayedOpacity();
        }
        this->updateDisplayedOpacity(parentOpacity);
    } else {
        m_impl->m_quadsDirty = true;
    }
}

GLubyte Label::getOpacity() {
    return m_impl->m_opacity;
}

GLubyte Label::getDisplayedOpacity() {
    return m_impl->m_displayedOpacity;
}

void Label::setUseColoredEmojis(bool value) noexcept {
    if (m_impl->m_emojisUseColors == value) {
        return;
    }

    m_impl->m_emojisUseColors = value;
    m_impl->m_quadsDirty = true;
}

bool Label::getUseColoredEmojis() noexcept {
    return m_impl->m_emojisUseColors;
}

void Label::setUseColoredNodes(bool value) noexcept {
    if (m_impl->m_customNodesColors == value) {
        return;
    }

    m_impl->m_customNodesColors = value;
    m_impl->m_quadsDirty = true;
}

bool Label::getUseColoredNodes() noexcept {
    return m_impl->m_customNodesColors;
}

std::vector<Ref<CCNode>> const& Label::getEmbeddedNodes() const noexcept {
    return m_impl->m_embeddedNodes;
}

void Label::setOpacityModifyRGB(bool bValue) {
    if (m_impl->m_isOpacityModifyRGB == bValue) {
        return;
    }

    m_impl->m_isOpacityModifyRGB = bValue;
    m_impl->m_quadsDirty = true;
}

bool Label::isOpacityModifyRGB() {
    return m_impl->m_isOpacityModifyRGB;
}

void Label::setCascadeColorEnabled(bool cascadeColorEnabled) {
    m_impl->m_cascadeColorEnabled = cascadeColorEnabled;
}

bool Label::isCascadeColorEnabled() {
    return m_impl->m_cascadeColorEnabled;
}

void Label::updateDisplayedColor(ccColor3B const& color) {
    m_impl->m_displayedColor.r = static_cast<GLubyte>(m_impl->m_color.r * color.r / 255.0f);
    m_impl->m_displayedColor.g = static_cast<GLubyte>(m_impl->m_color.g * color.g / 255.0f);
    m_impl->m_displayedColor.b = static_cast<GLubyte>(m_impl->m_color.b * color.b / 255.0f);

    m_impl->m_quadsDirty = true;

    if (m_impl->m_cascadeColorEnabled) {
        for (auto child : this->getChildrenExt()) {
            if (auto* rgba = typeinfo_cast<CCRGBAProtocol*>(child)) {
                rgba->updateDisplayedColor(m_impl->m_displayedColor);
            }
        }
    }
}

void Label::updateDisplayedOpacity(GLubyte opacity) {
    m_impl->m_displayedOpacity = static_cast<GLubyte>(m_impl->m_opacity * opacity / 255.0f);

    m_impl->m_quadsDirty = true;

    if (m_impl->m_cascadeOpacityEnabled) {
        for (auto child : this->getChildrenExt()) {
            if (auto* rgba = typeinfo_cast<CCRGBAProtocol*>(child)) {
                rgba->updateDisplayedOpacity(m_impl->m_displayedOpacity);
            }
        }
    }
}

void Label::setCascadeOpacityEnabled(bool cascadeOpacityEnabled) {
    m_impl->m_cascadeOpacityEnabled = cascadeOpacityEnabled;
}

bool Label::isCascadeOpacityEnabled() {
    return m_impl->m_cascadeOpacityEnabled;
}

void Label::setText(std::string text) noexcept {
    if (m_impl->m_text == text) {
        return;
    }

    m_impl->m_text = std::move(text);
    m_impl->m_textDirty = true;
}

ZStringView Label::getText() noexcept {
    return m_impl->m_text;
}

void Label::setRichText(std::string text) {
    bool wasRich = m_impl->m_useRichText;
    m_impl->m_useRichText = true;

    if (m_impl->m_text == text) {
        if (!wasRich) {
            m_impl->m_textDirty = true;
        }
        return;
    }

    m_impl->m_text = std::move(text);
    m_impl->m_textDirty = true;
}

void Label::setRichTextEnabled(bool enabled) noexcept {
    if (m_impl->m_useRichText == enabled) return;
    m_impl->m_useRichText = enabled;
    m_impl->m_textDirty = true;
}

bool Label::isRichTextEnabled() const noexcept {
    return m_impl->m_useRichText;
}

void Label::setString(char const* label) {
    if (m_impl->m_text == label) {
        return;
    }

    m_impl->m_text = label;
    m_impl->m_textDirty = true;
}

char const* Label::getString() {
    return m_impl->m_text.c_str();
}

void Label::setBlendFunc(ccBlendFunc blendFunc) {
    m_impl->m_blendFunc = blendFunc;
}

ccBlendFunc Label::getBlendFunc() {
    return m_impl->m_blendFunc;
}

CCSize const& Label::getContentSize() const {
    const_cast<Label*>(this)->validate();
    return CCNode::getContentSize();
}

CCSize Label::getScaledContentSize() {
    this->validate();
    return CCNode::getScaledContentSize();
}

float Label::getScale() {
    this->validate();
    return CCNode::getScale();
}

float Label::getScaleX() {
    this->validate();
    return CCNode::getScaleX();
}

float Label::getScaleY() {
    this->validate();
    return CCNode::getScaleY();
}

bool Label::initWithFont(BitmapFont* font) {
    if (!CCNode::init()) {
        return false;
    }

    if (!this->registerFont(font)) {
        return false;
    }

    bool hasPMA = m_impl->m_batches[0].texture->hasPremultipliedAlpha();
    m_impl->m_isOpacityModifyRGB = hasPMA;

    if (!hasPMA) {
        this->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });
    }

    this->setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    this->setAnchorPoint({0.5f, 0.5f});

    // provide vanilla emojis by default
    this->setEmojiRegistry(EmojiRegistry::vanillaEmojis());

    return true;
}

bool Label::initWithFontFile(ZStringView fntFile) {
    return this->initWithFont(BitmapFont::load(fntFile));
}

bool Label::initWithString(std::string text, BitmapFont* font) {
    if (!this->initWithFont(font)) {
        return false;
    }

    m_impl->m_text = std::move(text);

    return true;
}

bool Label::initWithString(std::string text, ZStringView fntFile) {
    return this->initWithString(std::move(text), BitmapFont::load(fntFile));
}

bool Label::initWithRichText(std::string text, BitmapFont* font) {
    if (!this->initWithString(std::move(text), font)) {
        return false;
    }

    m_impl->m_useRichText = true;

    return true;
}

bool Label::initWithRichText(std::string text, ZStringView fntFile) {
    return this->initWithRichText(std::move(text), BitmapFont::load(fntFile));
}
