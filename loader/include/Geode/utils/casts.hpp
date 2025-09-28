#pragma once

#include <inttypes.h>
#include <iostream>
#include <string>
#include <type_traits>

namespace geode::internal {

  template <class T, class = void>
  struct extract_modify_base { using type = void; };

  template <class T>
  struct extract_modify_base<T, std::void_t<decltype(T::m_fields)>> {
  private:
    template <class U>
    struct extract_base { using type = void; };

    template <class Derived, class Base>
    struct extract_base<FieldIntermediate<Derived, Base>> { using type = Base; };
  public:
    using type = typename extract_base<decltype(T::m_fields)>::type;
  };
  
  template <class T>
  using ModifyBase = typename extract_modify_base<T>::type;

}

namespace geode::cast {
    /**
     * Alias for static_cast
     */
    template <class T, class F>
    [[deprecated("This will be removed in v5, use static_cast instead")]]
    static constexpr T as(F const v) {
        return static_cast<T>(v);
    }

    /**
     * Cast from anything to anything else,
     * provided they are the same size
     */
    template <class T, class F>
    static constexpr T union_cast(F const v) {
        static_assert(sizeof(F) == sizeof(T), "union_cast: R and T don't match in size!");

        union {
            T t;
            F f;
        } x;

        x.f = v;
        return x.t;
    }

    /**
     * Reference casting. Does a pointer-to-pointer
     * cast but uses reference syntactic sugar to
     * look cleaner.
     */
    template <class T, class F>
    static constexpr T reference_cast(F v) {
        return reinterpret_cast<T&>(v);
    }

    /**
     * Cast based on RTTI. Casts an adjusted this pointer
     * to it's non offset form.
     */
    template <class T, class F>
    static constexpr T base_cast(F const obj) {
        return static_cast<T>(dynamic_cast<void*>(obj));
    }

    /**
     * Cast based on RTTI. This is used to check
     * if an object is exactly the class needed. Returns
     * nullptr on failure.
     */
    template <class T, class F>
    static T exact_cast(F const obj) {
        if (std::strcmp(typeid(*obj).name(), typeid(std::remove_pointer_t<T>).name()) == 0) {
            return base_cast<T>(obj);
        }
        return nullptr;
    }

    /**
     * A cast specialized to cast to modify classes. Static casts to the base class of the modify class first,
     * and then static casts to the modify class itself.
     * @example modify_cast<MyGJBaseGameLayer*>(PlayLayer::get());
     */
    template <class Target, class Original>
    constexpr Target modify_cast(Original original) {
    
      using TargetBase = geode::internal::ModifyBase<std::remove_pointer_t<Target>>;
    
      static_assert(std::is_pointer_v<Target> && !std::is_pointer_v<std::remove_pointer_t<Target>>, "Target class has to be a single pointer.");
      static_assert(std::is_pointer_v<Original> && !std::is_pointer_v<std::remove_pointer_t<Original>>, "Original class has to be a single pointer.");
      static_assert(
          (
              requires { std::remove_pointer_t<Target>::m_fields; !std::is_void_v<TargetBase>; } &&
              std::is_base_of_v<geode::Modify<std::remove_pointer_t<Target>, TargetBase>, std::remove_pointer_t<Target>>
          ),
          "The target class has to be a Modify class."
      );
      static_assert(
        !std::is_void_v<TargetBase> && requires { static_cast<TargetBase*>(original); },
        "The original class has to be castable to the class the modify class is modifying."
      );
    
      return static_cast<Target>(static_cast<TargetBase*>(original));
    }

}
