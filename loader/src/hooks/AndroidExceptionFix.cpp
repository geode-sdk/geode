#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID

// replaces https://github.com/llvm-mirror/libcxxabi/blob/master/src/private_typeinfo.cpp#L213
// this is the function that checks if a type can catch an exception
bool canCatchImpl(const std::type_info* self, const std::type_info* thrown, void*& adjustedPtr) {
    // cause who is gonna use foreign exceptions anyway
#if 0
    if (std::strcmp(self->name(), typeid(std::exception).name()) == 0) {
        return true;
    }
    if (adjustedPtr == nullptr) {
        return false;
    }
#endif
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(*(intptr_t*)adjustedPtr);
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable);
    auto typeinfo = complete->m_typeinfo;
    return traverseTypeinfoFor(adjustedPtr, typeinfo, self->name());
}

$execute {
    // we get the __class_type_info::can_catch address from the typeinfo of a dummy class
    geode::cast::DummyClass dummyClass;
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(*(intptr_t*)&dummyClass);
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable);
    auto typeinfo = complete->m_typeinfo;
    auto tiVtable = typeinfo->m_typeinfoVtable;
    auto hookAddr = tiVtable->m_vtable[4]; // 5th entry is can_catch

    (void)Mod::get()->hook(hookAddr, &canCatchImpl, "__cxxabiv1::__class_type_info::can_catch");
}

#endif