#include <Geode/ui/Label.hpp>

#include <Geode/loader/GameEvent.hpp>
#include <Geode/utils/StringMap.hpp>
#include <Geode/utils/file.hpp>
#include <asp/collections/SmallVec.hpp>
#include <simdutf/implementation.h>

#include <numeric>
#include <ranges>

using namespace geode::prelude;

static StringMap<BitmapFont>& GetBitmapFontsCache() {
    static StringMap<BitmapFont> cache;
    return cache;
}

BitmapFont::BitmapFont() = default;
BitmapFont::~BitmapFont() = default;

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
    for (auto p : asp::iter::split(line, ' ')) {
        for (auto pair : asp::iter::split(p, '\t')) {
            auto arr = asp::iter::split(pair, '=').collect<asp::SmallVec<std::string_view, 2>>();
            auto size = arr.size();
            if (size == 2) fn(arr[0], arr[1]);
            else if (size == 1) fn(arr[0], std::string_view(""));
        }
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
        m_textureReloadHandle = GameEvent(GameEventType::TexturesUnloaded).listen([this]() {
            for (auto& entry : m_entries) {
                if (auto* emoji = std::get_if<EmojiEntry>(&entry)) {
                    emoji->invalidate();
                }
            }
        });
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

    void insert(std::span<std::tuple<std::u32string_view, Entry>> entries) {
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
        CCSpriteFrame* frame; // emoji
    };
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
    asp::SmallVec<BitmapFont const*, 2> m_fonts;
    asp::SmallVec<LabelFontBatch, 2> m_batches;
    std::vector<CharQuadRef> m_chars;

    Label* m_label;
    std::shared_ptr<EmojiRegistry::Impl> m_emojiRegistry;
    asp::SmallVec<LabelFontBatch, 1> m_emojiBatches;
    std::vector<Ref<CCNode>> m_embeddedNodes;

    std::vector<ShapedItem> m_shaped;
    asp::SmallVec<WordSpan, 3> m_words;
    asp::SmallVec<LineSpan, 1> m_lines;

    std::vector<StyleSpan> m_styles;

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

    CCSize m_limitSize{};
    float m_defaultScale = 1.f;
    float m_minScale = 0.f;
    bool m_limitChanged = false;
    bool m_fitLabelSize = false;

    bool m_emojisUseColors = false;
    bool m_customNodesColors = false;

    bool m_useRichText = false;

    bool m_textDirty = true;
    bool m_layoutDirty = true;
    bool m_quadsDirty = true;

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
                static_cast<GLubyte>(color.r * m_opacity / 255),
                static_cast<GLubyte>(color.g * m_opacity / 255),
                static_cast<GLubyte>(color.b * m_opacity / 255),
                static_cast<GLubyte>(m_opacity)
            };
        }

        return { color.r, color.g, color.b, m_opacity };
    }

    ccColor4B getEffectiveColor() const {
        return this->getEffectiveColor(m_color);
    }

    ccColor4B getEmojiColor() const {
        if (m_emojisUseColors) {
            return { m_color.r, m_color.g, m_color.b, m_opacity };
        }
        return { 255, 255, 255, m_opacity };
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
        if (!m_emojiRegistry) return false;

        auto entry = m_emojiRegistry->match(str, index);
        if (!entry) return false;

        float baseHeight = m_fonts.data()[0]->getCommonHeightScaled();

        std::visit([&]<typename T>(T& e) {
            if constexpr (std::is_same_v<T, EmojiRegistry::EmojiEntry>) {
                auto frame = e.getFrame();
                if (!frame) return;

                auto size = frame->getRect().size;
                auto batchIndex = this->getOrCreateBatch(frame->getTexture());

                m_shaped.push_back(ShapedItem{
                    .kind = ShapedItem::Kind::Emoji,
                    .fontIndex = batchIndex,
                    .advance = size.width * (baseHeight / size.height),
                    .frame = frame,
                });
            } else if constexpr (std::is_same_v<T, EmojiRegistry::NodeFactory>) {
                auto node = e();
                if (!node) return;

                auto size = node->getContentSize();

                m_shaped.push_back(ShapedItem{
                    .kind = ShapedItem::Kind::Node,
                    .fontIndex = 0,
                    .advance = size.width * (baseHeight / size.height),
                    .node = node,
                });

                m_label->addChild(node);
                m_embeddedNodes.push_back(node);
            } else if constexpr (std::is_same_v<T, EmojiRegistry::NodeFactoryParams>) {
                auto node = e(str, index);
                if (!node) return;

                auto size = node->getContentSize();

                m_shaped.push_back(ShapedItem{
                    .kind = ShapedItem::Kind::Node,
                    .fontIndex = 0,
                    .advance = size.width * (baseHeight / size.height),
                    .node = node,
                });

                m_label->addChild(node);
                m_embeddedNodes.push_back(node);
            }
        }, *entry);

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
            .def = def
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
                        .def = nullptr
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
                        .def = nullptr
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

                    this->appendGlyphItem(ch, def, fontIndex, prevCp, prevFontIndex);
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
                        .def = nullptr
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
                        .def = nullptr
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

                    this->appendGlyphItem(ch, def, fontIndex, prevCp, prevFontIndex);
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
                            lineMaxX = std::max(lineMaxX, x + item.advance);
                            break;
                        }
                        case ShapedItem::Kind::Emoji: {
                            auto frame = item.frame;
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

                            float scale = rectPoints.size.height > 0.f ? commonHeight / rectPoints.size.height : 1.f;
                            float w = rectPoints.size.width * scale;
                            float h = rectPoints.size.height * scale;
                            float qx = x;
                            float qy = y;

                            auto emojiColor = this->getEmojiColor();

                            batch.quads.push_back({
                                .bl = { .vertices = { qx, qy - h }, .colors = emojiColor, .texCoords = { blU, blV } },
                                .br = { .vertices = { qx + w, qy - h }, .colors = emojiColor, .texCoords = { brU, brV } },
                                .tl = { .vertices = { qx, qy }, .colors = emojiColor, .texCoords = { tlU, tlV } },
                                .tr = { .vertices = { qx + w, qy }, .colors = emojiColor, .texCoords = { trU, trV } },
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

        m_label->setContentSize({ maxLineWidth, lineHeight * m_lines.size() });
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

        auto emojiColor = this->getEmojiColor();

        for (auto& batch : m_emojiBatches) {
            for (auto& quad : batch.quads) {
                quad.bl.colors = emojiColor;
                quad.br.colors = emojiColor;
                quad.tl.colors = emojiColor;
                quad.tr.colors = emojiColor;
            }
            batch.isDirty = true;
        }

        for (auto& node : m_embeddedNodes) {
            if (auto* rgba = typeinfo_cast<CCRGBAProtocol*>(node.data())) {
                if (m_customNodesColors) rgba->setColor(m_color);
                rgba->setOpacity(m_opacity);
            }
        }
    }

    void applyLimitScale() {
        if (m_fitLabelSize || m_limitSize.width <= 0.f && m_limitSize.height <= 0.f) {
            return;
        }

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

        float lo = 1.f;
        float hi = std::max(naturalWidth, lo);

        float bestWrap = hi;
        float bestScale = -1.f;
        float lineHeight = m_fonts.empty() ? 0.f : m_fonts.front()->getCommonHeightScaled() + m_extraLineSpacing;

        m_lineBreak = true;

        constexpr auto ITERATIONS = 14;

        for (int i = 0; i < ITERATIONS; ++i) {
            float mid = (lo + hi) * 0.5f;
            m_maxLineWidth = mid;
            this->breakLines();

            float w = 0.f;
            for (auto const& line : m_lines) w = std::max(w, line.width);
            float h = lineHeight * m_lines.size();

            float rawWidthRatio = (m_limitSize.width > 0.f && w > 0.f) ? m_limitSize.width / w : std::numeric_limits<float>::max();
            float rawHeightRatio = (m_limitSize.height > 0.f && h > 0.f) ? m_limitSize.height / h : std::numeric_limits<float>::max();

            float widthRatio = std::min(rawWidthRatio, m_defaultScale);
            float heightRatio = std::min(rawHeightRatio, m_defaultScale);

            float scale = std::min(widthRatio, heightRatio);

            if (scale > bestScale) {
                bestScale = scale;
                bestWrap = mid;
            }

            if (widthRatio > heightRatio) {
                lo = mid;
            } else {
                hi = mid;
            }
        }

        m_maxLineWidth = bestWrap;
        this->breakLines();

        m_label->setScale(bestScale);
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

void EmojiRegistry::insert(std::span<std::tuple<std::u32string_view, Entry>> entries) {
    m_impl->insert(entries);
}

void EmojiRegistry::erase(std::u32string_view sequence) {
    m_impl->erase(sequence);
}

EmojiRegistry::Entry* EmojiRegistry::match(std::u32string_view str, size_t& index) {
    return m_impl->match(str, index);
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

void Label::draw() {
    if (!m_pShaderProgram) return;

    this->validate();
    if (m_impl->m_chars.empty()) return;

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
    m_impl->m_layoutDirty = true;
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

void Label::setEmojiRegistry(EmojiRegistry const& registry) {
    m_impl->m_emojiRegistry = registry.m_impl;
}

EmojiRegistry Label::getEmojiRegistry() {
    return EmojiRegistry(m_impl->m_emojiRegistry);
}

void Label::setColor(ccColor3B const& color) {
    if (m_impl->m_color == color) {
        return;
    }

    m_impl->m_color = color;
    m_impl->m_quadsDirty = true;
}

ccColor3B const& Label::getColor() {
    return m_impl->m_color;
}

ccColor3B const& Label::getDisplayedColor() {
    return m_impl->m_color;
}

void Label::setOpacity(GLubyte opacity) {
    if (m_impl->m_opacity == opacity) {
        return;
    }

    m_impl->m_opacity = opacity;
    m_impl->m_quadsDirty = true;
}

GLubyte Label::getOpacity() {
    return m_impl->m_opacity;
}

GLubyte Label::getDisplayedOpacity() {
    return m_impl->m_opacity;
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

void Label::setCascadeColorEnabled(bool cascadeColorEnabled) {}
bool Label::isCascadeColorEnabled() { return false; }
void Label::updateDisplayedColor(ccColor3B const& color) {}
void Label::updateDisplayedOpacity(GLubyte opacity) {}
void Label::setCascadeOpacityEnabled(bool cascadeOpacityEnabled) {}
bool Label::isCascadeOpacityEnabled() { return false; }

void Label::setText(std::string text) noexcept {
    if (m_impl->m_text == text) {
        return;
    }

    m_impl->m_text = std::move(text);
    m_impl->m_textDirty = true;
    m_impl->m_useRichText = false;
}

ZStringView Label::getText() noexcept {
    return m_impl->m_text;
}

void Label::setRichText(std::string text) {
    if (m_impl->m_text == text) {
        return;
    }

    m_impl->m_text = std::move(text);
    m_impl->m_textDirty = true;
    m_impl->m_useRichText = true;
}

void Label::setString(char const* label) {
    if (m_impl->m_text == label) {
        return;
    }

    m_impl->m_text = label;
    m_impl->m_textDirty = true;
    m_impl->m_useRichText = false;
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