#pragma once

#include "lex.hpp"
#include "ast.hpp"
#include <queue>

using std::queue;

#define loop while(true)
using Tokens = queue<Token>;

Token next(Tokens& tokens) {
	if (tokens.size() == 0) {
		cacerr("Unexpected EOF while parsing\n");
	}
	Token p = tokens.front();
	// epic debugger
	// std::cout << p.slice << std::endl;
	tokens.pop();
	return p;
}

Token peek(Tokens& tokens) {
	if (tokens.size() == 0) {
		cacerr("Unexpected EOF while parsing\n");
	}
	Token p = tokens.front();
	return p;
}

Token next_expect(Tokens& tokens, TokenType type, string tname) {
	auto t = next(tokens);
	if (t.type == type)
		return t;
	cacerr("Expected %s, found %s (next tokens %s)\n", tname.c_str(), t.slice.c_str(), next(tokens).slice.c_str());
}

bool next_if_type(TokenType type, Tokens& tokens) {
	auto t = peek(tokens);
	if (t.type == type) {
		next(tokens);
		return false;
	}
	return true;
}
string parseQualifiedName(Tokens& tokens);

inline bool checkEndOfType(bool qualifiedUsed, Tokens& tokens) {
	return (!qualifiedUsed || peek(tokens).type != kIdent) && 
		peek(tokens).type != kComma && peek(tokens).type != kParenR;
}
string parseReturn(Tokens& tokens) {
	string ret;
	bool qualifiedUsed = false;
	while (checkEndOfType(qualifiedUsed, tokens)) {
		if (peek(tokens).type == kIdent) {
			qualifiedUsed = true;
			ret += parseQualifiedName(tokens);
			if ((peek(tokens).type == kIdent || peek(tokens).type == kStruct || peek(tokens).type == kConst || peek(tokens).type == kUnsigned) && 
				checkEndOfType(qualifiedUsed, tokens)) ret += " ";
		}
		else {
			auto t = next(tokens);
			ret += t.slice;
			if ((t.type == kIdent || t.type == kStruct || t.type == kConst|| t.type == kUnsigned) && 
				(peek(tokens).type == kIdent || peek(tokens).type == kConst || peek(tokens).type == kUnsigned)
				&& checkEndOfType(qualifiedUsed, tokens)) ret += " ";
		}
	}
	// std::cout << ret << std::endl;

	return ret;
}

string parseQualifiedName(Tokens& tokens) {
	string qual;
	Token t;
	do {
		t = next(tokens);
		qual += t.slice;
	} while ((t.type == kQualifier && peek(tokens).type == kIdent) || (t.type == kIdent && peek(tokens).type == kQualifier));
	if (qual.size() == 0)
		cacerr("Expected identifier, found %s\n", next(tokens).slice.c_str());

	return qual;
}

string parseAttribute(Tokens& tokens) { // if attributes are expanded we will need to create a new attribute ast type intead of string
	string attrib_name = next_expect(tokens, kIdent, "identifier").slice;
	if (attrib_name == "mangle") {
		next_expect(tokens, kParenL, "(");
		string mangle = next_expect(tokens, kString, "string").slice;
		next_expect(tokens, kParenR, ")");
		next_expect(tokens, kAttrR, "]]");
		return mangle;
	} else if (attrib_name == "depends") {
		next_expect(tokens, kParenL, "(");
		string depends;
		while (next_if_type(kParenR, tokens)) {
			depends += next(tokens).slice;
		}
		next_expect(tokens, kAttrR, "]]");
		return depends;
	} else if (attrib_name == "docs") {
		next_expect(tokens, kParenL, "(");
		string docs = next_expect(tokens, kString, "string").slice;
		next_expect(tokens, kParenR, ")");
		next_expect(tokens, kAttrR, "]]");
		return docs;
	} else {
		// we can add more later
		cacerr("Invalid attribute %s\n", attrib_name.c_str());
		return "";
	}
}

