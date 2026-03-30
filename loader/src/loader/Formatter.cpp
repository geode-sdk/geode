#include <Geode/utils/StringBuffer.hpp>
#include <Geode/loader/Formatter.hpp>
#include <Geode/loader/Mod.hpp>
#include <ranges>

using namespace geode::prelude;

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

$on_mod(Loaded) {
    format::LogFormatEvent<CCNode>().listen(CCNode::create(), [] (std::string& output, CCNode* obj) {
        output = fmt::format(
            "{}, ({})",
            fmt::ptr(obj),
            obj->boundingBox()
        );
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCFloat>().listen(CCFloat::create(0.f), [] (std::string& output, CCFloat* obj) {
        output = numToString(obj->getValue());
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCDouble>().listen(CCDouble::create(0.0), [] (std::string& output, CCDouble* obj) {
        output = numToString(obj->getValue());
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCInteger>().listen(CCInteger::create(0), [] (std::string& output, CCInteger* obj) {
        output = numToString(obj->getValue());
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCBool>().listen(CCBool::create(false), [] (std::string& output, CCBool* obj) {
        output = obj->getValue() ? "true" : "false";
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCString>().listen(CCString::create(""), [] (std::string& output, CCString* obj) {
        output = fmt::format("\"{}\"", obj->getCString());
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCArray>().listen(CCArray::create(), [] (std::string& output, CCArray* obj, std::string_view specifier) {
        if (!obj || !obj->count()) {
            output = "[empty]";
            return ListenerResult::Stop;
        }

        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;

        buffer.append('[');

        bool first = true;

        for (auto obj : obj->asExt()) {
            if (!first) buffer.append(std::string_view(", "));
            if (!rawFormat) buffer.append(std::string_view("\n\t"));
            first = false;

            buffer.append("{}", format::wrap(obj));
        }

        if (!rawFormat) buffer.append('\n');

        buffer.append(']');
        output = buffer.str();
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCSet>().listen(CCSet::create(), [] (std::string& output, CCSet* obj, std::string_view specifier) {
        if (!obj || !obj->count()) {
            output = "[empty]";
            return ListenerResult::Stop;
        }

        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;

        buffer.append('[');

        bool first = true;

        for (CCSetIterator setIter = obj->begin(); setIter != obj->end(); ++setIter) {
            if (!first) buffer.append(std::string_view(", "));
            if (!rawFormat) buffer.append(std::string_view("\n\t"));
            first = false;

            buffer.append("{}", format::wrap(static_cast<CCObject*>(*setIter)));
        }

        if (!rawFormat) buffer.append('\n');

        buffer.append(']');
        output = buffer.str();
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCDictionary>().listen(CCDictionary::create(), [] (std::string& output, CCDictionary* obj, std::string_view specifier) {
        if (!obj || !obj->count()) {
            output = "[empty]";
            return ListenerResult::Stop;
        }

        bool rawFormat = specifier == "raw";

        StringBuffer<> buffer;

        buffer.append('[');

        auto appendAll = [&]<typename Key>() {
            bool first = true;

            for (const auto& [key, child] : obj->asExt<Key, CCObject>()) {
                if (!first) buffer.append(std::string_view(", "));
                if (!rawFormat) buffer.append(std::string_view("\n\t"));
                first = false;

                if (obj->m_eDictType == CCDictionary::CCDictType::kCCDictStr) {
                    buffer.append("{{ \"{}\": {} }}", key, format::wrap(child));
                }
                else {
                    buffer.append("{{ {}: {} }}", key, format::wrap(child));
                }
            }
        };

        if (obj->m_eDictType == CCDictionary::CCDictType::kCCDictInt) {
            appendAll.operator()<int>();
        }
        else {
            appendAll.operator()<std::string_view>();
        }

        if (!rawFormat) buffer.append('\n');

        buffer.append(']');
        output = buffer.str();
        return ListenerResult::Stop;
    }).leak();

    auto touch = new CCTouch();
    format::LogFormatEvent<CCTouch>().listen(touch, [] (std::string& output, CCTouch* obj, std::string_view specifier) {
        if (specifier == "full") {
            output = fmt::format(
                "{}, ({}) ... ({}) -> ({})",
                fmt::ptr(obj),
                obj->getStartLocation(),
                obj->getPreviousLocation(),
                obj->getLocation()
            );
        }
        else {
            output = fmt::format(
                "{}, ({})",
                fmt::ptr(obj),
                obj->getLocation()
            );
        }
        return ListenerResult::Stop;
    }).leak();
    delete touch;

    format::LogFormatEvent<CCFiniteTimeAction>().listen(CCFiniteTimeAction::create(), [] (std::string& output, CCFiniteTimeAction* obj) {
        output = fmt::format(
            "{}, d: {}",
            fmt::ptr(obj),
            obj->getDuration()
        );
        return ListenerResult::Stop;
    }).leak();

    format::LogFormatEvent<CCActionInterval>().listen(CCActionInterval::create(0.0), [] (std::string& output, CCActionInterval* obj) {
        output = fmt::format(
            "{}, d: {}, e: {}",
            fmt::ptr(obj),
            obj->getDuration(),
            obj->getElapsed()
        );
        return ListenerResult::Stop;
    }).leak();
}
