// note: intentionally missing pragma once, so the macros can be redefined
#include "Dispatch.hpp"

// - Macros for exporting functions via events -

// You can use these to easily export functions to other mods
// without being a required dependency.
// # Example Usage:
/*
```
// (In your api distributed header file)
#pragma once

#include <Geode/loader/EventExport.hpp>
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

#undef GEODE_EVENT_EXPORT
#undef GEODE_EVENT_EXPORT_ID
#undef GEODE_EVENT_EXPORT_MEMBER
#undef GEODE_EVENT_EXPORT_MEMBER_ID
#undef GEODE_EVENT_EXPORT_MEMBER_STATIC
#undef GEODE_EVENT_EXPORT_MEMBER_STATIC_ID

#ifndef GEODE_DEFINE_EVENT_EXPORTS
    #define GEODE_EVENT_EXPORT(ret, name, params, args) \
        GEODE_EVENT_EXPORT_CALL(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_CALL(ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER(Class, ret, name, params, args) \
        GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_ID(Class, ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER_STATIC(ret, name, params, args) \
        GEODE_EVENT_EXPORT_CALL_MEMBER_STATIC(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_STATIC_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_CALL_MEMBER_STATIC(ret, name, params, args, (eventID))
#else
    #define GEODE_EVENT_EXPORT(ret, name, params, args) \
        GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER(Class, ret, name, params, args) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_ID(Class, ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, (eventID))

    #define GEODE_EVENT_EXPORT_MEMBER_STATIC(ret, name, params, args) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER_STATIC(ret, name, params, args, GEODE_EVENT_EXPORT_ID_FOR(#name, #args))

    #define GEODE_EVENT_EXPORT_MEMBER_STATIC_ID(ret, name, params, args, eventID) \
        GEODE_EVENT_EXPORT_DEFINE_MEMBER_STATIC(ret, name, params, args, (eventID))
#endif
