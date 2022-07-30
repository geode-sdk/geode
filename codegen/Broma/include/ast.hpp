#include <string>
#include <vector>
#include <variant>
#include <map>
#include <algorithm>
#include <iostream>

enum class Platform {
	Mac,
	iOS,
	Windows,
	Android
};

struct PlatformNumber {
	size_t mac = 0;
	size_t ios = 0;
	size_t win = 0;
	size_t android = 0;
};

struct Type {
	bool is_struct = false;
	std::string name;

	bool operator==(Type const& t) const {
		return name == t.name;
	}
};

enum class FunctionType {
	Normal,
	Ctor,
	Dtor
};

struct FunctionBegin {
	Type ret;
	FunctionType type = FunctionType::Normal;
	std::vector<std::pair<Type, std::string>> args;
	bool is_const = false;
	bool is_virtual = false;
	bool is_static = false;
	std::string docs;
	std::string name;

	inline bool operator==(FunctionBegin const& f) const {
		if (name != f.name || is_const != f.is_const || args.size() != f.args.size())
			return false;

		for (int i=0; i<args.size(); ++i) {
			if (!(args[i].first == f.args[i].first))
				return false;
		}

		std::cout << f.ret.name << " " << f.name << "\n";

		return true;
	}
};

struct MemberField {
	std::string name;
	Type type;
	size_t count = 0;
};

struct PadField {
	PlatformNumber amount;
};

struct FunctionBindField {
	FunctionBegin beginning;
	PlatformNumber binds;
};

struct OutOfLineField {
	FunctionBegin beginning;
	std::string inner;
};

struct InlineField {
	std::string inner;
};

struct Class;
struct Field {
	size_t field_id;
	std::string parent;
	std::variant<InlineField, OutOfLineField, FunctionBindField, PadField, MemberField> inner;

	template <typename T>
	T* get_as() {
		return std::get_if<T>(&inner);
	}

	template <typename T>
	T const* get_as() const {
		return std::get_if<T>(&inner);
	}

	inline FunctionBegin* get_fn() {
		if (auto fn = get_as<OutOfLineField>()) {
			return &fn->beginning;
		} else if (auto fn = get_as<FunctionBindField>()) {
			return &fn->beginning;
		} else return nullptr;
	}
};

struct Class {
	std::string name;
	std::vector<std::string> superclasses;
	std::vector<std::string> depends;
	std::vector<Field> fields;

	inline bool operator==(Class const& c) const {
		return name == c.name;
	}
	inline bool operator==(std::string const& n) const {
		return name == n;
	}
};

struct Root {
	std::vector<Class> classes;

	inline Class* operator[](std::string const& name) {
		auto it = std::find_if(classes.begin(), classes.end(), [name](Class& cls) {
		        return cls.name == name;
		});

		if (it == classes.end())
			return nullptr;

		return &*it;
	}
};
