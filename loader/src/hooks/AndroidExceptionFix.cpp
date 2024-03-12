#include <Geode/Geode.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_ANDROID32

bool canCatchImpl(const std::type_info* self, const std::type_info* thrown, void*& adjustedPtr) {
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(*(intptr_t*)adjustedPtr);
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable);
    auto typeinfo = complete->m_typeinfo;
    return traverseTypeinfoFor(adjustedPtr, typeinfo, self->name());
}

$execute {
    geode::cast::DummyClass dummyClass;
    auto vtable = reinterpret_cast<geode::cast::VtableType*>(&dummyClass);
    auto complete = static_cast<geode::cast::CompleteVtableType*>(vtable);
    auto typeinfo = complete->m_typeinfo;
    auto hookAddr = typeinfo->m_typeinfoVtable->m_vtable[4];

    (void)Mod::get()->hook(hookAddr, &canCatchImpl, "__class_type_info::can_catch");
}

#endif
