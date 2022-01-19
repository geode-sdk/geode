#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using std::vector, std::unordered_map, std::string, std::is_same_v, std::cout, std::cin, std::endl;

struct ClassDefinition;

enum FieldType {
	kFunction=0,
	kMember=1,
	kInline=2
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

struct Function : ClassField {
	Function() : is_const(), return_type(), name(), args(), binds(), android_mangle(), index() {}
	bool is_const;
	FunctionType function_type;

	string return_type;
	string name;
	vector<string> args;

	string binds[3]; // mac, windows, ios (android has all symbols included). No binding = no string. Stored as a string because no math is done on it
	string android_mangle; // only sometimes matters. empty if irrelevant
	size_t index;
};

struct Member : ClassField {
	Member() : type(), name(), hardcode(), hardcodes(), count() {}
	string type;
	string name;
	bool hardcode;
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
	vector<string> superclasses;
	vector<Function> functions;
	vector<Member> members;
	vector<Inline> inlines;
	vector<ClassField*> in_order;

	void addSuperclass(string sclass) {
		if (std::find(superclasses.begin(), superclasses.end(), sclass) == superclasses.end()) {
			superclasses.push_back(sclass);	
		}
		// intentional
		// else cacerr("Duplicate superclass %s for class %s\n", sclass.c_str(), name.c_str());
	}

	template<typename T>
	void addField(T& field) {
		field.parent_class = this;
		if constexpr (is_same_v<Function, T>) {
			field.index = functions.size();
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
