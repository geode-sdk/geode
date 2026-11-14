#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/binding/TextInputDelegate.hpp>
#include <Geode/cocos/draw_nodes/CCDrawNode.h>
#include <Geode/cocos/misc_nodes/CCClippingNode.h>
#include <Geode/cocos/platform/CCEGLView.h>
#include <Geode/cocos/robtop/mouse_dispatcher/CCMouseDispatcher.h>
#include <Geode/cocos/touch_dispatcher/CCTouchHandler.h>
#include <Geode/loader/Log.hpp>
#include <Geode/modify/CCTextInputNode.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/Keyboard.hpp>
#include <simdutf/implementation.h>

using namespace geode::prelude;

struct TextInputNodeFix : Modify<TextInputNodeFix, CCTextInputNode> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("TextInputNode fix")

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) {
        if (!this->getUserObject("fix-text-input")) {
            return CCTextInputNode::ccTouchBegan(touch, event);
        }

        if (!nodeIsVisible(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        auto touchPos = touch->getLocation();
        auto const size = this->getContentSize();
        auto const pos = this->convertToNodeSpace(touchPos) + m_textField->getAnchorPoint() * size;

        float parentScale = 1.f;
        CCNode* currentParent = this;

        while ((currentParent = currentParent->getParent())) {
            parentScale *= currentParent->getScale();
        }

        CCPoint nodeSpace = this->convertToNodeSpace(touchPos);
        nodeSpace = nodeSpace / parentScale;
        touchPos = this->convertToWorldSpace(nodeSpace);

        if (pos.x < 0 || pos.x > size.width || pos.y < 0 || pos.y > size.height) {
            this->onClickTrackNode(false);
            return false;
        }
        if (m_delegate && !m_delegate->allowTextInput(this)) {
            this->onClickTrackNode(false);
            return false;
        }

        this->onClickTrackNode(true);
        this->updateCursorPosition(touchPos, {{0, 0}, size});

        return true;
    }
};

static constexpr bool isWordSeparator(char32_t c) {
    switch (c) {
        case U' ': case U'\t':
        case U'\n': case U'\r':
        case 0x00A0: case 0x3000: return true;
        default: return c >= 0x2000 && c <= 0x200A;
    }
}

static constexpr bool isCjk(char32_t c) {
    return (c >= 0x2E80 && c <= 0x9FFF) || (c >= 0xAC00 && c <= 0xD7AF) ||
        (c >= 0xF900 && c <= 0xFAFF) || (c >= 0xFF00 && c <= 0xFFEF) ||
        (c >= 0x20000 && c <= 0x2FFFF);
}

struct TextInput::Impl {
    std::string m_text;
    std::u32string m_textU32;

    int m_cursorPos = 0;
    int m_selectionStart = -1;

    Label::Alignment m_alignment = Label::Alignment::Center;
    bool m_multiline = false;
    bool m_password = false;
    bool m_numeric = false;
    bool m_hasFocus = false;
    bool m_isDragging = false;
    bool m_enabled = true;
    bool m_wrap = true;

    size_t m_maxLength = std::numeric_limits<size_t>::max();

    std::string m_filterText;
    std::u32string m_filterChars;

    CCPoint m_padding{4.f, 4.f};
    CCPoint m_scroll{0.f, 0.f};
    ccColor4B m_selectionColor{100, 150, 255, 128};
    float m_goalX = -1.f;

    TextInput* m_parent = nullptr;
    Label* m_label = nullptr;
    Label* m_placeholderLabel = nullptr;
    Label* m_captionLabel = nullptr;
    CCLayerColor* m_caret = nullptr;
    NineSlice* m_background = nullptr;
    CCDrawNode* m_selectionDraw = nullptr;
    CCClippingNode* m_clip = nullptr;

    float m_fontScale = 0.6f;
    float m_placeholderScale = 0.5f;

    std::vector<size_t> m_cursorStops;
    std::vector<CCPoint> m_stopPositions;

    ListenerHandle m_keyListener;

    Function<void(std::string const&)> m_onTextChanged;
    Function<void(std::string const&)> m_onEnter;
    Function<void(FocusState)> m_onFocusChanged;
    ListenerHandle m_textChangedListener;
    ListenerHandle m_focusedListener;
    ListenerHandle m_unfocusedListener;

    Function<void(float)> m_onKeyboardOffset;
    Function<void()> m_onKeyboardHide;
    bool m_keyboardShowing = false;

    bool m_refocusOnEnter = false;

    OverflowMode m_overflow = OverflowMode::Scale;

    enum class ClickMode : uint8_t {
        None,
        Word,
        Line
    };

    ClickMode m_clickMode = ClickMode::None;
    int m_clickCount = 0;
    asp::time::SystemTime m_lastClickTime;
    CCPoint m_lastClickPos{0.f, 0.f};
    std::pair<int, int> m_clickAnchor{0, 0};

    Impl(TextInput* parent) : m_parent(parent) {}

    void syncLabel(bool fireEvent = true) {
        m_text.resize(simdutf::utf8_length_from_utf32(m_textU32));
        (void)simdutf::convert_utf32_to_utf8(m_textU32.data(), m_textU32.size(), m_text.data());

        m_label->setText(m_text);
        this->rebuildCursorStops();
        m_cursorPos = std::clamp<int>(m_cursorPos, 0, m_cursorStops.back());

        if (m_password) {
            StringBuffer masked;
            size_t graphemes = m_cursorStops.size() - 1;
            for (size_t i = 0; i < graphemes; ++i) {
                masked.append("\u2022");
            }
            m_label->setText(masked.str());
        }

        m_stopPositions.resize(m_cursorStops.size());
        for (size_t i = 0; i < m_cursorStops.size(); ++i) {
            size_t queryIdx = m_password ? i : m_cursorStops[i];
            m_stopPositions[i] = m_label->getCursorPosition(queryIdx).value_or(CCPoint{});
        }

        m_placeholderLabel->setVisible(m_text.empty());

        if (m_textU32.empty()) m_scroll = CCPoint{0.f, 0.f};

        this->updateCaretPosition();
        this->updateSelectionHighlight();
        this->clampScroll();

        m_goalX = -1.f;

        if (fireEvent) {
            TextChangedEvent(m_parent).send(m_text);
        }
    }

    std::optional<CCPoint> stopPos(int cp) const {
        auto it = std::ranges::lower_bound(m_cursorStops, std::max(0, cp));
        if (it == m_cursorStops.end() || *it != cp) return std::nullopt;
        return m_stopPositions[it - m_cursorStops.begin()];
    }

    void updateCaretPosition() {
        auto pos = this->stopPos(m_cursorPos).value_or(CCPoint{});
        auto height = m_label->getLineHeight();
        if (!m_text.empty()) {
            pos.y += height * 0.5f;
        } else if (m_multiline) {
            pos.y -= height * 0.5f;
        }

        m_caret->setPosition(pos);

        if (m_hasFocus) {
            this->scrollCaretIntoView();
        }
    }

    float maxScrollX() const {
        if (m_overflow != OverflowMode::Clip) return 0.f;
        if (m_multiline && m_wrap) return 0.f;
        auto avail = m_parent->getContentWidth() - m_padding.x * 2.f;
        return std::max(0.f, m_label->getScaledContentWidth() - avail);
    }

    float maxScrollY() const {
        if (!m_multiline || m_overflow != OverflowMode::Clip) return 0.f;
        auto avail = m_parent->getContentHeight() - m_padding.y * 2.f;
        return std::max(0.f, m_label->getScaledContentHeight() - avail);
    }

    void clampScroll() {
        if (!m_label) return;
        m_scroll.x = std::clamp(m_scroll.x, 0.f, this->maxScrollX());
        m_scroll.y = std::clamp(m_scroll.y, 0.f, this->maxScrollY());
        this->updateLabelPositions();
    }

    void updateLabelPositions() {
        auto size = m_parent->getContentSize();

        auto alignment = m_alignment;
        if (this->maxScrollX() > 0.5f) {
            alignment = Label::Alignment::Left;
        }

        float ax, px;
        switch (alignment) {
            case Label::Alignment::Center:
                ax = 0.5f;
                px = size.width * 0.5f;
                break;
            case Label::Alignment::Right:
                ax = 1.f;
                px = size.width - m_padding.x;
                break;
            default:
                ax = 0.f;
                px = m_padding.x - m_scroll.x;
                break;
        }

        if (m_multiline) {
            float py = size.height - m_padding.y;
            m_label->setAnchorPoint({ax, 1.f});
            m_label->setPosition({px, py + m_scroll.y});
            m_placeholderLabel->setAnchorPoint({ax, 1.f});
            m_placeholderLabel->setPosition({px, py});
        } else {
            m_label->setAnchorPoint({ax, 0.5f});
            m_label->setPosition({px, size.height * 0.5f});
            m_placeholderLabel->setAnchorPoint({ax, 0.5f});
            m_placeholderLabel->setPosition({px, size.height * 0.5f});
        }
    }

    void scrollCaretIntoView() {
        auto world = m_label->convertToWorldSpace(m_caret->getPosition());
        auto clip = m_clip->convertToNodeSpace(world);
        auto size = m_parent->getContentSize();

        if (this->maxScrollX() > 0.f) {
            float right = size.width - m_padding.x;
            float left = m_padding.x;
            if (clip.x > right) {
                m_scroll.x += clip.x - right;
            } else if (clip.x < left) {
                m_scroll.x -= left - clip.x;
            }
        }

        if (m_multiline) {
            float half = m_label->getLineHeight() * 0.5f;
            float top = size.height - m_padding.y;
            float bottom = m_padding.y;
            if (clip.y + half > top) {
                m_scroll.y -= clip.y + half - top;
            } else if (clip.y - half < bottom) {
                m_scroll.y += bottom - clip.y + half;
            }
        }

        this->clampScroll();
    }

    void rebuildCursorStops() {
        m_cursorStops.clear();

        size_t cpCount = m_textU32.size();

        std::vector<bool> insideGlyph(cpCount + 1, false);
        for (auto& g : m_label->getGlyphInfos()) {
            size_t start = g.stringIndex;
            size_t end = g.stringIndex + std::max<size_t>(g.stringLength, 1);
            for (size_t j = start + 1; j < end && j <= cpCount; ++j) {
                insideGlyph[j] = true;
            }
        }

        std::vector<size_t> stops;
        stops.reserve(cpCount + 1);
        for (size_t i = 0; i <= cpCount; ++i) {
            if (!insideGlyph[i]) stops.push_back(i);
        }

        m_cursorStops = std::move(stops);
    }

    struct VisualLine {
        float y;
        std::vector<int> stops;
    };

    std::vector<VisualLine> visualLines() const {
        std::vector<VisualLine> lines;
        if (m_cursorStops.size() <= 1) return lines;

        float lineH = m_label->getLineHeight();

        struct Entry {
            float x;
            float y;
            int stop;
        };

        std::vector<Entry> entries;
        entries.reserve(m_cursorStops.size());
        for (int stop : m_cursorStops) {
            if (auto pos = this->stopPos(stop)) {
                entries.push_back({pos->x, pos->y, stop});
            }
        }

        std::ranges::sort(entries, [](Entry const& a, Entry const& b) {
            if (a.y != b.y) return a.y < b.y;
            return a.x < b.x;
        });

        for (auto const& entry : entries) {
            if (!lines.empty() && std::abs(entry.y - lines.back().y) <= lineH * 0.5f) {
                lines.back().stops.push_back(entry.stop);
            } else {
                lines.push_back({entry.y, {entry.stop}});
            }
        }

        for (auto& line : lines) {
            std::ranges::sort(line.stops, [&](int a, int b) {
                auto pa = this->stopPos(a);
                auto pb = this->stopPos(b);
                return pa && pb && pa->x < pb->x;
            });
        }

        return lines;
    }

    int codepointAtLabelPos(CCPoint const& pos) const {
        if (m_cursorStops.size() <= 1) return 0;

        auto lines = this->visualLines();
        if (lines.empty()) return 0;

        float lineH = m_label->getLineHeight();

        VisualLine const* line = nullptr;
        float bestDist = std::numeric_limits<float>::max();
        for (auto const& candidate : lines) {
            float dist = 0.f;
            if (pos.y < candidate.y) {
                dist = candidate.y - pos.y;
            } else if (pos.y > candidate.y + lineH) {
                dist = pos.y - (candidate.y + lineH);
            }

            if (dist < bestDist) {
                bestDist = dist;
                line = &candidate;
            }
        }

        if (!line) return 0;

        // find the stop on that line closest to the point
        int best = line->stops.front();
        float bestDx = std::numeric_limits<float>::max();
        for (int stop : line->stops) {
            auto p = this->stopPos(stop);
            if (!p) continue;
            float dx = std::abs(p->x - pos.x);
            if (dx < bestDx) {
                bestDx = dx;
                best = stop;
            }
        }

        return best;
    }

    int snapUp(int cp) const {
        auto it = std::ranges::lower_bound(m_cursorStops, std::max(0, cp));
        return it == m_cursorStops.end() ? m_cursorStops.back() : *it;
    }

    int snapToStop(int cp) const {
        auto it = std::ranges::upper_bound(m_cursorStops, std::max(0, cp));
        return it == m_cursorStops.begin() ? m_cursorStops.front() : *(it - 1);
    }

    int snapToPrevStop(int cp) const {
        auto it = std::ranges::lower_bound(m_cursorStops, std::max(0, cp));
        return it == m_cursorStops.begin() ? *it : *(it - 1);
    }

    int snapToNextStop(int cp) const {
        auto it = std::ranges::upper_bound(m_cursorStops, std::max(0, cp));
        return it == m_cursorStops.end() ? m_cursorStops.back() : *it;
    }

    std::pair<int, int> wordRangeAt(int cp) const {
        int len = m_textU32.size();
        cp = std::clamp(cp, 0, len);

        int side = -1;
        if (cp < len && !isWordSeparator(m_textU32[cp])) {
            side = cp;
        } else if (cp > 0 && !isWordSeparator(m_textU32[cp - 1])) {
            side = cp - 1;
        }

        if (side >= 0) {
            if (isCjk(m_textU32[side])) {
                return {this->snapUp(side), this->snapUp(side + 1)};
            }
            int start = side, end = side + 1;
            while (start > 0 && !isWordSeparator(m_textU32[start - 1]) && !isCjk(m_textU32[start - 1])) --start;
            while (end < len && !isWordSeparator(m_textU32[end]) && !isCjk(m_textU32[end])) ++end;
            return {this->snapUp(start), this->snapUp(end)};
        }

        int start = cp, end = cp;
        while (start > 0 && isWordSeparator(m_textU32[start - 1])) --start;
        while (end < len && isWordSeparator(m_textU32[end])) ++end;
        if (start == end) return {cp, cp};
        return {this->snapUp(start), this->snapUp(end)};
    }

    std::pair<int, int> lineRangeAt(int cp) const {
        int len = m_textU32.size();
        if (m_cursorStops.size() <= 1 || len <= 0) return {0, len};

        for (auto const& line : this->visualLines()) {
            if (std::ranges::find(line.stops, cp) != line.stops.end()) {
                return {line.stops.front(), line.stops.back()};
            }
        }
        return {cp, cp};
    }

    int lineStartFor(int cp) const { return this->lineRangeAt(cp).first; }
    int lineEndFor(int cp) const { return this->lineRangeAt(cp).second; }

    void extendClickSelection(int target) {
        auto range = m_clickMode == ClickMode::Line
            ? this->lineRangeAt(target)
            : this->wordRangeAt(target);

        if (target >= m_clickAnchor.first) {
            m_selectionStart = m_clickAnchor.first;
            m_cursorPos = range.second;
        } else {
            m_selectionStart = range.first;
            m_cursorPos = m_clickAnchor.second;
        }

        if (m_selectionStart == m_cursorPos) {
            m_selectionStart = -1;
        }
    }

    int findPrevWordStop(int cp) const {
        int len = m_textU32.size();
        int i = std::min(cp, len);
        if (i <= 0) return 0;

        while (i > 0 && isWordSeparator(m_textU32[i - 1])) --i;
        if (i == 0) return 0;

        if (isCjk(m_textU32[i - 1])) return this->snapToStop(i - 1);

        while (i > 0 && !isWordSeparator(m_textU32[i - 1]) && !isCjk(m_textU32[i - 1])) --i;
        return this->snapToStop(i);
    }

    int findNextWordStop(int cp) const {
        int len = m_textU32.size();
        int i = std::max(0, cp);
        if (i >= len) return len;

        while (i < len && isWordSeparator(m_textU32[i])) ++i;
        if (i >= len) return len;

        if (isCjk(m_textU32[i])) return this->snapToStop(i + 1);

        while (i < len && !isWordSeparator(m_textU32[i]) && !isCjk(m_textU32[i])) ++i;
        return this->snapToStop(i);
    }

    bool hasSelection() const {
        return m_selectionStart >= 0 && m_selectionStart != m_cursorPos;
    }

    void clearSelection() {
        m_selectionStart = -1;
    }

    void deleteSelection() {
        if (!this->hasSelection()) return;
        int a = std::min(m_selectionStart, m_cursorPos);
        int b = std::max(m_selectionStart, m_cursorPos);
        m_textU32.erase(a, b - a);
        m_cursorPos = a;
        m_selectionStart = -1;
    }

    void updateSelectionHighlight() {
        m_selectionDraw->clear();
        if (!this->hasSelection()) return;

        int a = std::min(m_selectionStart, m_cursorPos);
        int b = std::max(m_selectionStart, m_cursorPos);
        if (a >= b) return;

        float lineHeight = m_label->getLineHeight();

        struct LineExtent {
            float minX = std::numeric_limits<float>::max();
            float maxX = std::numeric_limits<float>::lowest();
        };

        std::map<float, LineExtent> lines;

        for (
            auto it = std::ranges::lower_bound(m_cursorStops, static_cast<size_t>(a));
            it != m_cursorStops.end() && *it < b; ++it
        ) {
            auto next = std::next(it);
            if (next == m_cursorStops.end()) break;

            auto left = this->stopPos(*it);
            auto right = this->stopPos(*next);
            if (!left || !right) continue;

            if (std::abs(left->y - right->y) > 0.5f) continue;

            auto& ext = lines[left->y];
            ext.minX = std::min(ext.minX, left->x);
            ext.maxX = std::max(ext.maxX, right->x);
        }

        auto fill = ccColor4F{
            m_selectionColor.r / 255.f,
            m_selectionColor.g / 255.f,
            m_selectionColor.b / 255.f,
            m_selectionColor.a / 255.f
        };

        for (auto& [y, ext] : lines) {
            if (ext.minX >= ext.maxX) continue;
            std::array verts = {
                CCPoint{ext.minX, y},
                CCPoint{ext.minX, y + lineHeight},
                CCPoint{ext.maxX, y + lineHeight},
                CCPoint{ext.maxX, y},
            };
            m_selectionDraw->drawPolygon(
                verts.data(), verts.size(),
                fill, 0.f, ccColor4F{},
                BorderAlignment::Inside
            );
        }
    }

    void selectAll() {
        if (m_textU32.empty()) return;
        m_selectionStart = 0;
        m_cursorPos = m_textU32.size();
        this->updateCaretPosition();
        this->updateSelectionHighlight();
        this->resetCaretBlink();
    }

    void copySelection() const {
        if (!this->hasSelection() || m_password) return;
        int a = std::min(m_selectionStart, m_cursorPos);
        int b = std::max(m_selectionStart, m_cursorPos);

        std::u32string_view selection(m_textU32.data() + a, b - a);
        fmt::basic_memory_buffer<char> buffer;
        buffer.resize(simdutf::utf8_length_from_utf32(selection));
        (void)simdutf::convert_utf32_to_utf8(selection.data(), selection.size(), buffer.data());
        buffer.push_back('\0');

        clipboard::write(buffer.data());
    }

    void cutSelection() {
        if (!this->hasSelection() || m_password) return;
        this->copySelection();
        this->deleteSelection();
        this->syncLabel();
    }

    void applyFilter(fmt::basic_memory_buffer<char32_t>& inserted) const {
        if (m_filterChars.empty()) return;

        size_t out = 0;
        for (size_t i = 0; i < inserted.size(); ++i) {
            char32_t c = inserted.data()[i];
            bool allow = m_multiline && c == U'\n';
            if (!allow) {
                allow = std::ranges::find(m_filterChars, c) != m_filterChars.end();
            }
            if (allow) {
                inserted.data()[out++] = c;
            }
        }
        inserted.resize(out);
    }

    void setString(std::string_view str, bool triggerCallback) {
        if (!triggerCallback && str == m_text) return;

        m_textU32.clear();
        if (!str.empty()) {
            m_textU32.resize(simdutf::utf32_length_from_utf8(str));
            (void)simdutf::convert_utf8_to_utf32(str.data(), str.size(), m_textU32.data());
        }

        m_cursorPos = m_textU32.size();
        m_selectionStart = -1;
        this->syncLabel(triggerCallback);
    }

    void applyLayout() {
        auto availW = std::max(0.f, m_parent->getContentWidth() - m_padding.x * 2.f);
        auto availH = std::max(0.f, m_parent->getContentHeight() - m_padding.y * 2.f);

        m_label->disableFitBox();

        if (!m_multiline) {
            // single-line
            m_label->setMaxWidth(0.f);
            if (m_overflow == OverflowMode::Scale) {
                m_label->setLimitLabelWidth(availW, m_fontScale);
            } else {
                m_label->setScale(m_fontScale);
                m_label->removeLabelSizeLimit();
            }
        } else {
            if (m_wrap) {
                m_label->setMaxWidth(availW / m_fontScale);
            } else {
                m_label->setMaxWidth(0.f);
            }

            if (m_overflow == OverflowMode::Scale) {
                if (m_wrap) {
                    // wrap + scale down = scale both axes
                    m_label->setFitBox({availW, availH}, m_fontScale);
                } else {
                    // !wrap + scale down = scale width only
                    m_label->setLimitLabelWidth(availW, m_fontScale);
                }
            } else {
                // clip = no scale, the content can be scrolled instead
                m_label->removeLabelSizeLimit();
                m_label->setScale(m_fontScale);
            }
        }

        m_placeholderLabel->setLimitLabelWidth(availW, m_placeholderScale);
        this->syncLabel(false);
    }

    static CCAction* createBlinkAction() {
        return CCRepeatForever::create(CCSequence::create(
            CCDelayTime::create(0.5f),
            CCFadeOut::create(0.25f),
            CCDelayTime::create(0.25f),
            CCFadeIn::create(0.25f),
            nullptr
        ));
    }

    void resetCaretBlink() const {
        m_caret->stopAllActions();
        m_caret->setOpacity(255);
        m_caret->runAction(createBlinkAction());
    }

    void insertNewline() {
        if (this->hasSelection()) this->deleteSelection();
        if (m_maxLength != std::numeric_limits<size_t>::max() && m_textU32.size() >= m_maxLength) {
            return;
        }

        m_textU32.insert(m_textU32.begin() + m_cursorPos, U'\n');
        m_cursorPos += 1;
        this->syncLabel();
    }

    void moveCursorVertical(int direction, bool extendSelection) {
        if (!m_multiline || m_cursorStops.size() <= 1) return;

        auto lines = this->visualLines();
        int lineCount = lines.size();
        if (lineCount <= 1) return;

        int curLine = 0;
        bool found = false;
        for (auto const& line : lines) {
            auto& stops = line.stops;
            if (std::ranges::find(stops, m_cursorPos) != stops.end()) {
                found = true;
                break;
            }
            ++curLine;
        }
        if (!found) return;

        int targetLine = curLine + direction;
        if (targetLine < 0 || targetLine >= lineCount) return;

        if (m_goalX < 0.f) {
            m_goalX = this->stopPos(m_cursorPos).value_or(CCPoint{}).x;
        }

        auto& stops = lines[targetLine].stops;
        int newPos = stops.front();
        float bestDx = std::numeric_limits<float>::max();
        for (int stop : stops) {
            auto pos = this->stopPos(stop);
            if (!pos) continue;
            float dx = std::abs(pos->x - m_goalX);
            if (dx < bestDx) {
                bestDx = dx;
                newPos = stop;
            }
        }
        if (newPos == m_cursorPos) return;

        if (extendSelection) {
            if (m_selectionStart < 0) m_selectionStart = m_cursorPos;
        } else {
            this->clearSelection();
        }

        m_cursorPos = newPos;
        this->updateCaretPosition();
        this->updateSelectionHighlight();
    }

    struct KeyMods {
        bool word = false;
        bool line = false;
    };

    static KeyMods getCurrentMods() {
        auto kd = CCKeyboardDispatcher::get();
#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
        return {
            .word = kd->getAltKeyPressed(),
            .line = kd->getCommandKeyPressed(),
        };
#else
        return {
            .word = kd->getControlKeyPressed(),
            .line = false,
        };
#endif
    }

    void setupKeyboardListener() {
        m_keyListener = KeyboardInputEvent().listen([this](KeyboardInputData& data) {
            if (!m_hasFocus) return ListenerResult::Propagate;
            if (data.action == KeyboardInputData::Action::Release) {
                return ListenerResult::Propagate;
            }

#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
            bool ctrl = data.modifiers & KeyboardModifier::Super;
#else
            bool ctrl = data.modifiers & KeyboardModifier::Control;
#endif
            bool shift = data.modifiers & KeyboardModifier::Shift;

            switch (data.key) {
                case KEY_Enter: {
                    if (m_multiline) {
                        ContextMenu::closeAll();
                        this->insertText("\n", 1, KEY_Enter);
                        return ListenerResult::Stop;
                    }
                    return ListenerResult::Propagate;
                }
                case KEY_A: {
                    if (ctrl) {
                        ContextMenu::closeAll();
                        this->selectAll();
                        this->resetCaretBlink();
                        return ListenerResult::Stop;
                    }
                    return ListenerResult::Propagate;
                }
                case KEY_C: {
                    if (ctrl && this->hasSelection()) {
                        ContextMenu::closeAll();
                        this->copySelection();
                        return ListenerResult::Stop;
                    }
                    return ListenerResult::Propagate;
                }
                case KEY_X: {
                    if (ctrl && this->hasSelection()) {
                        ContextMenu::closeAll();
                        this->cutSelection();
                        this->resetCaretBlink();
                        return ListenerResult::Stop;
                    }
                    return ListenerResult::Propagate;
                }
                case KEY_Home: {
                    ContextMenu::closeAll();
                    if (shift) {
                        if (m_selectionStart < 0) {
                            m_selectionStart = m_cursorPos;
                        }
                    } else {
                        this->clearSelection();
                    }
                    m_cursorPos = m_multiline ? this->lineStartFor(m_cursorPos) : 0;
                    m_goalX = -1.f;
                    this->updateCaretPosition();
                    this->updateSelectionHighlight();
                    this->resetCaretBlink();
                    return ListenerResult::Stop;
                }
                case KEY_End: {
                    ContextMenu::closeAll();
                    if (shift) {
                        if (m_selectionStart < 0) {
                            m_selectionStart = m_cursorPos;
                        }
                    } else {
                        this->clearSelection();
                    }
                    m_cursorPos = m_multiline ? this->lineEndFor(m_cursorPos) : m_textU32.size();
                    m_goalX = -1.f;
                    this->updateCaretPosition();
                    this->updateSelectionHighlight();
                    this->resetCaretBlink();
                    return ListenerResult::Stop;
                }
                default: return ListenerResult::Propagate;
            }
        });
    }

    void insertText(char const* text, int len, enumKeyCodes key) {
        this->resetCaretBlink();

        auto kd = CCKeyboardDispatcher::get();
        bool shift = kd->getShiftKeyPressed();
        auto mods = this->getCurrentMods();

        switch (key) {
            case KEY_Left: {
                if (!shift && this->hasSelection()) {
                    m_cursorPos = std::min(m_selectionStart, m_cursorPos);
                    this->clearSelection();
                } else {
                    if (shift && m_selectionStart < 0) m_selectionStart = m_cursorPos;
                    if (mods.line) {
                        m_cursorPos = m_multiline ? this->lineStartFor(m_cursorPos) : 0;
                    } else if (mods.word) {
                        m_cursorPos = this->findPrevWordStop(m_cursorPos);
                    } else {
                        m_cursorPos = this->snapToPrevStop(m_cursorPos);
                    }
                }
                m_goalX = -1.f;
                this->updateCaretPosition();
                this->updateSelectionHighlight();
                return;
            }
            case KEY_Right: {
                if (!shift && this->hasSelection()) {
                    m_cursorPos = std::max(m_selectionStart, m_cursorPos);
                    this->clearSelection();
                } else {
                    if (shift && m_selectionStart < 0) m_selectionStart = m_cursorPos;
                    if (mods.line) {
                        m_cursorPos = m_multiline ? this->lineEndFor(m_cursorPos) : m_text.size();
                    } else if (mods.word) {
                        m_cursorPos = this->findNextWordStop(m_cursorPos);
                    } else {
                        m_cursorPos = this->snapToNextStop(m_cursorPos);
                    }
                }
                m_goalX = -1.f;
                this->updateCaretPosition();
                this->updateSelectionHighlight();
                return;
            }
            case KEY_Up:
            case KEY_Down: {
                this->moveCursorVertical(key == KEY_Up ? 1 : -1, shift);
                return;
            }
            case KEY_Enter: {
                if (m_multiline) {
                    this->insertNewline();
                } else if (m_onEnter) {
                    m_onEnter(m_text);
                }
                return;
            }
            default:
                break;
        }

        if (!text || len <= 0) return;

        fmt::basic_memory_buffer<char32_t> buf;
        buf.resize(simdutf::utf32_length_from_utf8(text, len));
        if (simdutf::convert_utf8_to_utf32(text, len, buf.data()) == 0) {
            return;
        }

        // normalize line endings
        {
            size_t out = 0;
            for (size_t i = 0; i < buf.size(); ++i) {
                char32_t c = buf.data()[i];
                if (c == U'\r') continue;
                if (c == U'\n' && !m_multiline) c = U' ';
                buf.data()[out++] = c;
            }
            buf.resize(out);
        }

        if (buf.size() == 0) return;

        this->applyFilter(buf);
        if (buf.size() == 0) return;

        if (this->hasSelection()) this->deleteSelection();

        if (m_maxLength != std::numeric_limits<size_t>::max()) {
            size_t current = m_textU32.size();
            if (current >= m_maxLength) return;
            if (current + buf.size() > m_maxLength) {
                buf.resize(m_maxLength - current);
            }
        }

        m_textU32.insert(m_cursorPos, buf.data(), buf.size());
        m_cursorPos += buf.size();

        this->syncLabel();
    }
};

std::string const& geode::getCommonFilterAllowedChars(CommonFilter filter) {
    using CF = CommonFilter;
    auto bits = static_cast<uint32_t>(filter);

    static std::unordered_map<uint32_t, std::string> cache{{0, ""}};
    auto it = cache.find(bits);
    if (it != cache.end()) return it->second;

    auto has = [bits](CF f) {
        return (bits & static_cast<uint32_t>(f)) != 0;
    };

    std::u32string out;
    auto push = [&out](std::initializer_list<char32_t> cps) {
        out.insert(out.end(), cps.begin(), cps.end());
    };
    auto pushRange = [&out](char32_t from, char32_t to) {
        for (char32_t c = from; c <= to; ++c) out.push_back(c);
    };

    // ASCII
    if (has(CF::Uint)) pushRange(U'0', U'9');
    if (has(CF::Lowercase)) pushRange(U'a', U'z');
    if (has(CF::Uppercase)) pushRange(U'A', U'Z');
    if (has(CF::Hex)) {
        pushRange(U'0', U'9');
        pushRange(U'a', U'f');
        pushRange(U'A', U'F');
    }
    if (has(CF::Hyphen)) out.push_back(U'-');
    if (has(CF::Dot)) out.push_back(U'.');
    if (has(CF::Underscore)) out.push_back(U'_');
    if (has(CF::Space)) out.push_back(U' ');
    if (has(CF::Plus)) out.push_back(U'+');
    if (has(CF::Slash)) out.push_back(U'/');
    if (has(CF::Equals)) out.push_back(U'=');

    if (has(CF::Latin)) {
        pushRange(U'a', U'z');
        pushRange(U'A', U'Z');
        pushRange(0x00C0, 0x00D6); // À-Ö
        pushRange(0x00D8, 0x00F6); // Ø-ö
        pushRange(0x00F8, 0x00FF); // ø-ÿ
        pushRange(0x0100, 0x017F); // Ā-ſ (Latin Extended-A)
        pushRange(0x0180, 0x024F); // ƀ-ɏ (Latin Extended-B)
        pushRange(0x1E00, 0x1EFF); // Ḁ-ỿ (Latin Extended Additional)
        pushRange(0x0300, 0x036F); // diacritics
        push({0x0259}); // ə
    }

    if (has(CF::Cyrillic)) {
        pushRange(0x0400, 0x04FF); // Ѐ-ӿ (Cyrillic)
        pushRange(0x0500, 0x052F); // Ԁ-ԯ (Cyrillic Supplement)
    }

    if (has(CommonFilter::Emoji)) {
        auto& reg = EmojiRegistry::vanillaEmojis();
        for (auto seq : reg.getSequences()) {
            for (char32_t c : seq) {
                if (c >= 0x80) out.push_back(c); // skip :name: ASCII chars
            }
        }

        out.push_back(0x200D); // ZWJ (for 😵‍💫)
    }

    if (has(CF::Printable)) {
        for (char32_t c = 0x20; c <= 0x7E; ++c) {
            bool alnum = (c >= U'0' && c <= U'9')
                      || (c >= U'a' && c <= U'z')
                      || (c >= U'A' && c <= U'Z');
            if (!alnum) out.push_back(c);
        }
        out.push_back(0x2022); // •
    }

    if (has(CF::PrintableExt)) {
        for (char32_t c = 0x20; c <= 0x7E; ++c) {
            bool alnum = (c >= U'0' && c <= U'9')
                      || (c >= U'a' && c <= U'z')
                      || (c >= U'A' && c <= U'Z');
            if (!alnum) out.push_back(c);
        }

        // Latin-1 Supplement punctuation
        push({
            0x00A1, 0x00A3, 0x00A5, 0x00A9, 0x00AB, 0x00AE, 0x00B0,
            0x00B1, 0x00B2, 0x00B3, 0x00B7, 0x00BB, 0x00BD, 0x00BF
        });
        // General Punctuation
        push({
            0x2013, 0x2014, 0x2018, 0x2019, 0x201A, 0x201C, 0x201D,
            0x201E, 0x2022, 0x2026, 0x2039, 0x203A
        });
        // Currency Symbols
        push({0x20A9, 0x20AA, 0x20AB, 0x20AC, 0x20B9, 0x20BA, 0x20BD});
        // Letterlike Symbols
        push({0x2116, 0x2122});
        // Arrows
        pushRange(0x2190, 0x2194);
        // Math operators
        push({0x2248, 0x2260});
        // Misc Symbols
        push({0x2605, 0x2606, 0x2661, 0x2665, 0x266A, 0x266B});
        // Dingbats ✓✔✕✖✗
        pushRange(0x2713, 0x2717);
    }

    // dedup
    std::ranges::sort(out);
    out.erase(std::ranges::unique(out).begin(), out.end());

    std::string result;
    result.resize(simdutf::utf8_length_from_utf32(out));
    (void)simdutf::convert_utf32_to_utf8(out.data(), out.size(), result.data());

    auto [it2, _] = cache.emplace(bits, std::move(result));
    return it2->second;
}

TextInput::TextInput() : m_impl(std::make_unique<Impl>(this)) {}
TextInput::~TextInput() = default;

TextInput* TextInput::create(float width, std::string placeholder, ZStringView font) {
    auto ret = new TextInput();
    if (ret->initWithWidth(width, std::move(placeholder), font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

TextInput* TextInput::create(CCSize const& size, std::string placeholder, ZStringView font) {
    auto ret = new TextInput();
    if (ret->initWithSize(size, std::move(placeholder), font, false)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

TextInput* TextInput::createMultiline(CCSize const& size, std::string placeholder, ZStringView font) {
    auto ret = new TextInput();
    if (ret->initWithSize(size, std::move(placeholder), font, true)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool TextInput::attachWithIME() {
    if (!m_impl->m_enabled) return false;
    if (!cocos::nodeIsVisible(this)) return false;
    if (m_impl->m_hasFocus) return true;

#ifdef GEODE_IS_MOBILE
    int flags = 0;
    if (m_impl->m_multiline) flags |= 1;
    if (m_impl->m_password) flags |= 2;
    if (m_impl->m_numeric) flags |= 4;

    PlatformToolbox::setKeyboardInputFlags(flags);
#endif

    return CCIMEDelegate::attachWithIME();
}

bool TextInput::detachWithIME() {
    if (m_impl->m_hasFocus) {
        CCIMEDelegate::detachWithIME();
    }
    return true;
}

static bool s_keyboardVisible = false;
static CCRect s_keyboardRect;

void TextInput::applyKeyboardOffset(CCRect keyboardRect) {
    auto size = this->getContentSize();
    auto bl = this->convertToWorldSpace({0.f, 0.f});
    auto tr = this->convertToWorldSpace({size.width, size.height});
    CCRect rect{bl.x, bl.y, tr.x - bl.x, tr.y - bl.y};
    rect.origin.y -= 4.f;

    float offset = keyboardRect.getMaxY() - rect.getMinY();

    auto test = rect;
    if (offset < 0.f) test.origin.y += offset;
    if (!test.intersectsRect(keyboardRect)) return;

    m_impl->m_keyboardShowing = true;
    if (m_impl->m_onKeyboardOffset) {
        m_impl->m_onKeyboardOffset(offset);
    }
}

void TextInput::returnKeyboardOffset() {
    if (!m_impl->m_keyboardShowing) return;
    m_impl->m_keyboardShowing = false;
    if (m_impl->m_onKeyboardHide) {
        m_impl->m_onKeyboardHide();
    }
}

void TextInput::keyboardWillHide(CCIMEKeyboardNotificationInfo& info) {
    s_keyboardVisible = false;
    if (m_impl->m_hasFocus) {
        this->detachWithIME();
    }

    this->returnKeyboardOffset();
}

void TextInput::keyboardWillShow(CCIMEKeyboardNotificationInfo& info) {
    s_keyboardVisible = true;
    s_keyboardRect = info.end;

    if (!m_impl->m_hasFocus) return;
    this->applyKeyboardOffset(info.end);
}

void TextInput::onEnter() {
    CCNode::onEnter();
    this->registerWithTouchDispatcher();

    if (m_impl->m_refocusOnEnter) {
        m_impl->m_refocusOnEnter = false;
        this->attachWithIME();
    }
}

void TextInput::onExit() {
    if (m_impl->m_hasFocus) {
        m_impl->m_refocusOnEnter = true;
    }
    CCNode::onExit();
    this->detachWithIME();
    CCTouchDispatcher::get()->removeDelegate(this);
    CCMouseDispatcher::get()->removeDelegate(this);
}

void TextInput::setContentSize(CCSize const& contentSize) {
    CCNode::setContentSize(contentSize);

    m_impl->m_background->setContentSize(contentSize);
    m_impl->m_clip->getStencil()->setContentSize(contentSize);
    m_impl->m_clip->setContentSize(contentSize);
    if (m_impl->m_captionLabel) {
        m_impl->m_captionLabel->setPosition({0.f, contentSize.height + 2.f});
    }

    m_impl->clampScroll();
    m_impl->applyLayout();
    this->updateLayout();
}

void TextInput::setPasswordMode(bool enable) {
    if (m_impl->m_password == enable) return;
    m_impl->m_password = enable;
    m_impl->syncLabel(false);
}

bool TextInput::isPasswordMode() const {
    return m_impl->m_password;
}

void TextInput::setNumericMode(bool enable) {
    m_impl->m_numeric = enable;
}

bool TextInput::isNumeric() const {
    return m_impl->m_numeric;
}

void TextInput::setMultiline(bool multiline) {
    if (m_impl->m_multiline == multiline) return;
    m_impl->m_multiline = multiline;
    m_impl->applyLayout();
}

bool TextInput::isMultiline() const {
    return m_impl->m_multiline;
}

void TextInput::setEnabled(bool enabled) {
    m_impl->m_enabled = enabled;
    m_impl->m_background->setOpacity(enabled ? 90 : 50);
    if (!enabled && m_impl->m_hasFocus) {
        this->detachWithIME();
    }
}

bool TextInput::isEnabled() const {
    return m_impl->m_enabled;
}

void TextInput::setWrap(bool wrap) {
    if (m_impl->m_wrap == wrap) return;
    m_impl->m_wrap = wrap;
    m_impl->applyLayout();
}

bool TextInput::isWrap() const {
    return m_impl->m_wrap;
}

void TextInput::setOverflowMode(OverflowMode mode) {
    if (m_impl->m_overflow == mode) return;
    m_impl->m_overflow = mode;
    m_impl->applyLayout();
}

TextInput::OverflowMode TextInput::getOverflowMode() const {
    return m_impl->m_overflow;
}

void TextInput::setFontScale(float scale) {
    m_impl->m_fontScale = scale;
    m_impl->applyLayout();
}

float TextInput::getFontScale() const {
    return m_impl->m_fontScale;
}

void TextInput::setPlaceholderScale(float scale) {
    m_impl->m_placeholderScale = scale;
    m_impl->applyLayout();
}

float TextInput::getPlaceholderScale() const {
    return m_impl->m_placeholderScale;
}

void TextInput::setDisplayEmojis(bool allow) {
    if (allow) {
        m_impl->m_label->setEmojiRegistry(EmojiRegistry::vanillaEmojis());
    } else {
        m_impl->m_label->disableEmojis();
    }
}

void TextInput::hideBG() {
    m_impl->m_background->setVisible(false);
}

void TextInput::setCallback(Function<void(std::string const&)> onInput) {
    m_impl->m_onTextChanged = std::move(onInput);
    m_impl->m_textChangedListener = TextChangedEvent(this).listen([this](std::string const& value) {
        if (m_impl->m_onTextChanged) m_impl->m_onTextChanged(value);
    });
}

void TextInput::setEnterCallback(Function<void(std::string const&)> onEnter) {
    m_impl->m_onEnter = std::move(onEnter);
}

void TextInput::setFocusCallback(Function<void(FocusState state)> onFocusChange) {
    m_impl->m_onFocusChanged = std::move(onFocusChange);
    m_impl->m_focusedListener = FocusEvent(this, FocusState::Focused).listen([this] {
        if (m_impl->m_onFocusChanged) m_impl->m_onFocusChanged(FocusState::Focused);
    });
    m_impl->m_unfocusedListener = FocusEvent(this, FocusState::Unfocused).listen([this] {
        if (m_impl->m_onFocusChanged) m_impl->m_onFocusChanged(FocusState::Unfocused);
    });
}

void TextInput::setKeyboardOffsetCallback(Function<void(float offset)> onKeyboardOffset) {
    m_impl->m_onKeyboardOffset = std::move(onKeyboardOffset);
}

void TextInput::setKeyboardHideCallback(Function<void()> onKeyboardHide) {
    m_impl->m_onKeyboardHide = std::move(onKeyboardHide);
}

void TextInput::setString(std::string_view str, bool triggerCallback) {
    m_impl->setString(str, triggerCallback);
}

std::string const& TextInput::getString() const {
    return m_impl->m_text;
}

void TextInput::focus() {
    this->attachWithIME();
}

void TextInput::unfocus() {
    this->detachWithIME();
}

bool TextInput::isFocused() const {
    return m_impl->m_hasFocus;
}

Label* TextInput::getContentLabel() const {
    return m_impl->m_label;
}

Label* TextInput::getPlaceholderLabel() const {
    return m_impl->m_placeholderLabel;
}

Label* TextInput::getCaptionLabel() const {
    return m_impl->m_captionLabel;
}

NineSlice* TextInput::getBGSprite() const {
    return m_impl->m_background;
}

bool TextInput::canAttachWithIME() {
    return true;
}

bool TextInput::canDetachWithIME() {
    return true;
}

void TextInput::didAttachWithIME() {
    m_impl->m_hasFocus = true;
    m_impl->m_caret->setVisible(true);
    m_impl->resetCaretBlink();
    m_impl->updateCaretPosition();

    if (auto gl = CCEGLView::get()) {
        gl->setIMEKeyboardState(true);
    }

    m_impl->setupKeyboardListener();

    FocusEvent(this, FocusState::Focused).send();

    if (s_keyboardVisible) {
        this->applyKeyboardOffset(s_keyboardRect);
    }
}

void TextInput::didDetachWithIME() {
    ContextMenu::closeAll();
    m_impl->m_hasFocus = false;
    m_impl->m_caret->setVisible(false);

    if (auto gl = CCEGLView::get()) {
        gl->setIMEKeyboardState(false);
    }

    m_impl->clearSelection();
    m_impl->updateSelectionHighlight();
    m_impl->m_keyListener.destroy();

    FocusEvent(this, FocusState::Unfocused).send();
    this->returnKeyboardOffset();
}

void TextInput::insertText(char const* text, int len, enumKeyCodes key) {
    if (!m_impl->m_enabled) return;
    if (!cocos::nodeIsVisible(this)) return;
    m_impl->insertText(text, len, key);
}

void TextInput::deleteBackward() {
    if (!m_impl->m_enabled) return;
    if (!cocos::nodeIsVisible(this)) return;

    if (m_impl->hasSelection()) {
        m_impl->deleteSelection();
        m_impl->syncLabel();
        return;
    }

    if (m_impl->m_cursorPos <= 0) return;

    auto mods = Impl::getCurrentMods();

    int newPos;
    if (mods.line) {
        newPos = m_impl->lineStartFor(m_impl->m_cursorPos);
    } else if (mods.word) {
        newPos = m_impl->findPrevWordStop(m_impl->m_cursorPos);
    } else {
        newPos = m_impl->snapToPrevStop(m_impl->m_cursorPos);
    }

    if (newPos >= m_impl->m_cursorPos) return;

    m_impl->m_textU32.erase(newPos, m_impl->m_cursorPos - newPos);
    m_impl->m_cursorPos = newPos;
    m_impl->syncLabel();
    m_impl->resetCaretBlink();
}

void TextInput::deleteForward() {
    if (!m_impl->m_enabled) return;
    if (!cocos::nodeIsVisible(this)) return;

    if (m_impl->hasSelection()) {
        m_impl->deleteSelection();
        m_impl->syncLabel();
        return;
    }

    if (m_impl->m_cursorPos >= m_impl->m_textU32.size()) return;

    auto mods = Impl::getCurrentMods();

    int newPos;
    if (mods.line) {
        newPos = m_impl->lineEndFor(m_impl->m_cursorPos);
    } else if (mods.word) {
        newPos = m_impl->findNextWordStop(m_impl->m_cursorPos);
    } else {
        newPos = m_impl->snapToNextStop(m_impl->m_cursorPos);
    }

    if (newPos <= m_impl->m_cursorPos) return;

    m_impl->m_textU32.erase(m_impl->m_cursorPos, newPos - m_impl->m_cursorPos);
    m_impl->syncLabel();
    m_impl->resetCaretBlink();
}

char const* TextInput::getContentText() {
    return m_impl->m_text.c_str();
}

bool TextInput::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_enabled) return false;
    if (!cocos::nodeIsVisible(this)) return false;

    auto local = this->convertToNodeSpace(touch->getLocation());
    CCRect rect{0.f, 0.f, this->getContentWidth(), this->getContentHeight()};
    if (!rect.containsPoint(local)) {
        this->detachWithIME();
        return false;
    }

    this->attachWithIME();

    auto labelPos = m_impl->m_label->convertToNodeSpace(touch->getLocation());
    int newPos = m_impl->codepointAtLabelPos(labelPos);

    auto now = asp::time::SystemTime::now();
    bool quick = m_impl->m_lastClickTime.elapsed().millis() < 300;
    bool nearPos = touch->getLocation().getDistance(m_impl->m_lastClickPos) < 5.f;
    m_impl->m_clickCount = (quick && nearPos) ? m_impl->m_clickCount + 1 : 1;
    m_impl->m_lastClickTime = now;
    m_impl->m_lastClickPos = touch->getLocation();

    int clickType = (m_impl->m_clickCount - 1) % 3;
    if (clickType == 2) {
        // triple click - select line
        auto range = m_impl->lineRangeAt(newPos);
        m_impl->m_clickMode = Impl::ClickMode::Line;
        m_impl->m_clickAnchor = range;
        m_impl->m_selectionStart = range.first;
        m_impl->m_cursorPos = range.second;
    } else if (clickType == 1) {
        // double click - select word
        auto range = m_impl->wordRangeAt(newPos);
        m_impl->m_clickMode = Impl::ClickMode::Word;
        m_impl->m_clickAnchor = range;
        m_impl->m_selectionStart = range.first;
        m_impl->m_cursorPos = range.second;
    } else {
        // normal click - move cursor
        m_impl->m_clickMode = Impl::ClickMode::None;
        if (CCKeyboardDispatcher::get()->getShiftKeyPressed()) {
            if (m_impl->m_selectionStart < 0) {
                m_impl->m_selectionStart = m_impl->m_cursorPos;
            }
        } else {
            m_impl->m_selectionStart = -1;
        }
        m_impl->m_cursorPos = newPos;
    }

    m_impl->m_goalX = -1.f;
    m_impl->m_isDragging = true;

    m_impl->updateCaretPosition();
    m_impl->updateSelectionHighlight();
    m_impl->resetCaretBlink();
    return true;
}

void TextInput::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    if (!m_impl->m_isDragging || !m_impl->m_enabled) return;
    if (!cocos::nodeIsVisible(this)) return;

    m_impl->m_goalX = -1.f;

    auto labelPos = m_impl->m_label->convertToNodeSpace(touch->getLocation());
    int newPos = m_impl->codepointAtLabelPos(labelPos);

    if (m_impl->m_clickMode != Impl::ClickMode::None) {
        m_impl->extendClickSelection(newPos);
        m_impl->updateCaretPosition();
        m_impl->updateSelectionHighlight();
        m_impl->resetCaretBlink();
        return;
    }

    if (newPos == m_impl->m_cursorPos) return;

    if (m_impl->m_selectionStart < 0) {
        m_impl->m_selectionStart = m_impl->m_cursorPos;
    }

    m_impl->m_cursorPos = newPos;
    m_impl->updateCaretPosition();
    m_impl->updateSelectionHighlight();
    m_impl->resetCaretBlink();
}

void TextInput::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    m_impl->m_isDragging = false;
    m_impl->m_clickMode = Impl::ClickMode::None;
    if (m_impl->m_selectionStart == m_impl->m_cursorPos) {
        m_impl->m_selectionStart = -1;
        m_impl->updateSelectionHighlight();
    }
}

void TextInput::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    m_impl->m_isDragging = false;
    m_impl->m_clickMode = Impl::ClickMode::None;
}

bool TextInput::scrollWheel(float y, float x) {
    if (!m_impl->m_enabled) return false;
    if (!cocos::nodeIsVisible(this)) return false;

    y *= 0.1f;
    x *= 0.1f;

    auto mousePos = geode::cocos::getMousePos();
    auto local = this->convertToNodeSpace(mousePos);
    CCRect rect{0.f, 0.f, this->getContentWidth(), this->getContentHeight()};
    if (!rect.containsPoint(local)) return false;

    float lineH = std::max(1.f, m_impl->m_label->getLineHeight());
    auto before = m_impl->m_scroll;

    if (m_impl->m_multiline) {
        m_impl->m_scroll.y += y * lineH;
        m_impl->m_scroll.x -= x * lineH;
    }
    else {
        m_impl->m_scroll.x += (y - x) * lineH;
    }

    m_impl->clampScroll();
    return m_impl->m_scroll != before;
}

// i'm sorry, but this is what RobTop does for CCTextInputNode
static bool isCoveredByPopup(TextInput* input) {
    auto touchDispatcher = CCTouchDispatcher::get();
    auto handler = touchDispatcher->findHandler(input);
    if (!handler) return false;
    return handler->getPriority() > touchDispatcher->getTargetPrio();
}

bool TextInput::rightClick(float x, float y) {
    if (!m_impl->m_enabled) return false;
    if (!cocos::nodeIsVisible(this)) return false;

    auto local = this->convertToNodeSpace({x, y});
    CCRect rect{0.f, 0.f, this->getContentWidth(), this->getContentHeight()};
    if (!rect.containsPoint(local) || isCoveredByPopup(this)) return false;

    this->attachWithIME();

    auto labelPos = m_impl->m_label->convertToNodeSpace({x, y});
    int newPos = m_impl->codepointAtLabelPos(labelPos);

    bool insideSelection = false;
    if (m_impl->hasSelection()) {
        int a = std::min(m_impl->m_selectionStart, m_impl->m_cursorPos);
        int b = std::max(m_impl->m_selectionStart, m_impl->m_cursorPos);
        insideSelection = newPos >= a && newPos <= b;
    }

    if (!insideSelection) {
        m_impl->m_selectionStart = -1;
        m_impl->m_cursorPos = newPos;
        m_impl->m_goalX = -1.f;
        m_impl->updateCaretPosition();
        m_impl->updateSelectionHighlight();
        m_impl->resetCaretBlink();
    }

    auto ctx = ContextMenu::create();

    bool hasSel = m_impl->hasSelection();
    bool hasText = !m_impl->m_textU32.empty();
    bool hasPassword = m_impl->m_password;

#if defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
    #define CTRL_NAME "Cmd"
#else
    #define CTRL_NAME "Ctrl"
#endif

    if (hasSel && !hasPassword) {
        ctx->addItem("Cut", [self = WeakRef(this)] {
            if (auto s = self.lock()) {
                s->m_impl->cutSelection();
                s->m_impl->resetCaretBlink();
            }
        }, CTRL_NAME "+X");
        ctx->addItem("Copy", [self = WeakRef(this)] {
            if (auto s = self.lock()) {
                s->m_impl->copySelection();
            }
        }, CTRL_NAME "+C");
    }

    if (!hasPassword) {
        if (!clipboard::read().empty()) {
            ctx->addItem("Paste", [self = WeakRef(this)] {
                if (auto s = self.lock()) {
                    auto text = clipboard::read();
                    if (!text.empty()) {
                        s->m_impl->insertText(
                            text.data(),
                            static_cast<int>(text.size()),
                            KEY_None
                        );
                    }
                }
            }, CTRL_NAME "+V");
        }
    }

    if (hasText) {
        ctx->addItem("Select All", [self = WeakRef(this)] {
            if (auto s = self.lock()) {
                s->m_impl->selectAll();
                s->m_impl->resetCaretBlink();
            }
        }, CTRL_NAME "+A");
        ctx->addSeparator();
        ctx->addItem("Clear", [self = WeakRef(this)] {
            if (auto s = self.lock()) {
                s->m_impl->m_textU32.clear();
                s->m_impl->m_cursorPos = 0;
                s->m_impl->m_selectionStart = -1;
                s->m_impl->syncLabel();
            }
        });
    }

#undef CTRL_NAME

    ctx->show({x, y});

    return true;
}

void TextInput::registerWithTouchDispatcher() {
    // rob uses -500 so i guess that's fine
    CCTouchDispatcher::get()->addTargetedDelegate(this, -500, true);
    CCMouseDispatcher::get()->addDelegate(this);
}

void TextInput::setPlaceholder(std::string placeholder) {
    m_impl->m_placeholderLabel->setText(std::move(placeholder));
}

void TextInput::setPlaceholderColor(ccColor4B color) {
    m_impl->m_placeholderLabel->setColor({color.r, color.g, color.b});
    m_impl->m_placeholderLabel->setOpacity(color.a);
}

void TextInput::setSelectionColor(ccColor4B color) {
    if (m_impl->m_selectionColor == color) return;
    m_impl->m_selectionColor = color;
    m_impl->updateSelectionHighlight();
}

void TextInput::setCaretColor(ccColor4B color) {
    m_impl->m_caret->setColor({color.r, color.g, color.b});
    m_impl->m_caret->setOpacity(color.a);
}

void TextInput::setCaretWidth(float width) {
    m_impl->m_caret->setContentSize({
        std::max(1.f, width),
        std::max(1.f, m_impl->m_label->getLineHeight())
    });
}

void TextInput::setTextColor(ccColor4B color) {
    m_impl->m_label->setColor({color.r, color.g, color.b});
    m_impl->m_label->setOpacity(color.a);
}

void TextInput::setPadding(CCPoint padding) {
    if (m_impl->m_padding == padding) return;
    m_impl->m_padding = padding;
    m_impl->applyLayout();
}

CCPoint TextInput::getPadding() const {
    return m_impl->m_padding;
}

void TextInput::setAlignment(Label::Alignment alignment) {
    if (m_impl->m_alignment == alignment) return;
    m_impl->m_alignment = alignment;
    m_impl->clampScroll();
}

Label::Alignment TextInput::getAlignment() const {
    return m_impl->m_alignment;
}

void TextInput::setFilter(std::string filter) {
    m_impl->m_filterText = std::move(filter);
    m_impl->m_filterChars.clear();
    if (!m_impl->m_filterText.empty()) {
        m_impl->m_filterChars.resize(simdutf::utf32_length_from_utf8(m_impl->m_filterText));
        (void)simdutf::convert_utf8_to_utf32(
            m_impl->m_filterText.data(),
            m_impl->m_filterText.size(),
            m_impl->m_filterChars.data()
        );
    }
}

void TextInput::setCommonFilter(CommonFilter filter) {
    this->setFilter(getCommonFilterAllowedChars(filter));
}

std::string const& TextInput::getFilter() const {
    return m_impl->m_filterText;
}

void TextInput::setLabel(std::string label) {
    if (label.empty()) {
        if (m_impl->m_captionLabel) m_impl->m_captionLabel->setVisible(false);
        return;
    }

    if (m_impl->m_captionLabel) {
        m_impl->m_captionLabel->setText(std::move(label));
        m_impl->m_captionLabel->setVisible(true);
        return;
    }

    auto caption = Label::create(std::move(label), m_impl->m_label->getFont());
    if (!caption) return;
    caption->setID("caption-label");
    caption->setAnchorPoint({0.f, 0.f});
    caption->setPosition({0.f, this->getContentHeight() + 2.f});
    this->addChild(caption);
    m_impl->m_captionLabel = caption;
}

void TextInput::setMaxCharCount(size_t length) {
    m_impl->m_maxLength = length == 0 ? std::numeric_limits<size_t>::max() : length;
    if (m_impl->m_textU32.size() > m_impl->m_maxLength) {
        m_impl->m_textU32.resize(m_impl->m_maxLength);
        auto size = static_cast<int>(m_impl->m_textU32.size());
        m_impl->m_cursorPos = std::min(m_impl->m_cursorPos, size);
        m_impl->m_selectionStart = std::min(m_impl->m_selectionStart, size);
        m_impl->syncLabel();
    }
}

size_t TextInput::getMaxCharCount() const {
    return m_impl->m_maxLength == std::numeric_limits<size_t>::max() ? 0 : m_impl->m_maxLength;
}

bool TextInput::initWithWidth(float width, std::string placeholder, ZStringView font) {
    return this->initWithSize({width, 0}, std::move(placeholder), font, false);
}

bool TextInput::initWithSize(CCSize const& size, std::string placeholder, ZStringView font, bool multiline) {
    if (!CCNode::init()) {
        return false;
    }

    m_impl->m_multiline = multiline;
    if (multiline) {
        // multiline is clipping by default
        m_impl->m_overflow = OverflowMode::Clip;
        m_impl->m_alignment = Label::Alignment::Left;
    }

    auto label = Label::create(font);
    auto placeholderLabel = Label::create(std::move(placeholder), font);
    auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});

    if (!label || !placeholderLabel || !background) {
        return false;
    }

    background->setID("background");
    background->setScaleMultiplier(0.5f);
    background->setColor({0, 0, 0});
    background->setOpacity(90);
    m_impl->m_background = background;

    auto labelAnchor = multiline ? CCPoint{0.f, 1.f} : CCPoint{0.f, 0.5f};

    label->setID("main-label");
    label->setAnchorPoint(labelAnchor);
    label->setBreakWords(true);
    label->setAlignment(m_impl->m_alignment);
    m_impl->m_label = label;

    placeholderLabel->setID("placeholder-label");
    placeholderLabel->setAnchorPoint(labelAnchor);
    placeholderLabel->setColor({150, 150, 150});
    placeholderLabel->setAlignment(m_impl->m_alignment);
    m_impl->m_placeholderLabel = placeholderLabel;

    float height = size.height;
    float lineHeight = m_impl->m_label->getLineHeight();
    if (height == 0.f) {
        height = lineHeight * m_impl->m_fontScale + m_impl->m_padding.y * 2.f;
    }

    this->setAnchorPoint({0.5f, 0.5f});

    auto caret = CCLayerColor::create({255, 255, 255, 255}, 1.05f, lineHeight);
    caret->setID("caret");
    caret->setVisible(false);
    caret->setAnchorPoint({0.5f, 0.5f});
    caret->ignoreAnchorPointForPosition(false);
    caret->runAction(m_impl->createBlinkAction());
    m_impl->m_caret = caret;
    m_impl->m_label->addChild(caret);

    auto selDraw = CCDrawNode::create();
    selDraw->setID("selection");
    selDraw->setPosition({0.f, 0.f});
    selDraw->setBlendFunc({GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA});
    m_impl->m_selectionDraw = selDraw;
    m_impl->m_label->addChild(selDraw, -1);

    auto stencil = CCLayerColor::create({255, 255, 255, 255}, size.width, height);
    stencil->setAnchorPoint({0.f, 0.f});
    stencil->setPosition({0.f, 0.f});

    m_impl->m_clip = CCClippingNode::create();
    m_impl->m_clip->setID("clip");
    m_impl->m_clip->setAnchorPoint({0.f, 0.f});
    m_impl->m_clip->setPosition({0.f, 0.f});
    m_impl->m_clip->setContentSize(this->getContentSize());
    m_impl->m_clip->setStencil(stencil);

    this->addChildAtPosition(m_impl->m_background, Anchor::Center);
    this->addChildAtPosition(m_impl->m_clip, Anchor::BottomLeft);

    m_impl->m_label->setScale(m_impl->m_fontScale);
    m_impl->m_placeholderLabel->setScale(m_impl->m_placeholderScale);

    m_impl->m_clip->addChild(m_impl->m_placeholderLabel);
    m_impl->m_clip->addChild(m_impl->m_label);

    this->setContentSize({size.width, height});
    m_impl->applyLayout();

    if (font == "chatFont.fnt") {
        this->setCommonFilter(CommonFilter::TextEmojis);
    } else {
        this->setDisplayEmojis(false);
        this->setCommonFilter(CommonFilter::Any);
    }

    return true;
}

struct ContextMenu::Impl {
    struct Entry {
        Ref<CCNode> content;
        Ref<Label> label;
        Function<void()> action;
    };

    std::vector<Entry> m_entries;

    static constexpr float BUTTON_SCALE = 0.35f;
    static constexpr float DESCRIPTION_SCALE = 0.45f;

    CCNode* makeRow(CCNode* parent, Entry& entry, float maxW) {
        auto* row = CCLayerColor::create({255, 255, 255, 0}, maxW, 1);
        row->ignoreAnchorPointForPosition(false);

        row->addChild(entry.content);
        if (entry.label) {
            row->addChild(entry.label);
        }

        row->setLayout(
            SimpleRowLayout::create()
                ->setMainAxisAlignment(MainAxisAlignment::Between)
                ->setCrossAxisAlignment(CrossAxisAlignment::Center)
                ->setMainAxisScaling(AxisScaling::None)
                ->setCrossAxisScaling(AxisScaling::Fit)
                ->setPadding(Padding::symmetric(4.f, 2.f))
        );

        auto btn = Button::createWithNode(row, [parent, callback = std::move(entry.action)](auto*) mutable {
            callback();
            parent->removeFromParent();
        });

        btn->setSelectCallback([row, label = entry.label](auto* btn) {
            row->stopAllActions();
            row->runAction(CCFadeTo::create(0.1f, 120));
            if (label) {
                label->stopAllActions();
                label->runAction(CCTintTo::create(0.1f, 255, 255, 255));
            }
        });

        btn->setUnselectCallback([row, label = entry.label](auto* btn) {
            row->stopAllActions();
            row->runAction(CCFadeTo::create(0.1f, 0));
            if (label) {
                label->stopAllActions();
                label->runAction(CCTintTo::create(0.1f, 175, 175, 175));
            }
        });

        btn->setAnimationType(Button::AnimationType::None);

        return btn;
    }

    void populateItems(CCNode* container) {
        float maxW = 0.f;
        for (auto& entry : m_entries) {
            if (!entry.content) continue;

            auto w = entry.content->getScaledContentWidth();
            if (entry.label) {
                w += entry.label->getScaledContentWidth() + 16.f;
            }

            maxW = std::max(maxW, w);
        }

        for (auto& entry : m_entries) {
            if (!entry.content) {
                // separator
                container->addChild(CCLayerColor::create({255, 255, 255, 160}, maxW, 0.5f));
                continue;
            }

            auto btn = makeRow(container, entry, maxW);
            container->addChild(btn);
        }
    }
};

ContextMenu::ContextMenu() : m_impl(std::make_unique<Impl>()) {}
ContextMenu::~ContextMenu() = default;

ContextMenu* ContextMenu::create() {
    auto ret = new ContextMenu();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void ContextMenu::onEnter() {
    CCNode::onEnter();
    CCTouchDispatcher::get()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

void ContextMenu::onExit() {
    CCNode::onExit();
    CCTouchDispatcher::get()->removeDelegate(this);
}

void ContextMenu::addItem(
    ZStringView name,
    Function<void()> callback,
    ZStringView description
) {
    auto label = Label::createRich(name, "bigFont.fnt");
    label->setScale(Impl::BUTTON_SCALE);

    Label* descLabel = nullptr;
    if (!description.empty()) {
        descLabel = Label::createRich(description, "chatFont.fnt");
        descLabel->setScale(Impl::DESCRIPTION_SCALE);
        descLabel->setColor({175, 175, 175});
    }

    m_impl->m_entries.emplace_back(Impl::Entry{
        label,
        descLabel,
        std::move(callback)
    });
}

void ContextMenu::addSeparator() {
    m_impl->m_entries.emplace_back(Impl::Entry{nullptr, nullptr, nullptr});
}

void ContextMenu::show(CCPoint pos) {
    if (m_impl->m_entries.empty()) {
        return;
    }

    m_impl->populateItems(this);
    this->updateLayout();

    if (auto scene = CCScene::get()) {
        this->closeAll();

        auto z = scene->getHighestChildZ();
        scene->addChild(this, z + 1);

        auto size = this->getScaledContentSize();
        auto winSize = CCDirector::get()->getWinSize();

        CCPoint anchor{0.f, 1.f};

        auto right = pos.x + size.width;
        if (right > winSize.width) {
            anchor.x = 1.f;
        }

        auto bottom = pos.y - size.height;
        if (bottom < 0) {
            auto fract = -bottom / size.height;
            anchor.y = 1.f - fract;
        }

        this->setAnchorPoint(anchor);
        this->setPosition(pos);
    }
}

void ContextMenu::closeAll() {
    if (auto scene = CCScene::get()) {
        while (auto menu = scene->getChildByType<ContextMenu>()) {
            menu->removeFromParent();
        }
    }
}

bool ContextMenu::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    auto local = this->convertToNodeSpace(touch->getLocation());
    CCRect rect{0.f, 0.f, this->getContentWidth(), this->getContentHeight()};
    if (rect.containsPoint(local)) {
        return true;
    }

    this->removeFromParent();
    return false;
}

bool ContextMenu::init() {
    if (!NineSlice::initWithFile("square02b_001.png", {0, 0, 80, 80}, {})) {
        return false;
    }

    this->setColor({0, 0, 0});
    this->setOpacity(220);
    this->setScaleMultiplier(0.25f);

    this->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisScaling(AxisScaling::Fit)
            ->setCrossAxisScaling(AxisScaling::Fit)
            ->setMainAxisAlignment(MainAxisAlignment::Center)
            ->setCrossAxisAlignment(CrossAxisAlignment::Start)
            ->setPadding(Padding::uniform(2.f))
    );

    return true;
}