#include <Geode/utils/VMTHookManager.hpp>
#include <unordered_map>

using namespace geode::prelude;

struct VMTTableKey {
    std::string typenamePtr;
    ptrdiff_t thunkOffset = 0;

    bool operator==(const VMTTableKey& other) const {
        return typenamePtr == other.typenamePtr && thunkOffset == other.thunkOffset;
    }
};

struct VMTTableKeyHash {
    std::size_t operator()(const VMTTableKey& key) const {
        return std::hash<std::string>()(key.typenamePtr) ^ std::hash<ptrdiff_t>()(key.thunkOffset);
    }
};

struct VMTTableValue {
    void* vtable = nullptr;
};

struct VMTMapKey {
    VMTTableKey tableKey;
    ptrdiff_t vtableOffset = 0;

    bool operator==(const VMTMapKey& other) const {
        return tableKey == other.tableKey && vtableOffset == other.vtableOffset;
    }
};

struct VMTMapKeyHash {
    std::size_t operator()(const VMTMapKey& key) const {
        return VMTTableKeyHash{}(key.tableKey) ^ std::hash<ptrdiff_t>()(key.vtableOffset);
    }
};

struct VMTMapValue {
    void* vtable = nullptr;
    void* empty = nullptr;
    void* original = nullptr;
    std::vector<void*> detours;
};

class VMTHookManager::Impl {
public:
    std::unordered_map<VMTTableKey, VMTTableValue, VMTTableKeyHash> m_tables;
    std::unordered_map<VMTMapKey, VMTMapValue, VMTMapKeyHash> m_hooks;

    void*& getTable(void* instance, ptrdiff_t thunkOffset);
    void replaceTable(void* instance, ptrdiff_t thunkOffset, void* vtable);

    void*& getFunction(void* vtable, ptrdiff_t vtableOffset);
    void replaceFunction(void* vtable, ptrdiff_t vtableOffset, void* function);

    Result<std::optional<std::shared_ptr<Hook>>> addHook(
        void* instance,  ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize,
        void* emptyFunc, void* newFunc, std::string const& typeName, std::string const& displayName,
        tulip::hook::HandlerMetadata const& handlerMetadata,
        tulip::hook::HookMetadata const& hookMetadata
    );

    Result<> forceDisableFunction(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset);
    Result<> forceEnableFunction(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset);
};

void*& VMTHookManager::Impl::getTable(void* instance, ptrdiff_t thunkOffset) {
    auto i1 = reinterpret_cast<uintptr_t>(instance);
    return *reinterpret_cast<void**>(i1 + thunkOffset);
}

void VMTHookManager::Impl::replaceTable(void* instance, ptrdiff_t thunkOffset, void* vtable) {
    this->getTable(instance, thunkOffset) = vtable;
}

void*& VMTHookManager::Impl::getFunction(void* vtable, ptrdiff_t vtableOffset) {
    auto i1 = reinterpret_cast<uintptr_t>(vtable);
    return *reinterpret_cast<void**>(i1 + vtableOffset);
}

void VMTHookManager::Impl::replaceFunction(void* vtable, ptrdiff_t vtableOffset, void* function) {
    this->getFunction(vtable, vtableOffset) = function;
}

Result<> VMTHookManager::Impl::forceDisableFunction(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) {
    VMTMapKey mapKey{ typeName, thunkOffset, vtableOffset };
    auto mapIt = m_hooks.find(mapKey);

    if (mapIt != m_hooks.end()) {
        auto& value = mapIt->second;
        this->replaceFunction(value.vtable, vtableOffset, value.original);

        return Ok();
    }
    return Err("No hook found for the given type and thunk offset");
}

Result<> VMTHookManager::Impl::forceEnableFunction(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) {
    VMTMapKey mapKey{ typeName, thunkOffset, vtableOffset };
    auto mapIt = m_hooks.find(mapKey);

    if (mapIt != m_hooks.end()) {
        auto& value = mapIt->second;
        this->replaceFunction(value.vtable, vtableOffset, value.empty);

        return Ok();
    }
    return Err("No hook found for the given type and thunk offset");
}

