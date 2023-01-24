#pragma once
#include <type_traits>

namespace geode::modifier {
    template <class FunctionType>
    struct MemberFunc {
        template <class Class>
        using with = FunctionType Class::*;
    };

    template <class FunctionType>
    struct ConstMemberFunc {
        template <class Class>
        using with = FunctionType Class::*;
    };

    // why
    template <class Return, class... Parameters>
    struct ConstMemberFunc<Return(Parameters...)> {
        using FunctionType = Return(Parameters...) const;
        template <class Class>
        using with = FunctionType Class::*;
    };

    template <class FunctionType>
    struct StaticFunc {
        using type = FunctionType*;
    };

    template <class...>
    static constexpr bool alwaysFalse = false;

    /**
     * The ~unevaluated~ function that gets the appropriate
     * version of a function type from its return, parameters, and classes.
     *
     * nvm its no more unevaluated
     */
    template <class FunctionType, class Class>
    constexpr auto substitute(typename MemberFunc<FunctionType>::template with<Class> function) {
        return function;
    }

    template <class FunctionType, class Class>
    constexpr auto substitute(typename ConstMemberFunc<FunctionType>::template with<Class> function
    ) {
        return function;
    }

    template <class FunctionType>
    constexpr auto substitute(typename StaticFunc<FunctionType>::type function) {
        return function;
    }

    /**
     * An UUID system that generates an unique comparable
     * value for every instance. Internally used for comparing member
     * function pointers.
     */
    template <auto a>
    struct FunctionUUID {
    private:
        constexpr static void function() {}

    public:
        constexpr static inline void (*value)() = &FunctionUUID::function;
    };

    /**
     * A type trait that removes the class from a member function pointer.
     */
    template <class Func>
    struct RemoveClass {
        using type = Func;
    };

    template <class Return, class Class, class... Params>
    struct RemoveClass<Return(Class::*)(Params...)> {
        using type = Return(Params...);
    };

    template <class Return, class Class, class... Params>
    struct RemoveClass<Return(Class::*)(Params...) const> {
        using type = Return(Params...);
    };

    template <class Func>
    using RemoveClassType = typename RemoveClass<Func>::type;

    /**
     * A helper struct that allows for checking if two function pointers 
     * are the same or different.
     */
    struct Unique {
        using ValueType = void(*)(...);
        static constexpr auto nvalue = static_cast<void(*)(...)>(nullptr);
        
        template <auto Value>
        struct Impl {
            static void unique(...) {};

            static constexpr auto value = &unique;
        };

        template <>
        struct Impl<nvalue> {
            static constexpr auto value = nvalue;
        };

        template <auto Value>
        static constexpr auto value = Impl<Value>::value;

        
        /**
         * Checks if two function pointers are the same. If their types are 
         * different, returns false.
         */
        template <auto p1, auto p2>
        static constexpr auto same() {
            if (!std::is_same_v<RemoveClassType<decltype(p1)>, RemoveClassType<decltype(p2)>>) return false;
            auto v1 = value<p1>;
            auto v2 = value<p2>;
            if (v1 == nvalue) return false;
            if (v2 == nvalue) return false;
            return v1 == v2;
        }

        /**
         * Checks if two function pointers are different. If their types are 
         * different, returns false.
         */
        template <auto p1, auto p2>
        static constexpr auto different() {
            if (!std::is_same_v<RemoveClassType<decltype(p1)>, RemoveClassType<decltype(p2)>>) return false;
            auto v1 = value<p1>;
            auto v2 = value<p2>;
            if (v1 == nvalue) return false;
            if (v2 == nvalue) return false;
            return v1 != v2;
        }
    };

    /**
     * Helps resolving an overloaded function pointer to a specific function using 
     * its parameter types as the hint.
     */
    template <class... Params>
    struct Resolve {
        template <class Return>
        static constexpr auto func(Return(*ptr)(std::type_identity_t<Params>...)) {
            return ptr;
        }

        template <class Return, class Class>
        static constexpr auto func(Return(Class::*ptr)(std::type_identity_t<Params>...)) {
            return ptr;
        }

        template <class Return, class Class>
        static constexpr auto func(Return(Class::*ptr)(std::type_identity_t<Params>...) const) {
            return ptr;
        }

        static constexpr auto func(...) {
            return Unique::nvalue;
        }
    };

    /**
     * A specialization for giving the variadic types as a single type with the 
     * function type. The return type is ignored.
     */
    template <class... Params>
    struct Resolve<void(Params...)> : Resolve<Params...> {
        using Resolve<Params...>::func;
    };

    /**
     * A type trait that checks if a class has a function called "constructor".
     */
    template <class Class>
    concept HasConstructor = requires {
        &Class::constructor;
    };

    /**
     * A type trait that checks if a class has a function called "destructor".
     */
    template <class Class>
    concept HasDestructor = requires {
        &Class::destructor;
    };

    template <class FunctionType>
    struct AsStaticType {
        using type = FunctionType;
    };

    template <class Return, class Class, class... Params>
    struct AsStaticType<Return(Class::*)(Params...)> {
        using type = Return(*)(Class*, Params...);
    };

    template <class Return, class Class, class... Params>
    struct AsStaticType<Return(Class::*)(Params...) const> {
        using type = Return(*)(Class const*, Params...);
    };
}
