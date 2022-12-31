#pragma once
#include <Geode/Bindings.hpp>

template <auto... Values>
struct False {
    static constexpr auto value = false;
};

template<int Difference, class Class, class Member, bool Predicate>
struct MemberIsOffsetBy {
    static_assert(False<Difference>::value, "Member is offset by this value");
};

template<int Difference, class Class, class Member>
struct MemberIsOffsetBy<Difference, Class, Member, true> {
    
};

template <class Member, class Class, int Offset, int Expected>
struct SingleChecker {
    using IsOffsetBy = MemberIsOffsetBy<Offset - Expected, Class, Member, Expected == Offset>;
};

#define GEODE_MEMBER_CHECK(Class_, Member_, Offset_) class Member_; SingleChecker<Member_, Class_, offsetof(Class_, Member_), Offset_>::IsOffsetBy GEODE_CONCAT(OffsetBy, __LINE__)
