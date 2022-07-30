#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

namespace broma {
	struct comment : 
		disable<sor<
			seq<ascii::string<'/', '/'>, until<eolf>>,
			seq<ascii::string<'/', '*'>, until<seq<ascii::string<'*', '/'>>>>
		>> {};

	struct ignore : sor<comment, one<'\n', '\t', '\r', ' '>> {};

	struct sep : star<ignore> {};
	struct whitespace : plus<ignore> {};

	template <typename ...Args>
	struct pad_space : seq<Args..., whitespace> {};

	template <typename ...Args>
	struct rpad_space : seq<whitespace, Args...> {};

	template <typename R, typename ...S>
	struct if_then_must : if_then_else<R, must<S...>, seq<S...>> {};

	template <char Quote>
	struct basic_literal : if_must<one<Quote>, until<one<Quote>>> {};
	struct string_literal : sor<basic_literal<'"'>, basic_literal<'\''>> {};

	struct brace_end;
	struct brace_start : seq<one<'{'>, brace_end> {};
	struct brace_end : until<one<'}'>, sor<string_literal, brace_start, any>> {};

	struct template_end;
	struct template_start : seq<one<'<'>, template_end> {};
	struct template_end : until<one<'>'>, sor<string_literal, template_start, any>> {};

	template <typename T, typename ...Args>
	struct tagged_rule : seq<Args...> {};

	template <typename T>
	struct rule_begin : success {};

	#define $named_rule(name, ...) tagged_rule<TAO_PEGTL_INTERNAL_STRING(ascii::string, name), __VA_ARGS__>

	#define $keyword(name) struct keyword_##name : TAO_PEGTL_KEYWORD(#name) {}

	$keyword(const);
	$keyword(static);
	$keyword(virtual);
	$keyword(inline);
	$keyword(class);
	$keyword(struct);
	$keyword(unsigned);
	$keyword(mac);
	$keyword(win);
	$keyword(ios);
	$keyword(android);
	$keyword(PAD);

	struct qualified : list<seq<identifier, opt<template_start>>, ascii::string<':', ':'>>  {};

	// point of no return: '0x'
	struct hex : if_must<ascii::string<'0', 'x'>, plus<ascii::xdigit>> {};

	struct platform : sor<keyword_mac, keyword_win, keyword_ios, keyword_android> {};
} // namespace broma
