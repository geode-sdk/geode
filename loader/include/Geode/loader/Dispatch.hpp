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

// Below are helpers for event export, event export macros themselves are in EventExport.hpp

// once this is set in stone we should not change it ever
#define GEODE_EVENT_EXPORT_ID_FOR(fnPtrStr, callArgsStr) \
    (std::string(MY_MOD_ID "/") + (fnPtrStr[0] == '&' ? &fnPtrStr[1] : fnPtrStr))

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

    template <class Class, class Fn>
    struct MemFnPtr;

    template <class Class, class R, class... Args>
    struct MemFnPtr<Class, R (*)(Args...)> {
        using type = R (Class::*)(Args...);
    };

    template <class Class, class Fn>
    using MemFnPtrT = typename MemFnPtr<Class, Fn>::type;
}

// Workaround for exported member functions on Windows, note that this only supports clang and there is no MSVC equivalent
//
// Other platforms allow there to be an inline + non-inline member function in the class and they just pick the non-inline symbol,
// but windows does not like this and causes a duplicate symbol error. Forcing the inline def to be internal linkage causes it to work.
#ifdef __clang__
# define GEODE_INTERNAL_LINKAGE __attribute__((internal_linkage))
#else
# define GEODE_INTERNAL_LINKAGE
#endif

/// A hacky way to turn return type & parameters of a function into a function pointer type.
/// Unlike a simple `ret (*)(params)`, this allows specifying default arguments because it uses a function declaration
/// The `noexcept(false)` here is to avoid the Clang warning "Empty parantheses interpreted as a function declaration"
#define GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, uniq_name) \
    decltype([] { \
        ret uniq_name params noexcept(false); \
        return &uniq_name; \
    }())

#define GEODE_EVENT_EXPORT_GET_MEMBERFNTYPE(Class, ret, params, uniq_name) \
    ::geode::geode_internal::MemFnPtrT<Class, GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, uniq_name)>

#define GEODE_EVENT_EXPORT_UNIQ_NAME(sfx) \
    GEODE_CONCAT(GEODE_CONCAT(GEODE_CONCAT(_priv_call_fn_type, sfx), __COUNTER__), __LINE__)

#define GEODE_EVENT_EXPORT_CALL(ret, name, params, args, eventID)                                       \
    static ret (name) params {                                                                          \
        using FnType = GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name)); \
        static auto storage = geode::geode_internal::callEventExportListener<FnType>(eventID);          \
        if (!storage) return geode::geode_internal::getValueOnFailed<ret>();                            \
        return storage args;                                                                            \
    }

#define GEODE_EVENT_EXPORT_CALL_MEMBER(Class, ret, name, params, args, eventID)                                \
    GEODE_INTERNAL_LINKAGE inline ret (name) params {                                                          \
        using FnType = GEODE_EVENT_EXPORT_GET_MEMBERFNTYPE(Class, ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name)); \
        static auto storage = geode::geode_internal::callEventExportListener<FnType>(eventID);                  \
        if (!storage) return geode::geode_internal::getValueOnFailed<ret>();                                   \
        return storage args;                                                                                   \
    }

#define GEODE_EVENT_EXPORT_DEFINE(ret, name, params, args, eventID)                                \
    ret name params;                                                                               \
    static bool GEODE_EVENT_EXPORT_UNIQ_NAME(_geode_event_export_reg_)                   \
    = geode::geode_internal::getEventExportListener(                                               \
        static_cast<GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name))>(&name), \
        eventID                                                                                    \
    );

#define GEODE_EVENT_EXPORT_DEFINE_MEMBER(Class, ret, name, params, args, eventID)                               \
    ret name params;                                                                                            \
    static inline bool GEODE_EVENT_EXPORT_UNIQ_NAME(_geode_event_export_reg_)                         \
    = geode::geode_internal::getEventExportListener(static_cast<                                                \
        GEODE_EVENT_EXPORT_GET_MEMBERFNTYPE(Class, ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name)) \
    >(&Class::name), eventID);

#define GEODE_EVENT_EXPORT_DEFINE_MEMBER_STATIC(ret, name, params, args, eventID) \
    static ret name params;                                                                             \
    static inline bool GEODE_EVENT_EXPORT_UNIQ_NAME(_geode_event_export_reg_)                 \
    = geode::geode_internal::getEventExportListener(                                                    \
        static_cast<GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name))>(&name), \
        eventID \
    );

#define GEODE_EVENT_EXPORT_CALL_MEMBER_STATIC(ret, name, params, args, eventID) \
    GEODE_INTERNAL_LINKAGE static ret (name) params { \
        using FnType = GEODE_EVENT_EXPORT_GET_FNTYPE(ret, params, GEODE_EVENT_EXPORT_UNIQ_NAME(name)); \
        static auto storage = geode::geode_internal::callEventExportListener<FnType>(eventID);            \
        if (!storage) return geode::geode_internal::getValueOnFailed<ret>(); \
        return storage args; \
    }
