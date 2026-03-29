#include <Geode/utils/StringBuffer.hpp>
#include <Geode/loader/Formatter.hpp>
#include <ranges>

using namespace geode::prelude;

class geode::format::FormatterImpl {
private:
    std::vector<std::unique_ptr<FormatBase>> m_formatCallbacks;
public:
    static FormatterImpl* get() {
        static auto s_instance = new FormatterImpl();
        return s_instance;
    }

    void addFormat(std::unique_ptr<FormatBase> format) {
        m_formatCallbacks.push_back(std::move(format));
    }

    Result<std::string> handleFormat(CCObject const* obj, std::string_view specifier) {
        for (const auto& callback : std::views::reverse(m_formatCallbacks)) {
            auto res = callback->format(obj, specifier);
            if (res) return Ok(res.unwrap());
        }

        return Err("Format not found");
    }
};

void geode::format::registerFormatImpl(std::unique_ptr<FormatBase> format) {
    FormatterImpl::get()->addFormat(std::move(format));
}

geode::Result<std::string> geode::format::handleFormatImpl(cocos2d::CCObject const* obj, std::string_view specifier) {
    return FormatterImpl::get()->handleFormat(obj, specifier);
}

std::string geode::format_as(Mod* mod) {
    if (mod) {
        return fmt::format("{{ Mod, {} }}", mod->getName());
    }
    else {
        return "{ Mod, null }";
    }
}

std::string geode::format_as(CCObject const* obj) {
    if (obj) {
        return fmt::format("{{ {}, {} }}", getObjectName(obj), fmt::ptr(obj));
    } else {
        return "{ CCObject, null }";
    }
}

std::string geode::format_as(CCNode* obj) {
    if (obj) {
        return fmt::format(
            "{{ {}, {}, ({}) }}",
            getObjectName(obj),
            fmt::ptr(obj),
            obj->boundingBox()
        );
    } else {
        return "{ CCNode, null }";
    }
}

std::string geode::format_as(CCArray* arr) {
    if (!arr && !arr->count()) return "[empty]";

    StringBuffer<> buffer;

    buffer.append('[');

    bool first = true;

    for (auto obj : arr->asExt()) {
        if (!first) buffer.append(", ");
        
        first = false;
        buffer.append(format_as(obj));
    }

    buffer.append(']');

    return buffer.str();
}

$execute {
    format::registerFormat<CCNode>([] (CCNode* self) -> std::string {
        return fmt::format(
            "{}, ({})",
            fmt::ptr(self),
            self->boundingBox()
        );
    });

    format::registerFormat<CCFloat>([] (CCFloat* self) -> std::string {
        return numToString(self->getValue());
    });

    format::registerFormat<CCDouble>([] (CCDouble* self) -> std::string {
        return numToString(self->getValue());
    });

    format::registerFormat<CCInteger>([] (CCInteger* self) -> std::string {
        return numToString(self->getValue());
    });

    format::registerFormat<CCBool>([] (CCBool* self) -> std::string {
        return self->getValue() ? "true" : "false";
    });

    format::registerFormat<CCString>([] (CCString* self) -> std::string {
        return fmt::format("\"{}\"", self->getCString());
    });

    format::registerFormat<CCArray>([] (CCArray* self, std::string_view specifier) -> std::string {
        if (!self || !self->count()) return "[empty]";

        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;

        buffer.append('[');

        bool first = true;

        for (auto obj : self->asExt()) {
            if (!first) buffer.append(std::string_view(", "));
            if (!rawFormat) buffer.append(std::string_view("\n\t"));
            first = false;

            buffer.append("{}", format::wrap(obj));
        }

        if (!rawFormat) buffer.append('\n');

        buffer.append(']');
        return buffer.str();
    });

    format::registerFormat<CCSet>([] (CCSet* self, std::string_view specifier) -> std::string {
        if (!self || !self->count()) return "[empty]";

        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;
        buffer.append('[');

        bool first = true;

        for (CCSetIterator setIter = self->begin(); setIter != self->end(); ++setIter) {
            if (!first) buffer.append(std::string_view(", "));
            if (!rawFormat) buffer.append(std::string_view("\n\t"));
            first = false;

            buffer.append("{}", format::wrap(static_cast<CCObject*>(*setIter)));
        }

        if (!rawFormat) buffer.append('\n');
        buffer.append(']');

        return buffer.str();
    });

    format::registerFormat<CCDictionary>([] (CCDictionary* self, std::string_view specifier) -> std::string {
        if (!self || !self->count()) return "[empty]";
        
        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;
        buffer.append('[');

        auto appendAll = [&]<typename Key>() {
            bool first = true;

            for (const auto& [key, obj] : self->asExt<Key, CCObject>()) {
                if (!first) buffer.append(std::string_view(", "));
                if (!rawFormat) buffer.append(std::string_view("\n\t"));
                first = false;
                
                if (self->m_eDictType == CCDictionary::CCDictType::kCCDictStr) {
                    buffer.append("{{ \"{}\": {} }}", key, format::wrap(obj));
                }
                else {
                    buffer.append("{{ {}: {} }}", key, format::wrap(obj));
                }
            }
        };

        if (self->m_eDictType == CCDictionary::CCDictType::kCCDictInt) {
            appendAll.operator()<int>();
        } 
        else {
            appendAll.operator()<std::string_view>();
        }

        if (!rawFormat) buffer.append('\n');
        buffer.append(']');

        return buffer.str();
    });

    format::registerFormat<CCTouch>([] (CCTouch* self) -> std::string {
        return fmt::format(
            "{}, ({})",
            fmt::ptr(self),
            self->getLocation()
        );
    });

    format::registerFormat<CCFiniteTimeAction>([] (CCFiniteTimeAction* self) -> std::string {
        return fmt::format(
            "{}, d: {}",
            fmt::ptr(self),
            self->getDuration()
        );
    });

    format::registerFormat<CCActionInterval>([] (CCActionInterval* self) -> std::string {
        return fmt::format(
            "{}, d: {}, e: {}",
            fmt::ptr(self),
            self->getDuration(),
            self->getElapsed()
        );
    });
}
