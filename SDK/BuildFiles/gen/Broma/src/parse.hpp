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

string parseQualifiedName(Tokens& tokens) {
	string qual;
	while (peek(tokens).type == kIdent || peek(tokens).type == kQualifier) {
		qual += next(tokens).slice;
	}
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
	} else {
		// we can add more later
		cacerr("Invalid attribute %s\n", attrib_name.c_str());
		return "";
	}
}

void parseFunction(ClassDefinition& c, Function myFunction, Tokens& tokens) {
	next_expect(tokens, kParenL, "(");

	while (next_if_type(kParenR, tokens)) {
		string args;
		while (next_if_type(kComma, tokens) && peek(tokens).type != kParenR) {
			auto t = next(tokens);
			args += t.slice;
			if ((t.type == kIdent || t.type == kConst) && (peek(tokens).type == kIdent || peek(tokens).type == kConst))
				args += " ";
		}
		if (args.size() == 0)
			continue;

		myFunction.args.push_back(args);
	}

	if (!next_if_type(kConst, tokens))
		myFunction.is_const = true;
	// next_expect(tokens, kEqual, "=");

	if (!next_if_type(kEqual, tokens)) {
		for (int k = 0; k < 4; ++k) {
			if (k == 3)
				cacerr("Maximum of 3 binds allowed\n");

			auto t = peek(tokens);
			if (t.type == kComma)
				myFunction.binds[k] = "";
			else if (t.type == kAddress) {
				next(tokens);
				myFunction.binds[k] = t.slice;
			} else 
				cacerr("Expected address, found %s\n", t.slice.c_str());

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
	if (!next_if_type(kBrackL, tokens)) {
		auto num_maybe = next_expect(tokens, kIdent, "number").slice;
		if (num_maybe.find_first_not_of("0123456789") != string::npos) {
			cacerr("Expected number, found %s\n", num_maybe.c_str());
		}
		myMember.count = strtoll(num_maybe.c_str(), NULL, 10);

		next_expect(tokens, kBrackR, "]");
	}

	if (!next_if_type(kEqual, tokens)) {
		myMember.hardcode = true;
		for (int k = 0; k < 4; ++k) {
			if (k == 3)
				cacerr("Maximum of 3 hardcodes allowed\n");

			auto t = next(tokens);
			if (t.type == kComma)
				myMember.hardcodes[k] = "";
			else if (t.type == kAddress)
				myMember.hardcodes[k] = t.slice;
			else 
				cacerr("Expected address, found %s\n", t.slice.c_str());

			t = next(tokens);
			if (t.type == kSemi)
				break;
			if (t.type != kComma)
				cacerr("Expected comma, found %s.\n", t.slice.c_str());
		}
	} else next_expect(tokens, kSemi, ";");

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
		string inlined = next(tokens).slice;
		i.inlined = inlined.substr(1, inlined.size());
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
			if ((t.type == kIdent || t.type == kConst) && (p.type == kIdent || p.type == kConst))
				return_type += " ";
		}
		
	}
		
	if (peek(tokens).type == kParenL) {
		Function myFunction;
		myFunction.return_type = return_type;
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
	next_expect(tokens, kClass, "'class'");
	string name = parseQualifiedName(tokens);
	ClassDefinition& myClass = r.addClass(name);

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
