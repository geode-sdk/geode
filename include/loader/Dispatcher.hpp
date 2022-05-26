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
		dispatch_handle(dispatch_handle&& d) : handle(d.handle) {}
		dispatch_handle(dispatch_handle const& d) : handle(d.handle) {}

		template <typename T>
		std::function<T> get() {
			*reinterpret_cast<std::function<T>*>(this->handle);
		}

		template <typename R, typename ...Args>
		R call(Args... args) {
			return this->get<std::function<R(Args...)>>()(args...);
		}
		
	 private:
		dispatch_handle(void* h) : handle(h) {}
		friend class Dispatcher;
	};

	class GEODE_DLL Dispatcher {
	 protected:
	 	std::map<dispatch_handle, std::pair<std::string, Mod*>> m_dispatchMap;
	 	std::map<std::string, std::vector<dispatch_handle>> m_selectorMap;
	 	// 2 maps for lookup speed

	 	std::vector<dispatch_handle> _getHandles(std::string const& a);
	 	std::pair<string, Mod*> _getInfo(dispatch_handle u);

	 	void _removeFunction(dispatch_handle u);
	 	dispatch_handle _addFunction(Mod* m, std::string const& a);

	 	std::vector<dispatch_handle> _allHandles();




	 	template <typename T>
	 	std::function<T> _fromOpaque(void* f) {
	 		*reinterpret_cast<std::function<T>*>(f);
	 	}

	 	template <typename T>
	 	void* _toOpaque(std::function<T> f) {
	 		return reinterpret_cast<void*>(new std::function<T>(f));
	 	}
	 public:
		static Dispatcher* get();

		template <typename T>
		std::vector<std::function<T>> getSelectors(std::string_view const& a) {
			return geode::vector_utils::map(_getSelectors(a), [this](std::pair<dispatch_handle, void*> selector) {
				return _fromOpaque<T>(selector.second);
			});
		}

	};
}