#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <functional>
#include <sstream>
#include <iomanip>

using std::vector, std::unordered_map, std::string, std::is_same_v, std::cout, std::cin, std::endl, std::stringstream;

struct ClassDefinition;

enum Platform {
    kMac=0,
    kWindows=1,
    kIos=2,
    kAndroid=3
};

enum PlatformMember {
    kMacMember=0,
    kWindowsMember=1,
    kIosMember=0,
    kAndroidMember=2
};

enum PlatformFunction {
    kMacFunction=0,
    kWindowsFunction=1,
    kIosFunction=2,
    kAndroidFunction=-1
};

enum FieldType {
	kFunction=0,
	kMember=1,
	kInline=2,
};

struct ClassField {
	FieldType field_type;
	ClassDefinition* parent_class;
};

enum FunctionType {
	kVirtualFunction=0,
	kStaticFunction=1,
	kRegularFunction=2,
	//kStructorCutoff=10, // used for comparisons
	kConstructor=11,
	kDestructor=12
};

enum MemberType {
	kDefault=0,
	kHardcode=1,
	kPad=2
};

struct Function : ClassField {
	Function() : is_const(), return_type(), name(), args(), binds(), android_mangle(), index(), is_defined(), definition(), docs() {}
	bool is_const;
	FunctionType function_type;

	string return_type;
	string name;
	string docs;
	vector<string> args;
	vector<string> argnames;

	string binds[3]; // mac, windows, ios (android has all symbols included). No binding = no string. Stored as a string because no math is done on it
	string android_mangle; // only sometimes matters. empty if irrelevant
	size_t index;

	bool is_defined;
	string definition;
	bool same(Function const& other) const {
		if (name != other.name) return false;
		// if (return_type != other.return_type) return false;
		// if (is_const != other.is_const) return false;
		// if (function_type != other.function_type) return false;
		if (args.size() != other.args.size()) return false;
		for (auto i = 0u; i < args.size(); ++i) if (args[i] != other.args[i]) return false;
		return true;
	}
	void merge(Function const& other) {
		for (int i = 0; i < 3; ++i)
			if (binds[i] == "") binds[i] = other.binds[i];
		if (return_type == "void") return_type = other.return_type;
		for (auto i = 0u; i < args.size(); ++i) args[i] = other.args[i];
	}
	inline string hash() const;
};

struct Member : ClassField {
	Member() : type(), name(), hardcodes(), count() {}
	string type;
	string name;
	MemberType member_type;
	string hardcodes[3]; // mac/ios, windows, android
	size_t count; // for arrays
	
};

struct Inline : ClassField {
	Inline() : inlined() {}
	string inlined;
};

struct Root;
struct ClassDefinition {
	string name;
	vector<string> depends;
	vector<string> superclasses;
	vector<Function> functions;
	vector<Member> members;
	vector<Inline> inlines;
	vector<ClassField*> in_order;

	void addSuperclass(string sclass) {
		if (std::find(superclasses.begin(), superclasses.end(), sclass) == superclasses.end()) {
			superclasses.push_back(sclass);	
			depends.push_back(sclass);	
		}
		// intentional
		// else cacerr("Duplicate superclass %s for class %s\n", sclass.c_str(), name.c_str());
	}

	template<typename T>
	void addField(T& field) {
		field.parent_class = this;
		if constexpr (is_same_v<Function, T>) {
			field.index = functions.size();
			for (auto& f : functions) {
				if (f.same(field)) {
					f.merge(field);
					return;
				}
			}
			field.field_type = FieldType::kFunction;
			functions.push_back(field);
		}
		if constexpr (is_same_v<Member, T>) {
			field.field_type = FieldType::kMember;
			members.push_back(field);
		}
		if constexpr (is_same_v<Inline, T>) {
			field.field_type = FieldType::kInline;
			inlines.push_back(field);
		}
	}
};

struct Root {
	unordered_map<string, ClassDefinition> classes;

	ClassDefinition& addClass(string name) {
		if (classes.find(name) == classes.end()) {
			classes[name] = ClassDefinition();
			classes[name].name = name;
		}
		return classes[name];
	}
};

inline string Function::hash() const {
	string argstring;
	for (auto& s : args) argstring += s;
	uint32_t hash = std::hash<std::string>{}(argstring + name + parent_class->name);
	stringstream stream;
	stream << std::setbase(16) << hash;
	return stream.str();
}
