#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

#include "type.hpp"
#include "bind.hpp"
#include "state.hpp"

namespace broma {
	struct member_expr : 
		seq<
			rule_begin<member_expr>,
			type,
			whitespace,
			tagged_rule<member_expr, identifier>,
			sep,
			opt<array>,
			sep,
			one<';'>
		> {};

	template <>
	struct run_action<rule_begin<member_expr>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			MemberField f;
			scratch->wip_field.inner = f;
		}
	};

	template <>
	struct run_action<tagged_rule<member_expr, identifier>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_field.get_as<MemberField>()->name = input.string();
		}
	};

	template <>
	struct run_action<member_expr> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_field.get_as<MemberField>()->type = scratch->wip_type;
		}
	};

	struct pad_expr : seq<keyword_PAD, sep, bind> {};

	template <>
	struct run_action<keyword_PAD> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			PadField f;
			PlatformNumber p;
			scratch->wip_field.inner = f;
			scratch->wip_bind = p;
		}
	};

	template <>
	struct run_action<pad_expr> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_field.get_as<PadField>()->amount = scratch->wip_bind;
		}
	};
} // namespace broma
