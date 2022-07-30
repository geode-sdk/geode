#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

#include "basic_components.hpp"
#include "state.hpp"

namespace broma {
	struct type_content : 
		if_then_must<
			pad_space<sor<keyword_const, keyword_struct>>,

			sor<pad_space<keyword_const>, pad_space<keyword_struct>, success>,

			if_then_else<pad_space<keyword_unsigned>, opt<qualified>, qualified>,

			opt<rpad_space<keyword_const>>,
			star<seq<sep, one<'&', '*'>>>
		> {};

	struct type : seq<rule_begin<type>, sep, type_content> {};

	template <>
	struct run_action<rule_begin<type>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_type = Type();
		}
	};

	template <>
	struct run_action<keyword_struct> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_type.is_struct = true;
		}
	};

	template <>
	struct run_action<type_content> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_type.name = input.string();
		}
	};

	struct array : if_must<one<'['>, tagged_rule<array, plus<ascii::digit>>, one<']'>> {};

	template <>
	struct run_action<tagged_rule<array, plus<ascii::digit>>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_field.get_as<MemberField>()->count = std::stoul(input.string(), nullptr, 10);
		}
	};

	struct arg_name : opt<identifier> {};
	struct arg_list : 
		seq<
			rule_begin<arg_list>,
			one<'('>,
			if_then_else<
				at<one<')'>>,
				success,
				list<seq<
					sep,
					tagged_rule<arg_list, type>,
					sep,
					arg_name,
					sep
				>, one<','>>
			>,
			one<')'>
		> {};

	template <>
	struct run_action<rule_begin<arg_list>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.args.clear();
		}
	};

	template <>
	struct run_action<tagged_rule<arg_list, type>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.args.push_back({scratch->wip_type, ""});
		}
	};

	template <>
	struct run_action<arg_name> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			if (input.string() == "")
				scratch->wip_fn_begin.args.back().second = std::string("p") + std::to_string(scratch->wip_fn_begin.args.size() - 1);
			else
				scratch->wip_fn_begin.args.back().second = input.string();
		}
	};
} // namespace broma
