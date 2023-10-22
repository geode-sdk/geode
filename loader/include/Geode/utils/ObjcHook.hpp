#pragma once

#include "../loader/Hook.hpp"
#include "Result.hpp"

namespace geode {
    namespace hook {
        /**
         * Add a new Objective-C method to a class. This method will be created
         * using the imp provided. If the method already exists, it won't do
         * anything.
         * @param className The name of the class to add the method to
         * @param selectorName The name of the method to add
         * @param imp The implementation of the method
         * @returns Ok() if the method was added successfully, or an error.
         */
        Result<> addObjcMethod(std::string const& className, std::string const& selectorName, void* imp);

        /**
         * Get the implementation of an Objective-C method. 
         * @param className The name of the class whose method to get
         * @param selectorName The name of the method to get
         * @returns The implementation of the method, or an error.
         */
        Result<void*> getObjcMethodImp(std::string const& className, std::string const& selectorName);
    }

    class ObjcHook {
    public:
        /**
         * Create a hook for an Objective-C method
         * @param className The name of the class whose method to hook
         * @param selectorName The name of the method to hook
         * @param function The detour to run when the method is called
         * @returns The created hook, or an error. 
         */
        template <class Func>
        static Result<Hook*> create(std::string const& className, std::string const& selectorName, Func function, tulip::hook::HookMetadata const& metadata = tulip::hook::HookMetadata()) {
            GEODE_UNWRAP_INTO(auto imp, geode::hook::getObjcMethodImp(className, selectorName));

            return Ok(Hook::create(
                getMod(),
                imp,
                function,
                className + "::" + selectorName,
                tulip::hook::TulipConvention::Default,
                metadata
            ));
        }

        /**
         * Create a hook for a new Objective-C method. This method will be 
         * created with a dummy implementation that does nothing. 
         * @param className The name of the class whose method to hook
         * @param selectorName The name of the method to hook
         * @param function The detour to run when the method is called
         * @param empty A function that takes no arguments and returns nothing. 
         * This is used to create a dummy method that can be hooked.
         * @returns The created hook, or an error. 
         */
        template <class Func>
        static Result<Hook*> create(std::string const& className, std::string const& selectorName, Func function, void(*empty)(), tulip::hook::HookMetadata const& metadata = tulip::hook::HookMetadata()) {
            GEODE_UNWRAP(geode::hook::addObjcMethod(className, selectorName, (void*)empty));

            return ObjcHook::create(className, selectorName, function, metadata);
        }
    };
}