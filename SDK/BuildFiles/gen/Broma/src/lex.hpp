#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "err.hpp"
using std::stringstream;
using std::string;

enum TokenType {
	kIdent,
	kParenR,
	kParenL,
	kTemplateExpr,
	kBraceR,
	kBraceL,
	kBrackL,
	kBrackR,
	kAttrL,
	kAttrR,
	kSemi,
	kAddress,
	kStar,
	kAmp,
	kQualifier,
	kDtor,
	kEqual,
	kAuto,
	kColon,
	kComma,
	kIgnore,
	kString,
	kClass,
	kStatic,
	kVirtual,
	kConst,
	kInlineExpr
};

struct Token {
	TokenType type;
	string slice;
};

Token parseIdent(stringstream& stream) {
    while (stream.peek() == ' ') {
        stream.get();
    }

    string slice;
    do {
        char j = (stream).peek();
        bool real = false;

        if ((j >= -1 && isalnum(j)) || j == '_') {
            slice += j;
            real = true;
        }

        if (!real)
            return {kIdent, slice};
        stream.get();
    } while (stream);

    cacerr("Unexpected EOF\n");
}

void parseString(stringstream& stream, string& slice, char start) {
	slice.clear();
	slice.reserve(128);
	while (stream.peek() != start) {
		slice += stream.get();
	}
	stream.get();
	slice.shrink_to_fit();
}

void parseInline(stringstream& stream, string& slice) {
	slice.clear();
	slice.reserve(256);
	while (stream.peek() != '{' && stream.peek() != '\n') {
		slice += stream.get();
	}
	if (stream.peek() == '\n') {
		slice.shrink_to_fit();
		return;
	}
	slice += stream.get();
	int brace_nest = 1;
	char quote = '\0';
	bool comment_multi = false;
	while (brace_nest > 0) {
		char p = stream.get();

		if (comment_multi) {
			slice += p;
			if (p == '*' && stream.peek() == '/') {
				comment_multi = false;
				slice += stream.get();
			}
		} else if (quote) {
			slice += p;
			if (p == '\\' && stream.peek() == quote) {
				slice += stream.get();
			} else if (p == quote) {
				quote = '\0';
			}
		} else {
			slice += p;
			if (p == '}')
				--brace_nest;
			if (p == '{')
				++brace_nest;
			if (p == '\'' || p == '"')
				quote = p;
			if (p == '/') {
				if (stream.peek() == '*') {
					slice += stream.get();
					comment_multi = true;
				} else if (stream.peek() == '/') {
					while (stream.peek() != '\n') {
						slice += stream.get();
					}
				}
			}
		}
	}
	stream.get();
	slice.shrink_to_fit();
}

void parseTemplate(stringstream& stream, string& slice) {
	slice.clear();
	slice.reserve(64);
	slice += "<";

	int template_nest = 1;
	while (template_nest > 0) {
		char p = stream.get();
		if (p == '<')
			++template_nest;
		if (p == '>')
			--template_nest;

		slice += (char)p;
	}

	slice.shrink_to_fit();
} 

void parsePreproc(stringstream& stream) {
	string slice = parseIdent(stream).slice;
	if (slice == "include") {
		while (stream.peek() == ' ') {
			stream.get();
		}

		if (stream.get() != '<') {
			cacerr("Invalid include\n");
		}
		stringstream a;
		while (stream.peek() != '>') {
			a << (char)stream.get();
		}
		stream.get();

		string fname(a.str());

		FILE* fptr = fopen(fname.c_str(), "r");

		if (!fptr) {
			cout << fname << endl;
			cacerr("Could not include file %s\n", fname.c_str());
		}
		// why c file reading

		fseek(fptr, 0, SEEK_END);
		long fsize = ftell(fptr);
		fseek(fptr, 0, SEEK_SET);
		char* contents = new char[fsize + 1];
		fread(contents, fsize, 1, fptr);
		fclose(fptr);
		contents[fsize] = 0;

		stream << contents;

		//slice = stream.str();
		//cout << stream.str().erase(0, stream.tellp()) << endl;
	} else {
		cacerr("Invalid preprocessor token %s\n", slice.c_str());
	}
}

vector<Token> lexStream(stringstream& stream) {
	vector<Token> ts;

	while ((stream).peek() != -1) {
		Token t = parseIdent(stream);
		string slice = t.slice;
		if (slice.size() != 0) {
			if (slice == "class") {
				t.type = kClass;
				ts.push_back(t);
				continue;
			}

			if (slice == "virtual") {
				t.type = kVirtual;
				ts.push_back(t);
				continue;
			}

			if (slice == "const") {
				t.type = kConst;
				ts.push_back(t);
				continue;
			}

			if (slice == "static") {
				t.type = kStatic;
				ts.push_back(t);
				continue;
			}

			if (slice == "inline") {
				t.type = kInlineExpr;
				parseInline(stream, t.slice);
				// insane
				ts.push_back(t);
				continue;
			}

			if (slice.rfind("0x", 0) == 0) {
				t.type = kAddress;
				char* startpos = new char[slice.size()];
				strncpy(startpos, slice.c_str(), slice.size());
				char* endpos = startpos+slice.size();
				strtoul(startpos, &endpos, 16);
				if (errno != 0) {
					cacerr("Invalid hex %s\n", slice.c_str());
				}
				delete[] startpos;
			}

			ts.push_back(t);
		} else {
			TokenType sp = kIdent;
			char b = (stream).peek();
			string slice = std::string(&b, 1);


			switch (stream.get()) {
				case '=':
					sp = kEqual;
					break;
				case '{':
					sp = kBraceL;
					break;
				case '}':
					sp = kBraceR;
					break;
				case '(':
					sp = kParenL;
					break;
				case ')':
					sp = kParenR;
					break;
				case '[': 
					sp = kBrackL;
					if (stream.peek() == '[') {
						stream.get();
						sp = kAttrL;
					}
					break;
				case ']':
					sp = kBrackR;
					if (stream.peek() == ']') {
						stream.get();
						sp = kAttrR;
					}
					break;
				case ';':
					sp = kSemi;
					break;
				case '*':
					sp = kStar;
					break;
				case '&':
					sp = kAmp;
					break;
				case '~':
					sp = kDtor;
					break;
				case ',':
					sp = kComma;
					break;

				case '<':
					sp = kTemplateExpr;
					parseTemplate(stream, slice);
					break;
				case ':':
					if ((stream).peek() == ':') {
						stream.get();
						sp = kQualifier;
						slice = "::";
						break;
					} else {
						sp = kColon;
						break;
					}
				case ' ':
					sp = kIgnore;
					break;
				case '#':
					parsePreproc(stream);
					sp = kIgnore;
					break;
				case '\n':
					sp = kIgnore;
					break;
				case '\t':
					sp = kIgnore;
					break;
				case '/':
					if (stream.get() == '/') {
						sp = kIgnore;
						while (stream.peek() != '\n')
							stream.get();
					} else if (stream.get() == '*') {
						sp = kIgnore;
						while (stream) {
							char p = stream.get();
							if (p == '*' && stream.peek() == '/') {
								stream.get();
								break;
							}
						}
					} else {
						cacerr("Invalid token \"%s\"", slice.c_str());
					}
					break;
				case '\'':
					parseString(stream, slice, '\'');
					sp = kString;
					break;
				case '"':
					parseString(stream, slice, '"');
					sp = kString;
					break;
				default:
					cacerr("Invalid token \"%s\"", slice.c_str());
			}

			t.slice = slice;
			t.type = sp;
			if (sp != kIgnore)
				ts.push_back(t);
		}
	}

	return ts;
}
