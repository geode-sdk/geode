#ifndef LILAC_CORE_META_TUPLE_HPP
#define LILAC_CORE_META_TUPLE_HPP

#include "common.hpp"

#include <type_traits>
#include <utility>

namespace lilac::meta {
    /* The Tuple class presents a nicer way to interact with parameter packs.
     * While this container is technically usable in other code, this is mostly
     * used for static reflection in function wrapping. Other applications will
     * usually be better covered by std::tuple.
     */
    template <class...>
    class Tuple;

    template <>
    class Tuple<> {
    private:
        template <class Type, size_t i>
        class Element {
        private:
            Type value;

        protected:
            constexpr Type at(std::integral_constant<size_t, i>&&) const {
                return this->value;
            }

        public:
            constexpr Element(Type value) : value(value) {}
        };

        template <class... Parents>
        class Elements : public Parents... {
        private:
            using Parents::at...;

        public:
            static constexpr size_t size = sizeof...(Parents);

            template <size_t i>
            constexpr decltype(auto) at() const {
                static_assert(i < size, "Out of range access!");
                return this->at(std::integral_constant<size_t, i>());
            }
        };

        template <class, class...>
        class elements_for_impl;

        template <size_t... indices, class... Classes>
        class elements_for_impl<std::index_sequence<indices...>, Classes...> {
        public:
            using result = Elements<Element<Classes, indices>...>;
        };

    public:
        template <class... Classes>
        using elements_for = typename elements_for_impl<
            std::make_index_sequence<sizeof...(Classes)>, Classes...>::result;
    };

    template <class... Classes>
    class Tuple : public Tuple<>::elements_for<Classes...> {
    private:
        using MyElements = Tuple<>::elements_for<Classes...>;

    public:
        template <size_t i>
        using type_at = decltype(std::declval<MyElements>().template at<i>());

    private:
        template <size_t i, bool>
        class type_at_wrap_impl {
        public:
            using result = void;
        };

        template <size_t i>
        class type_at_wrap_impl<i, true> {
        public:
            using result = type_at<i>;
        };

    public:
        // MSVC literally shows internal compiler structures if I don't wrap this sometimes.
        template <size_t i>
        using type_at_wrap = typename type_at_wrap_impl<i, (i < sizeof...(Classes))>::result;
    };

    // Deduction guide!
    template <class... Classes>
    Tuple(Classes...) -> Tuple<Classes...>;
}

#endif /* LILAC_CORE_META_TUPLE_HPP */