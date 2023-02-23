#pragma once

#include <Geode/DefaultInclude.hpp>
#include <algorithm>
#include <string>
#include <concepts>
#include <optional>
#include <iterator>

#ifndef __cpp_lib_concepts
namespace std {
    // <concepts> isn't working for me lmao
    template <class From, class To>
    concept convertible_to = std::is_convertible_v<From, To> && requires {
        static_cast<To>(std::declval<From>());
    };
}
#endif

#undef min
#undef max

namespace geode::utils::ranges {
    template <class C>
    concept ValidConstContainer = requires(C const& c) {
        c.begin();
        c.end();
        { c.size() } -> std::convertible_to<size_t>;
        typename C::value_type;
        typename C::iterator;
        typename C::const_iterator;
    };

    template <class C>
    concept ValidMutContainer = requires(C& c) {
        c.begin();
        c.end();
        { c.size() } -> std::convertible_to<size_t>;
        typename C::value_type;
        typename C::iterator;
        typename C::const_iterator;
    };

    template <class C>
    concept ValidContainer = ValidConstContainer<C> && ValidMutContainer<C>;

    template <class P, class C>
    concept ValidCUnaryPredicate = requires(P p, typename C::value_type const& t) {
        { p(t) } -> std::convertible_to<bool>;
    };

    template <class P, class From, class Into>
    concept ValidIntoConverter = requires(P p, From const& t) {
        { p(t) } -> std::convertible_to<Into>;
    };

    template <ValidConstContainer C>
    bool contains(C const& cont, typename C::value_type const& elem) {
        return std::find(cont.begin(), cont.end(), elem) != cont.end();
    }

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate>
    bool contains(C const& cont, Predicate fun) {
        return std::find_if(cont.begin(), cont.end(), fun) != cont.end();
    }

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate>
    std::optional<typename C::value_type> find(C const& cont, Predicate fun) {
        auto it = std::find_if(cont.begin(), cont.end(), fun);
        if (it != cont.end()) {
            return std::optional(*it);
        }
        return std::nullopt;
    }

    template <ValidConstContainer C>
    std::optional<size_t> indexOf(C const& cont, typename C::value_type const& elem) {
        auto it = std::find(cont.begin(), cont.end(), elem);
        if (it != cont.end()) {
            return std::optional(std::distance(cont.begin(), it));
        }
        return std::nullopt;
    }

    template <ValidConstContainer C, ValidCUnaryPredicate<C> Predicate>
    std::optional<size_t> indexOf(C const& cont, Predicate fun) {
        auto it = std::find_if(cont.begin(), cont.end(), fun);
        if (it != cont.end()) {
            return std::optional(std::distance(cont.begin(), it));
        }
        return std::nullopt;
    }

    template <ValidMutContainer C>
    bool move(C& cont, typename C::value_type const& elem, size_t where) {
        if (where > cont.size() - 1) {
            return false;
        }
        auto ix = indexOf(cont, elem);
        if (ix) {
            if (ix.value() > where) {
                std::rotate(
                    cont.rend() - ix.value() - 1,
                    cont.rend() - ix.value(),
                    cont.rend() - where
                );
            } else {
                std::rotate(
                    cont.begin() + ix.value(),
                    cont.begin() + ix.value() + 1,
                    cont.begin() + where + 1
                );
            }
            return true;
        }
        return false;
    }

    template <ValidConstContainer C, class Output>
        requires
            std::is_default_constructible_v<Output> &&
            std::is_convertible_v<Output, typename C::value_type>
    Output join(C const& cont, Output const& separator) {
        auto res = Output();
        bool first = true;
        for (auto& p : cont) {
            if (!first) {
                res += separator;
            }
            else {
                first = false;
            }
            res += p;
        }
        return res;
    }

    template <ValidConstContainer C>
    std::string join(C const& cont, std::string const& separator) {
        auto res = std::string();
        bool first = true;
        for (auto& p : cont) {
            if (!first) {
                res += separator;
            }
            else {
                first = false;
            }
            res += p;
        }
        return res;
    }