Result<std::optional<std::shared_ptr<Hook>>> VMTHookManager::Impl::addHook(
    void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize,
    void* emptyFunc, void* newFunc, std::string const& typeName, std::string const& displayName,
    tulip::hook::HandlerMetadata const& handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata)
{
    VMTMapKey mapKey{ typeName, thunkOffset, vtableOffset };
    auto mapIt = m_hooks.find(mapKey);
    // log::debug("Map key: {}, {}, {}", typeName, thunkOffset, vtableOffset);

    if (mapIt == m_hooks.end()) {
        VMTTableKey tableKey{ typeName, thunkOffset };
        auto tableIt = m_tables.find(tableKey);
        // log::debug("Table key: {}, {}", typeName, thunkOffset);

        if (tableIt == m_tables.end()) {
            static constexpr size_t extraSize = sizeof(void*) * 3; // 2 for thunk offset + typeinfo pointer + virtual offset (unused in gd but idc)

            // we have not created the vtable yet, lets do that :D
            auto vtablem3 = operator new[](vtableSize + extraSize);
            // log::debug("Vtable (m3): {}", vtablem3);
            auto originalVtable = this->getTable(instance, thunkOffset);
            // log::debug("Original vtable: {}", originalVtable);
            auto originalVtablem3 = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(originalVtable) - extraSize);
            // log::debug("Original vtable (m3): {}", originalVtablem3);
            std::memcpy(vtablem3, originalVtablem3, vtableSize + extraSize);
            auto vtable = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(vtablem3) + extraSize);
            // log::debug("New vtable: {}", vtable);

            m_tables[tableKey] = { vtable };
        }

        // we already created the vtable, but have not replaced this specific function
        auto& value = m_tables[tableKey];
        auto originalFunc = this->getFunction(value.vtable, vtableOffset);
        // log::debug("Original function: {}", originalFunc);
        // log::debug("Replacing with: {}", emptyFunc);
        this->replaceFunction(value.vtable, vtableOffset, emptyFunc);

        // log::debug("Creating the original");
        auto hook = Hook::create(emptyFunc, originalFunc, displayName, handlerMetadata, tulip::hook::HookMetadata{
            .m_priority = INT_MAX
        });
        // log::debug("Claiming the hook");
        GEODE_UNWRAP_INTO(auto hook2, Mod::get()->claimHook(hook));

        // log::debug("Adding to hooks map");
        m_hooks[mapKey] = { value.vtable, emptyFunc, originalFunc };
    }

    // we have a table and we replaced the function
    // log::debug("Replacing the table in the instance {}", instance);
    auto& value = m_hooks[mapKey];
    this->replaceTable(instance, thunkOffset, value.vtable);

    auto& detours = value.detours;
    if (std::find(detours.begin(), detours.end(), newFunc) != detours.end()) {
        // already added a hook for this function, just replace the table
        // log::debug("Already added a hook for this function, just replacing the table");
        return Ok(std::nullopt);
    }
    // need to generate the hook based on the existing original
    // log::debug("Adding new detour: {}", newFunc);
    auto hook = Hook::create(value.empty, newFunc, displayName, handlerMetadata, hookMetadata);
    // log::debug("Done");
    detours.push_back(newFunc);
    return Ok(std::move(hook));
}

VMTHookManager& VMTHookManager::get() {
    static VMTHookManager instance;
    return instance;
}

VMTHookManager::VMTHookManager() : m_impl(std::make_unique<Impl>()) {}
VMTHookManager::~VMTHookManager() = default;

Result<std::optional<std::shared_ptr<Hook>>> VMTHookManager::addHookInternal(
    void* instance, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset, size_t vtableSize,
    void* emptyFunc, void* newFunc, std::string const& typeName, std::string const& displayName,
    tulip::hook::HandlerMetadata const& handlerMetadata,
    tulip::hook::HookMetadata const& hookMetadata
) {
    return m_impl->addHook(
        instance, thunkOffset, vtableOffset, vtableSize, emptyFunc,
        newFunc, typeName, displayName, handlerMetadata, hookMetadata
    );
}

Result<> VMTHookManager::forceEnableFunctionInternal(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) {
    return m_impl->forceEnableFunction(instance, typeName, thunkOffset, vtableOffset);
}

Result<> VMTHookManager::forceDisableFunctionInternal(void* instance, std::string const& typeName, ptrdiff_t thunkOffset, ptrdiff_t vtableOffset) {
    return m_impl->forceDisableFunction(instance, typeName, thunkOffset, vtableOffset);
}