#pragma once

#include <Geode/DefaultInclude.hpp>
#include <algorithm>
#include <string>

#undef min
#undef max

namespace geode::utils::ranges {
    template <class C>
    concept ValidConstContainer = requires(C const& c) {
        c.begin();
        c.end();
        typename C::value_type;
    };

    template <class C>
    concept ValidMutContainer = requires(C& c) {
        c.begin();
        c.end();
        typename C::value_type;
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

    template <ValidConstContainer C, class Output>

    requires std::is_default_constructible_v<Output> &&
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
            res += Conv(p);
        }
        return res;
    }

    template <ValidContainer C>
    C& push(C& container, C const& toAdd) {
        container.insert(container.end(), toAdd.begin(), toAdd.end());
        return container;
    }

    template <ValidMutContainer C>
    C& remove(C& container, typename C::value_type const& value) {
        container.erase(std::remove(container.begin(), container.end(), value), container.end());
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
        ValidConstContainer From, ValidContainer Into,
        ValidIntoConverter<typename From::value_type, typename Into::value_type> Mapper>
    Into map(From const& from, Mapper mapper) {
        auto res = Into();
        std::transform(from.begin(), from.end(), res.end(), mapper);
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
}
