#pragma once

#include "Shared.hpp"

#include <unordered_set>
#include <vector>
#include <string>
#include <string_view>

std::string mangleIdent(std::string_view str, bool ne = true) {
	if (str.find("::") != -1) {
		std::string result = ne ? "N" : "";
		auto s = str;
		do {
			const auto i = s.find("::");
			const auto t = s.substr(0, i);
			result += std::to_string(t.size()) + std::string(t);
			if (i == -1) s = "";
			else
				s = s.substr(i + 2);
		} while(s.size());
		return result + (ne ? "E" : "");
	} else {
		return std::to_string(str.size()) + std::string(str);
	}
};

std::string intToString(unsigned int value, unsigned int radix) {
    static constexpr char base36[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    std::string result;
    do {
        unsigned int remainder = value % radix;
        value /= radix;
        result.insert(result.begin(), base36[remainder]);
    } while (value);
    return result;
}

std::string lookForSeen(std::vector<std::string>& seen, std::string mangled) {
	for (int i = 0; i < seen.size(); ++i) {
		if (seen[i] == mangled) {
			if (i == 0) return "S_";
			// yes, its base 36
			return "S" + intToString(i - 1, 36) + "_";
		}
	}
	return "";
}

std::string subsSeen(std::vector<std::string>& seen, std::string mangled, bool subs) {
	if (!subs) return mangled;
    if (mangled.empty()) return mangled;
	if (auto x = lookForSeen(seen, mangled); !x.empty()) return x;
	seen.push_back(mangled);
	return mangled;
}

std::string mangleType(std::vector<std::string>& seen, std::string name, bool subs = true) {
	if (name == "int") return "i";
	if (name == "float") return "f";
	if (name == "bool") return "b";
	if (name == "char") return "c";
	if (name == "gd::string") return "Ss";
	if (name == "cocos2d::ccColor3B") return mangleType(seen, "cocos2d::_ccColor3B", subs);
	// too lazy
	if (name == "gd::map<gd::string, gd::string>") return "St3mapISsSsSt4lessISsESaISt4pairIKSsSsEEE";
	if (name == "cocos2d::SEL_MenuHandler") {
		const auto a = mangleType(seen, "cocos2d::CCObject", subs);
		const auto b = mangleType(seen, "cocos2d::CCObject*", subs);
		const auto fnptr = subsSeen(seen, "Fv" + b + "E", subs);
		return subsSeen(seen, "M" + a + fnptr, subs);
	}
	if (name.find('*') == name.size() - 1) {
		auto inner = mangleType(seen, name.substr(0, name.size() - 1), false);
		if (auto x = lookForSeen(seen, "P" + inner); !x.empty()) return x;
		inner = mangleType(seen, name.substr(0, name.size() - 1), subs);
		return subsSeen(seen, "P" + inner, subs);
	}
	if (name.find('&') == name.size() - 1) {
		auto inner = mangleType(seen, name.substr(0, name.size() - 1), false);
		if (auto x = lookForSeen(seen, "R" + inner); !x.empty()) return x;
		inner = mangleType(seen, name.substr(0, name.size() - 1), subs);
		return subsSeen(seen, "R" + inner, subs);
	}
	if (auto i = name.find("const"); i != -1) {
		std::string inner;
		// at the end of the name
		if (i == name.size() - 5) {
			inner = mangleType(seen, name.substr(0, i - 1));
		} else if (i == 0) {
			inner = mangleType(seen, name.substr(6));
		} else {
			inner = "v";
			std::cout << "um " << name << std::endl;
		}
		return subsSeen(seen, "K" + inner, subs);
	}

	if (name.find("::") != -1) {
		std::string result = "";
		std::string substituted = "";
		auto s = name;
		do {
			const auto i = s.find("::");
			const auto t = s.substr(0, i);
            auto part = std::to_string(t.size()) + std::string(t);
		    if (auto x = lookForSeen(seen, result + part); !x.empty()) {
                substituted = x;
            } else {
                substituted = subsSeen(seen, substituted + part, subs);
            }
            result += part;

			if (i == -1) s = "";
			else s = s.substr(i + 2);
		} while(s.size());
        if (substituted.size() == 3 && substituted[0] == 'S')
            return substituted;
		return "N" + substituted + "E";
	} else {
		return subsSeen(seen, mangleIdent(name), subs);
	}
};

std::string generateAndroidSymbol(const Class& clazz, const FunctionBindField* fn) {
	auto& decl = fn->prototype;


	std::string mangledSymbol;
	switch (decl.type) {
		case FunctionType::Ctor:
			mangledSymbol = "_ZN" + mangleIdent(clazz.name, false) + "C2E";
			break;
		case FunctionType::Dtor:
			mangledSymbol = "_ZN" + mangleIdent(clazz.name, false) + "D2E";
			break;
		default:
			mangledSymbol = "_Z" + mangleIdent(clazz.name + "::" + decl.name);
			break;
	}
	if (decl.args.empty()) {
		mangledSymbol += "v";
	} else {
		std::vector<std::string> seen;
		static constexpr auto firstPart = [](std::string_view str, std::string_view sep) {
			return str.substr(0, str.find(sep));
		};
		// this is S_
		seen.push_back(mangleIdent(firstPart(clazz.name, "::")));
		for (auto& [ty, _] : decl.args) {
			mangledSymbol += mangleType(seen, ty.name);
		}
	}
    return mangledSymbol;
}