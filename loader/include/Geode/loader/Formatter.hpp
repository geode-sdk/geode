#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/utils/StringBuffer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/Result.hpp>
#include <arc/future/PollableMetadata.hpp>
#include <ccTypes.h>
#include <string_view>
#include <type_traits>
#include <fmt/core.h>
// for formatting std::vector and such
#include <fmt/ranges.h>
// for std::optional
#include <fmt/std.h>

namespace geode {
    // these are here because theyre special :-)

    // these three have been made obsolete, but removing them would technically be a breaking change
    GEODE_DLL std::string format_as(cocos2d::CCObject const*);
    GEODE_DLL std::string format_as(cocos2d::CCArray*);
    GEODE_DLL std::string format_as(cocos2d::CCNode*);

    class Mod;
    GEODE_DLL std::string format_as(Mod*);
}

template <typename T, typename E>
struct fmt::formatter<geode::Result<T, E>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(geode::Result<T, E> const& result, FormatContext& ctx) const noexcept {
        std::string out;

        auto formatValue = [&](auto&& value) {
            using V = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<V, std::string>) {
                fmt::format_to(std::back_inserter(out), "\"{}\"", value);
            } else if constexpr (std::is_void_v<V>) {
                // nothing
            } else if constexpr (requires { geode::format_as(value); }) {
                fmt::format_to(std::back_inserter(out), "{}", geode::format_as(value));
            } else if constexpr (std::is_pointer_v<V>) {
                fmt::format_to(std::back_inserter(out), "{}", (void*)value);
            } else {
                fmt::format_to(std::back_inserter(out), "{}", value);
            }
        };

        if (result) {
            std::string_view quotes = std::is_same_v<T, std::string> ? "\"" : "";
            if constexpr (!std::is_void_v<T>) {
                return fmt::format_to(ctx.out(), "Ok({}{}{})", quotes, result.unwrap(), quotes);
            }
            else {
                return fmt::format_to(ctx.out(), "Ok()");
            }
        } else {
            std::string_view quotes = std::is_same_v<E, std::string> ? "\"" : "";
            if constexpr (!std::is_void_v<E>) {
                return fmt::format_to(ctx.out(), "Err({}{}{})", quotes, result.unwrapErr(), quotes);
            }
            else {
                return fmt::format_to(ctx.out(), "Err()");
            }
        }
    }
};

template <>
struct fmt::formatter<cocos2d::ccColor3B> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor3B const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgb({}, {}, {})", col.r, col.g, col.b);
    }
};

template <>
struct fmt::formatter<cocos2d::ccColor4B> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor4B const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
    }
};

template <>
struct fmt::formatter<cocos2d::ccColor4F> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::ccColor4F const& col, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "rgba({}, {}, {}, {})", col.r, col.g, col.b, col.a);
    }
};

template <>
struct fmt::formatter<cocos2d::CCPoint> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCPoint const& pt, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{}, {}", pt.x, pt.y);
    }
};

template <>
struct fmt::formatter<cocos2d::CCSize> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCSize const& sz, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{} : {}", sz.width, sz.height);
    }
};

template <>
struct fmt::formatter<cocos2d::CCRect> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { return ctx.begin(); }

    template <typename FormatContext>
    auto format(cocos2d::CCRect const& rect, FormatContext& ctx) const noexcept {
        return fmt::format_to(ctx.out(), "{} | {}", rect.origin, rect.size);
    }
};

namespace geode::format {
    struct Wrapper {
        Wrapper(cocos2d::CCObject* obj) : m_obj(obj) {}
        cocos2d::CCObject* m_obj;
    };

    class FormatBase {
        virtual geode::Result<std::string> format(cocos2d::CCObject const* obj, std::string_view specifier) = 0;
        friend class FormatterImpl;
    };

    template<class T>
    class FormatType : public FormatBase {
    public:
        using FormatCallback = geode::Function<std::string(T*, std::string_view)>;

        ~FormatType() = default;
        FormatType(FormatCallback callback) : m_formatter(std::move(callback)) {}
    private:
        FormatCallback m_formatter;

        geode::Result<std::string> format(cocos2d::CCObject const* obj, std::string_view specifier) override {
            if (!geode::cast::typeinfo_cast<T*>(obj)) return geode::Err("Incorrect Type");
            if (!m_formatter) return geode::Err("No formatter callback");
            if (m_formatter) return geode::Ok(m_formatter(static_cast<T*>(const_cast<cocos2d::CCObject*>(obj)), specifier));
        }

        friend class FormatterImpl;
    };

    GEODE_DLL void registerFormatImpl(std::string_view name, std::unique_ptr<FormatBase> format);
    GEODE_DLL geode::Result<std::string> handleFormatImpl(cocos2d::CCObject const* obj, std::string_view specifier);

    template<class T, typename F>
    inline void registerFormat(F&& callback) {
        constexpr auto name = arc::getTypename<T>();

        using Callback = typename FormatType<T>::FormatCallback;

        if constexpr (std::is_invocable_r_v<std::string, F, T*, std::string_view>) {
            registerFormatImpl(std::string_view{name.first, name.second}, std::make_unique<FormatType<T>>(std::forward<F>(callback)));
        }
        else if constexpr (std::is_invocable_r_v<std::string, F, T*>) {
            registerFormatImpl(std::string_view{name.first, name.second}, std::make_unique<FormatType<T>>(Callback(
                [f = std::forward<F>(callback)](T* obj, std::string_view) {
                    return f(obj);
                }
            )));
        }
        else {
            static_assert(
                false, 
                "Format callback must be invocable as std::string(T*) or std::string(T*, std::string_view)"
            );
        }
    }
    
    template <class T>
    concept CocosPtr =     
        std::is_pointer_v<std::decay_t<T>> &&
        std::is_base_of_v<
            cocos2d::CCObject,
            std::remove_pointer_t<std::decay_t<T>>
    >;

    // allow wrapping CCObjects so they can be caught by the formatter
    template <class T>
    decltype(auto) wrap(T&& value) {
        if constexpr (CocosPtr<T>) {
            return Wrapper{ value };
        } else {
            return std::forward<T>(value);
        }
    }

    template <class T>
    using TransformType = std::conditional_t<
        CocosPtr<T>,
        decltype(wrap<T>(std::declval<T>())),
        T
    >;

    template <class... Args>
    using FmtStr = fmt::format_string<TransformType<Args>...>;
}

template <>
struct fmt::formatter<geode::format::Wrapper> {
    std::string specifier;

    template <typename ParseContext>
    constexpr auto parse(ParseContext& ctx) noexcept { 
        auto begin = ctx.begin();
        auto end = ctx.end();

        auto it = begin;
        while (it != end && *it != '}') ++it;

        specifier = std::string_view(begin, it - begin);
        return it;
    }

    template <typename FormatContext>
    auto format(geode::format::Wrapper const& wrapper, FormatContext& ctx) const noexcept {
        if (!wrapper.m_obj) return fmt::format_to(ctx.out(), "{}", "{ CCObject, null }");

        auto result = geode::format::handleFormatImpl(wrapper.m_obj, specifier);
        if (result) return fmt::format_to(ctx.out(), "{{ {} ({}), {} }}", geode::cocos::getObjectName(wrapper.m_obj), wrapper.m_obj->getTag(), result.unwrap());

        return fmt::format_to(ctx.out(), "{{ {} ({}), {} }}", geode::cocos::getObjectName(wrapper.m_obj), wrapper.m_obj->getTag(), fmt::ptr(wrapper.m_obj));
    }
};
