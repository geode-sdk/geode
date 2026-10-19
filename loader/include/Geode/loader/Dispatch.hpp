#pragma once

#include "Event.hpp"
#include "../utils/function.hpp"
#include "../modify/Traits.hpp"
#include <Geode/utils/function.hpp>
#include <Geode/utils/StringMap.hpp>

#include <functional>
#include <string>
#include <tuple>

namespace geode {
    template <class... Args>
    class Dispatch : public ThreadSafeEvent<Dispatch<Args...>, bool(Args...), std::string> {
    public:
        using ThreadSafeEvent<Dispatch<Args...>, bool(Args...), std::string>::ThreadSafeEvent;
    };

    template<class... Args>
    using DispatchEvent = Dispatch<Args...>;

}

// - Macros for exporting functions via events -

// You can use these to easily export functions to other mods
// without being a required dependency.
// # Example Usage:
/*
```
// (In your api distributed header file)
#pragma once

#include <Geode/loader/Dispatch.hpp>
// You must **manually** declare the mod id, as macros like GEODE_MOD_ID will not
// behave correctly to other mods using your api.
#define MY_MOD_ID "dev.my-api"

namespace api {
// Important: the function must return a Result, as it can fail if the api is not available.
GEODE_EVENT_EXPORT(geode::Result<int>, addNumbers, (int a, int b), (a, b));

// Overloads work too:
GEODE_EVENT_EXPORT(geode::Result<int>, addNumbers, (int a, int b, int c), (a, b, c));
}
```
*/
// Then, in **one** of your source files, you must define the exported functions:
/*
```
// MUST be defined before including the header.
#define GEODE_DEFINE_EVENT_EXPORTS
#include "../include/api.hpp"

Result<int> api::addNumbers(int a, int b) {
    return Ok(a + b);
}
```
*/

// once this is set in stone we should not change it ever
#define GEODE_EVENT_EXPORT_ID_FOR(fnPtrStr, callArgsStr) \
    (std::string(MY_MOD_ID "/") + (fnPtrStr[0] == '&' ? &fnPtrStr[1] : fnPtrStr))
#define GEODE_EVENT_EXPORT_UNIQUE(prefix, counter) GEODE_CONCAT(prefix, counter)

namespace geode::geode_internal {
    template <class Fn>
    inline auto callEventExportListener(auto eventID) {
        using StaticType = geode::modifier::AsStaticType<Fn>::type;
        Fn ptr = nullptr;
        geode::Dispatch<Fn*>(std::move(eventID)).send(&ptr);
        return geode::Function<std::remove_pointer_t<StaticType>>(ptr);
    }

    template <class Fn>
    inline bool getEventExportListener(Fn fnPtr, auto eventID) {
        geode::Dispatch<Fn*>(std::move(eventID)).listen([=](Fn* ptr) {
            *ptr = fnPtr;
            return geode::ListenerResult::Stop;
        }).leak();
        return true;
    }

    template <typename R>
    inline auto getValueOnFailed() {
        if constexpr (IsResult<R>) {
            return [] -> R {
                return Err("Unable to call method");
            }();
        } else if constexpr (!std::is_void_v<R>) {
            return R{};
        }
    }
}

#define GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, uniq_name) \
    decltype([] { \
        ret uniq_name params; \
        return &uniq_name; \
    }())

#define GEODE_EVENT_EXPORT_CALL(ret, name, params, args, eventID)                                       \
    static ret (name) params {                                                                          \
        using FnType = GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_CONCAT(GEODE_CONCAT(_priv_call_fn_type, name), __COUNTER__)); \
        static auto storage = geode::geode_internal::callEventExportListener<FnType>(eventID);          \
        if (!storage) return geode::geode_internal::getValueOnFailed<ret>();                            \
        return storage args;                                                                            \
    }

#define GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, eventID)                                \
    inline ret (name) params {                                                                                 \
        static auto storage = geode::geode_internal::callEventExportListener<ret (Class::*) params>(eventID);  \
        if (!storage) return geode::geode_internal::getValueOnFailed<ret>();                                   \
        return storage args;                                                                                   \
    }

#define GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, eventID)                                \
    ret name params;                                                                               \
    static bool GEODE_EVENT_EXPORT_UNIQUE(_geode_event_export_reg_, __COUNTER__)                   \
    = geode::geode_internal::getEventExportListener(                                               \
        static_cast<GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_CONCAT(GEODE_CONCAT(_priv_call_fn_type, name), __COUNTER__))>(&name), \
        eventID                                                                                    \
    );

#define GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, eventID)                               \
    ret name params;                                                                                            \
    static inline bool GEODE_EVENT_EXPORT_UNIQUE(_geode_event_export_reg_, __COUNTER__)                         \
    = geode::geode_internal::getEventExportListener(static_cast<ret (Class::*) params>(&Class::name), eventID);

#ifndef GEODE_DEFINE_EVENT_EXPORTS
    #define GEODE_EVENT_EXPORT(ret, name, params, args) \
        GEODE_EVENT_EXPORT_CALL(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_CALL(ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER(Class, ret, name, params, args) \
        GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_ID(Class, ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, (eventID))
#else
    #define GEODE_EVENT_EXPORT(ret, name, params, args) \
        GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER(Class, ret, name, params, args) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_ID(Class, ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, (eventID))
#endif