    template <
        ValidConstContainer C, class Output,
        ValidIntoConverter<typename C::value_type, Output> Conv>

    requires std::is_default_constructible_v<Output> Output
    join(C const& cont, Output const& separator, Conv converter) {
        auto res = Output();
        bool first = true;
        for (auto& p : cont) {
            if (!first) {
                res += separator;
            }
            else {
                first = false;
            }
            res += converter(p);
        }
        return res;
    }

    template <ValidContainer C>
    C& push(C& container, C const& toAdd) {
        container.insert(container.end(), toAdd.begin(), toAdd.end());
        return container;
    }

    template <ValidContainer C>
    C concat(C const& cont, typename C::value_type const& value) {
        auto copy = cont;
        copy.push_back(value);
        return copy;
    }

    template <ValidContainer C>
    C concat(C const& cont1, C const& cont2) {
        auto copy = cont1;
        ranges::push(copy, cont2);
        return copy;
    }

    template <ValidMutContainer C>
    C& remove(C& container, typename C::value_type const& value) {
        container.erase(std::remove(container.begin(), container.end(), value), container.end());
        return container;
    }

    template <ValidMutContainer C, ValidCUnaryPredicate<C> Predicate>
    C& remove(C& container, Predicate fun) {
        container.erase(
            std::remove_if(container.begin(), container.end(), fun),
            container.end()
        );
        return container;
    }

    template <ValidContainer C, ValidCUnaryPredicate<C> Predicate>
    C filter(C const& container, Predicate filterFun) {
        auto res = C();
        std::copy_if(container.begin(), container.end(), res.end(), filterFun);
        return res;
    }

    template <class R, ValidConstContainer C, class Reducer>

    requires requires(Reducer r, R& acc, typename C::value_type t) {
        r(acc, t);
    }

    R reduce(C const& container, Reducer reducer) {
        auto res = R();
        for (auto& item : container) {
            reducer(res, item);
        }
        return res;
    }

    template <
        ValidContainer Into, ValidConstContainer From,
        ValidIntoConverter<typename From::value_type, typename Into::value_type> Mapper>
    Into map(From const& from, Mapper mapper) {
        auto res = Into();
        std::transform(from.begin(), from.end(), std::back_inserter(res), mapper);
        return res;
    }

    template <ValidConstContainer C>
    typename C::value_type min(C const& container) {
        auto it = std::min_element(container.begin(), container.end());
        if (it == container.end()) {
            return C::value_type();
        }
        return *it;
    }

    template <class T, ValidConstContainer C, ValidIntoConverter<typename C::value_type, T> Member>
        requires requires(T a, T b) {
            a < b;
        }
    T min(C const& container, Member member) {
        auto it = std::min_element(
            container.begin(), container.end(),
            [member](auto const& a, auto const& b) -> bool {
                return member(a) < member(b);
            }
        );
        if (it == container.end()) {
            return T();
        }
        return member(*it);
    }

    template <ValidConstContainer C>
    typename C::value_type max(C const& container) {
        auto it = std::max_element(container.begin(), container.end());
        if (it == container.end()) {
            return C::value_type();
        }
        return *it;
    }

    template <class T, ValidConstContainer C, ValidIntoConverter<typename C::value_type, T> Member>
        requires requires(T a, T b) {
            a < b;
            T();
        }
    T max(C const& container, Member member) {
        auto it = std::max_element(
            container.begin(), container.end(),
            [member](auto const& a, auto const& b) -> bool {
                return member(a) < member(b);
            }
        );
        if (it == container.end()) {
            return T();
        }
        return member(*it);
    }

    template <class C>
    struct ReverseWrapper {
        C iter;

        decltype(auto) begin() {
            return std::rbegin(iter);
        }

        decltype(auto) end() {
            return std::rend(iter);
        }
    };

    template <class C>
    auto reverse(C&& iter) {
        return ReverseWrapper<C>{std::forward<C>(iter)};
    }
}