void parseFunction(ClassDefinition& c, Function myFunction, Tokens& tokens) {
	next_expect(tokens, kParenL, "(");

	// std::cout << "func: " << myFunction.name << std::endl;
	while (next_if_type(kParenR, tokens)) {
		string args;
		string name;
		args = parseReturn(tokens);
		// std::cout << peek(tokens).slice << std::endl;
		// std::cout << "args: " << args << std::endl;
		if (next_if_type(kComma, tokens) && peek(tokens).type != kParenR) {
			name = next(tokens).slice;
		}
		// std::cout << "name: " << name << std::endl;
		if (args.size() == 0)
			continue;

		myFunction.args.push_back(args);
		myFunction.argnames.push_back(name);
	}

	if (!next_if_type(kConst, tokens))
		myFunction.is_const = true;
	// next_expect(tokens, kEqual, "=");
	if (peek(tokens).type == kFunDefinition) {
		string contents = next(tokens).slice; 
		myFunction.is_defined = true;
		myFunction.definition = contents;
	} else if (!next_if_type(kEqual, tokens)) {
		for (int k = 0; k < 4; ++k) {
			if (k == 3)
				cacerr("Maximum of 3 binds allowed\n");

			auto t = peek(tokens);
			if (t.type == kComma)
				myFunction.binds[k] = "";
			else if (t.type == kIdent) {
				next(tokens);
				auto platform = t.slice;
				t = next(tokens);
				if (t.slice == "0x0") t.slice = "";
				if (t.type != kAddress) cacerr("Expected address, found %s\n", t.slice.c_str());
				if (platform == "mac") myFunction.binds[kMacFunction] = t.slice;
				else if (platform == "win") myFunction.binds[kWindowsFunction] = t.slice;
				else if (platform == "ios") myFunction.binds[kIosFunction] = t.slice;
				else cacerr("Unknown identifier, found %s\n", t.slice.c_str());
			} else 
				cacerr("Expected platform identifier, found %s\n", t.slice.c_str());

			t = next(tokens);
			if (t.type == kSemi)
				break;
			if (t.type != kComma)
				cacerr("Expected comma, found %s.\n", t.slice.c_str());
		}
	} else next_expect(tokens, kSemi, ";");
	// myFunction.parent_class = &c;
	// myFunction.index = c.in_order.size();
	c.addField(myFunction);
}

void parseMember(ClassDefinition& c, string type, string varName, Tokens& tokens) {
	//cacerr("not implemented yet!!!\n");

	Member myMember;
	myMember.type = type;
	myMember.name = varName;

	if (varName == "PAD" && type == "void") myMember.member_type = MemberType::kPad;

	if (!next_if_type(kBrackL, tokens)) {
		auto num_maybe = next_expect(tokens, kIdent, "number").slice;
		if (num_maybe.find_first_not_of("0123456789") != string::npos) {
			cacerr("Expected number, found %s\n", num_maybe.c_str());
		}
		myMember.count = static_cast<size_t>(strtoll(num_maybe.c_str(), NULL, 10));

		next_expect(tokens, kBrackR, "]");
	}


	if (!next_if_type(kEqual, tokens)) {
		if (myMember.member_type != MemberType::kPad)
			myMember.member_type = MemberType::kHardcode;
		for (int k = 0; k < 5; ++k) {
			if (k == 4)
				cacerr("Maximum of 4 hardcodes allowed\n");

			auto t = next(tokens);
			if (t.type == kComma)
				myMember.hardcodes[k] = "";
			else if (t.type == kIdent) {
				auto platform = t.slice;
				t = next(tokens);
				if (t.slice == "0x0") t.slice = "";
				if (t.type != kAddress) cacerr("Expected address, found %s\n", t.slice.c_str());
				if (platform == "mac") myMember.hardcodes[kMacMember] = t.slice;
				else if (platform == "win") myMember.hardcodes[kWindowsMember] = t.slice;
				else if (platform == "ios") myMember.hardcodes[kIosMember] = t.slice;
				else if (platform == "android") myMember.hardcodes[kAndroidMember] = t.slice;
				else cacerr("Unknown identifier, found %s\n", t.slice.c_str());
			}
			else if (t.type == kAddress)
				myMember.hardcodes[k] = t.slice;
			else 
				cacerr("Expected platform identifier, found %s\n", t.slice.c_str());

			t = next(tokens);
			if (t.type == kSemi)
				break;
			if (t.type != kComma)
				cacerr("Expected comma, found %s.\n", t.slice.c_str());
		}
	} else {
		myMember.member_type = MemberType::kDefault;
		next_expect(tokens, kSemi, ";");
	}

	// myMember.parent_class = &c;
	// myMember.index = c.in_order.size();
	c.addField(myMember);
}

