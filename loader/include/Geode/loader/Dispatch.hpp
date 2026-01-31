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
}

// - Macros for exporting functions via events -

// You can use these to easily export functions to other mods
// without being a required depedency.
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
// Important: The function must be declared inline, and return a geode::Result,
// as it can fail if the api is not available.
inline geode::Result<int> addNumbers(int a, int b) GEODE_EVENT_EXPORT(&addNumbers, (a, b));
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

namespace geode::geode_internal {
    template <class Fn>
    inline auto callEventExportListener(Fn fnPtr, auto eventID) {
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
}

#define GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, eventID)                                       \
    {                                                                                           \
        static auto storage = geode::geode_internal::callEventExportListener(fnPtr, eventID);   \
        if (!storage) return geode::Err("Unable to call method");                               \
        return storage callArgs;                                                                \
    }

#define GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, eventID)                                 \
    {                                                                                           \
        static auto storage = geode::geode_internal::callEventExportListener(fnPtr, eventID);   \
        if (!storage) return geode::utils::function::Return<decltype(fnPtr)>();                \
        return storage callArgs;                                                                \
    }


#define GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, eventID)                                             \
    ;                                                                                                   \
    template <auto>                                                                                     \
    struct EventExportDefine;                                                                           \
    template <>                                                                                         \
    struct EventExportDefine<geode::modifier::FunctionUUID<fnPtr>::value> {                             \
        static inline bool val = geode::geode_internal::getEventExportListener(fnPtr, eventID);         \
        static inline auto nonOmitted = &val;                                                           \
    };

#ifndef GEODE_DEFINE_EVENT_EXPORTS

    #define GEODE_EVENT_EXPORT(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs))

    #define GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_CALL(fnPtr, callArgs, eventID)

    #define GEODE_EVENT_EXPORT_NORES(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs))

    #define GEODE_EVENT_EXPORT_ID_NORES(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_CALL_NORES(fnPtr, callArgs, eventID)
#else

    #define GEODE_EVENT_EXPORT(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, GEODE_EVENT_EXPORT_ID_FOR(#fnPtr, #callArgs))

    #define GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_DEFINE(fnPtr, callArgs, eventID)

    #define GEODE_EVENT_EXPORT_NORES(fnPtr, callArgs) \
        GEODE_EVENT_EXPORT(fnPtr, callArgs)

    #define GEODE_EVENT_EXPORT_ID_NORES(fnPtr, callArgs, eventID) \
        GEODE_EVENT_EXPORT_ID(fnPtr, callArgs, eventID)
#endif