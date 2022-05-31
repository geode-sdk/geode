#pragma once

#include "../codegen-base/Macros.hpp"
#include "../utils/include.hpp"
#include "Mod.hpp"
#include <functional>
#include <string>
#include <vector>

namespace geode {
    class Dispatcher;
    
    struct GEODE_DLL dispatch_handle {
        void* handle;

        dispatch_handle() = delete;
        dispatch_handle(dispatch_handle const& d) : handle(d.handle) {}

        template <typename T>
        std::function<T> into() {
            *reinterpret_cast<std::function<T>*>(this->handle);
        }

        template <typename R, typename ...Args>
        R call(Args... args) {
            return this->into<std::function<R(Args...)>>()(args...);
        }

        bool operator<(dispatch_handle const& b) const {
            return handle < b.handle;
        }

        bool operator==(dispatch_handle const& b) const {
            return handle == b.handle;
        }
        
     private:
        dispatch_handle(void* h) : handle(h) {}

        template <typename T>
        static dispatch_handle from(std::function<T> f) {
            return dispatch_handle(reinterpret_cast<void*>(new std::function<T>(f)));
        }
        friend class Dispatcher;
    };

    class GEODE_DLL Dispatcher {
     protected:
        std::map<dispatch_handle, std::pair<std::string, Mod*>> m_dispatchMap;
        std::map<std::string, std::vector<dispatch_handle>> m_selectorMap;
        // 2 maps for lookup speed

        std::vector<dispatch_handle> getFunctions_(std::string const& a);

        void addFunction_(Mod* m, std::string const& a, dispatch_handle u);

        std::vector<dispatch_handle> allFunctions_();

        Dispatcher() {}

     public:
        static Dispatcher* get();

        template <typename T>
        std::vector<std::function<T>> getFunctions(std::string const& a) {
            return geode::vector_utils::map(getFunctions_(a), [this](dispatch_handle fn) {
                return fn.into<T>();
            });
        }

        template <typename T>
        std::function<T> getFunction(std::string const& a) {
            return getFunctions_(a).back().into<T>();
        }

        template <typename T>
        dispatch_handle addFunction(std::string const& a, std::function<T> f) {
            dispatch_handle hdl = dispatch_handle::from(f);
            addFunction_(Mod::get(), a, hdl);
            return hdl;
        }

        template <typename T>
        std::vector<dispatch_handle> allFunctions() {
            return geode::vector_utils::map(allFunctions_(), [this](dispatch_handle fn) {
                return fn.into<T>();
            });
        }

        void removeFunction(dispatch_handle u);
        std::pair<std::string, Mod*> getFunctionInfo(dispatch_handle u);
    };
}