void parseField(ClassDefinition& c, Tokens& tokens) {
	string attrib;
	if (!next_if_type(kAttrL, tokens)) {
		attrib = parseAttribute(tokens);
	}

	FunctionType fn_type = kRegularFunction;

	if (!next_if_type(kVirtual, tokens)) {
		fn_type = kVirtualFunction;
	} else if (!next_if_type(kStatic, tokens)) {
		fn_type = kStaticFunction;
	}

	if (peek(tokens).type == kInlineExpr) {
		Inline i;
		// i.parent_class = &c;
		// i.index = c.in_order.size();
		// string inlined = next(tokens).slice;
		i.inlined = next(tokens).slice;//inlined.substr(1, inlined.size());
		c.addField(i);
		return;
	}

	vector<Token> return_name;
	loop {
		Token t = peek(tokens);
		bool break_ = false;
		switch (t.type) {
			case kStar:
			case kAmp:
			case kIdent:
			case kUnsigned:
			case kConst:
			case kTemplateExpr:
			case kQualifier:
			case kDtor:
				if (t.type == kDtor)
					fn_type = kDestructor;
				return_name.push_back(t);
				next(tokens);
				break;
			default:
				break_ = true;
				break;
		}
		if (break_)
			break;
	}

	if (return_name.empty())
		cacerr("Expected identifier, found %s\n", next(tokens).slice.c_str());
	if (return_name.back().type != kIdent)
		cacerr("Expected identifier, found %s\n", return_name.back().slice.c_str());

	string varName = return_name.back().slice;
	return_name.pop_back();
	string return_type;

	// sorry for adding logic to the parser cami <3
	if (return_name.size() == 0) {
		string qualifiedName = c.name;
		if (c.name.rfind("::") != string::npos) {
			qualifiedName = c.name.substr(c.name.rfind("::")+2);
			return_type = "auto";
		}
		else return_type = "void";
		if (qualifiedName == varName && fn_type == kRegularFunction) fn_type = kConstructor;
	}
	
	for (size_t i = 0; i < return_name.size(); ++i) {
		auto& t = return_name[i];
		return_type += t.slice;
		if (i+1 < return_name.size()) {
			auto& p = return_name[i+1];
			if ((t.type == kIdent || t.type == kConst || t.type == kUnsigned) && (p.type == kIdent || p.type == kConst || p.type == kUnsigned))
				return_type += " ";
		}

		
	}
		
	if (peek(tokens).type == kParenL) {
		Function myFunction;
		myFunction.return_type = return_type;
		if (attrib.find("/*") != string::npos) 
			myFunction.docs = attrib;
		else 
			myFunction.android_mangle = attrib;
		myFunction.function_type = fn_type;
		myFunction.name = varName;
		if (fn_type == kDestructor) myFunction.name = "~" + myFunction.name;
		return parseFunction(c, myFunction, tokens);
	}

	if (fn_type == kVirtualFunction)
		cacerr("Unexpected virtual keyword\n")
	if (fn_type == kStaticFunction)
		cacerr("Unexpected static keyword\n")
	return parseMember(c, return_type, varName, tokens);
}

void parseClass(Root& r, Tokens& tokens) {
	string attrib;
	if (!next_if_type(kAttrL, tokens)) {
		attrib = parseAttribute(tokens);
	}
	next_expect(tokens, kClass, "'class'");



	if (!next_if_type(kBraceL, tokens)) {
		ClassDefinition& myClass = r.addClass("");
		while (next_if_type(kBraceR, tokens)) {
			parseField(myClass, tokens);
		}
	} else {
		string name = parseQualifiedName(tokens);
		ClassDefinition& myClass = r.addClass(name);

	    stringstream f(attrib);
	    string s;    
	    while (getline(f, s, ',')) {
	        myClass.depends.push_back(s);
	    }

		if (!next_if_type(kColon, tokens)) {
			loop {
				auto sc = parseQualifiedName(tokens);
				myClass.addSuperclass(sc);
				//auto t = next(tokens);
				if (!next_if_type(kBraceL, tokens)) 
					break;
				next_expect(tokens, kComma, "comma");
			}
		} else {
			next_expect(tokens, kBraceL, "{");
		}

		while (next_if_type(kBraceR, tokens)) {
			parseField(myClass, tokens);
		}
	}
}

Root parseTokens(vector<Token> ts) {
	Root root;
	Tokens tokens;
	for (auto& t : ts) {
		tokens.push(t);
	}

	while (tokens.size() > 0) {
		parseClass(root, tokens);
	}

	//cacerr("s %d\n", tokens.size());
	return root;
}
