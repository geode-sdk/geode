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
    template <class... Classes>
    class Tuple;

    template <>
    class Tuple<> {
    protected:
        // Haskell
        template <
            template <size_t, class, size_t> class,
            size_t, size_t, size_t... seq
        >
        class filter_impl {
        public:
            using result = std::index_sequence<seq...>;
        };

    public:
        static constexpr size_t size = 0;

        template <size_t>
        using type_at = void;

        template <size_t>
        constexpr decltype(auto) at() const {
            return 0;
        }

    public:
        template <class... Classes>
        static auto make(Classes&&... values) {
            return Tuple<Classes...> { values... };
        }

        template <class T>
        auto append(T&& value) const {
            return Tuple<T>(value);
        }
    };

    template <class Current, class... Rest>
    class Tuple<Current, Rest...> : public Tuple<Rest...> {
    private:
        using ThisType = Tuple<Current, Rest...>;
        using NextType = Tuple<Rest...>;

        Current value;

    protected:
        // Haskell
        template <
            template <size_t, class, size_t> class Pred, 
            size_t i, size_t counter, size_t... seq
        >
        class filter_impl {
        private:
            using MyPred = Pred<i, Current, counter>;
        
        public:
            using result = typename ternary<MyPred::result>
                ::template type<
                    typename NextType::template filter_impl<
                        Pred, i + 1, MyPred::counter, seq..., MyPred::index
                    >::result,
                    typename NextType::template filter_impl<
                        Pred, i + 1, counter, seq...
                    >::result
                >;
        };
        
    public:
        template <template <size_t, class, size_t> class Pred>
        using filter = typename filter_impl<Pred, 0, 0>::result;

        static constexpr size_t size = sizeof...(Rest) + 1;

        template <size_t i>
        using type_at = typename ternary<i == 0>
            ::template type<
                Current,
                typename NextType::template type_at<i - 1>
            >;

    public:
        constexpr Tuple(Current value, Rest... rest) :
            NextType(rest...),
            value(value) {}

        template <size_t i>
        constexpr decltype(auto) at() const {
            if constexpr (i == 0) {
                return value;
            }
            else {
                return this->NextType::template at<i - 1>();
            }
        }
    };
}

#endif /* GEODE_CORE_META_TUPLE_HPP */