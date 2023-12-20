#pragma once
#include <Geode/Bindings.hpp>

template <auto... Values>
struct False {
    static constexpr auto value = false;
};

template<int Difference, int Expected, int Current, class Class, class Member, bool Predicate>
struct MemberIsOffsetBy {
    static_assert(False<Difference, Expected, Current>::value, "Member is offset by this value (Difference, Expected, Current)");
};

template<int Difference, int Expected, int Current, class Class, bool Predicate>
struct SizeIsOffsetBy {
    static_assert(False<Difference, Expected, Current>::value, "Size is offset by this value (Difference, Expected, Current)");
};

template<int Difference, int Expected, int Current, class Class, class Member>
struct MemberIsOffsetBy<Difference, Class, Member, true> {
    
};

template<int Difference, int Expected, int Current, class Class>
struct SizeIsOffsetBy<Difference, Class, true> {
    
};

template <class Member, class Class, int Offset, int Expected>
struct SingleMemberChecker {
    using IsOffsetBy = MemberIsOffsetBy<Offset - Expected, Expected, Offset, Class, Member, Expected == Offset>;
};

template <class Class, int Offset, int Expected>
struct SingleSizeChecker {
    using IsOffsetBy = MemberIsOffsetBy<Offset - Expected, Expected, Offset, Class, Member, Expected == Offset>;
};

#define GEODE_MEMBER_CHECK(Class_, Member_, Offset_) class Member_; SingleMemberChecker<Member_, Class_, offsetof(Class_, Member_), Offset_>::IsOffsetBy GEODE_CONCAT(OffsetBy, __LINE__)

#define GEODE_SIZE_CHECK(Class_, Offset_) class Class_; SingleMemberChecker<Class_, sizeof(Class_), Offset_>::IsOffsetBy GEODE_CONCAT(OffsetBy, __LINE__)
