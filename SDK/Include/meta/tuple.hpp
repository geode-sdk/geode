#ifndef GEODE_CORE_META_TUPLE_HPP
#define GEODE_CORE_META_TUPLE_HPP

#include "common.hpp"

#include <type_traits>
#include <utility>

namespace geode::core::meta {
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
        template <class T, size_t i>
        class Element {
        private:
            T value;

        protected:
            constexpr T at(std::integral_constant<size_t, i>&&) const {
                return this->value;
            }
        
        public:
            constexpr Element(T value) : value(value) {}
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
        struct elements_for_impl;

        template <size_t... indices, class... Classes>
        struct elements_for_impl<std::index_sequence<indices...>, Classes...> {
            using result = Elements<Element<Classes, indices>...>;
        };

    public:
        template <class... Classes>
        using elements_for = typename elements_for_impl<
                std::make_index_sequence<sizeof...(Classes)>,
                Classes...
            >::result;

        template <class... Classes>
        static auto make(Classes&&... values) {
            return Tuple<Classes...> { values... };
        }
    };

    template <class Current, class... Rest>
    class Tuple<Current, Rest...> : public Tuple<>::elements_for<Current, Rest...> {
    private:
        using MyElements = Tuple<>::elements_for<Current, Rest...>;

    public:
        template <size_t i>
        using type_at = decltype(std::declval<MyElements>().template at<i>());

    protected:
        // Haskell (false case)
        template <
            template <size_t, class, size_t> class Pred, 
            bool, size_t i, size_t counter, size_t... seq
        >
        class filter_impl {
        private:
            using NextPred = Pred<i + 1, type_at<i + 1>, counter>;
        
        public:
            using result = typename filter_impl<
                    Pred, NextPred::result, i + 1, counter, seq...
                >::result;
        };

        // Haskell (true case)
        template <
            template <size_t, class, size_t> class Pred, 
            size_t i, size_t counter, size_t... seq
        >
        class filter_impl<Pred, true, i, counter, seq...> {
        private:
            using MyPred = Pred<i, type_at<i>, counter>;
            using NextPred = Pred<i + 1, type_at<i + 1>, counter>;
        
        public:
            using result = typename filter_impl<
                    Pred, NextPred::result, i + 1, 
                    MyPred::counter, seq..., MyPred::index
                >::result;
        };

        // Haskell (final case, if true)
        template <
            template <size_t, class, size_t> class Pred, 
            size_t counter, size_t... seq
        >
        class filter_impl<Pred, false, sizeof...(Rest), counter, seq...> {
        public:
            using result = std::index_sequence<seq...>;
        };

        // Haskell (final case, if false)
        template <
            template <size_t, class, size_t> class Pred, 
            size_t counter, size_t... seq
        >
        class filter_impl<Pred, true, sizeof...(Rest), counter, seq...> {
        private:
            static constexpr size_t i = sizeof...(Rest);
            using MyPred = Pred<i, type_at<i>, counter>;
            
        public:
            using result = std::index_sequence<seq..., MyPred::index>;
        };
        
    public:
        template <template <size_t, class, size_t> class Pred>
        using filter = typename filter_impl<Pred, Pred<0, Current, 0>::result, 0, 0>::result;
    };
}

#endif /* GEODE_CORE_META_TUPLE_HPP */