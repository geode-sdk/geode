#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

#include "basic_components.hpp"
#include "attribute.hpp"
#include "type.hpp"
#include "bind.hpp"
#include "state.hpp"

namespace broma {
	struct function_begin :
		seq<rule_begin<function_begin>, opt<attribute>, sep, sor<
			// ctor, dtor
			seq<
				$named_rule("structor", success),
				opt<pad_space<keyword_virtual>>,
				opt<tagged_rule<function_begin, one<'~'>>>,
				tagged_rule<function_begin, identifier>,
				arg_list
			>,

			// not ctor, dtor
			if_then_must<
				sor<keyword_static, keyword_virtual>,
				$named_rule("fn_normal", success),
				sep,
				tagged_rule<function_begin, type>,
				sep,
				tagged_rule<function_begin, identifier>, 
				arg_list,
				sep,
				opt<tagged_rule<function_begin, keyword_const>>
			>
		>> {};

	template <>
	struct run_action<rule_begin<function_begin>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin = FunctionBegin();
		}
	};

	template <>
	struct run_action<function_begin> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			for (auto& f : scratch->wip_class.fields) {
				if (auto fn = f.get_fn()) {
					if (*fn == scratch->wip_fn_begin) {
						scratch->errors.push_back(parse_error("Function duplicate!", input.position()));
					}
				}
			}
		}
	};

	template <>
	struct run_action<tagged_rule<function_begin, identifier>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.name = input.string();

			if (scratch->wip_fn_begin.type == FunctionType::Dtor)
				scratch->wip_fn_begin.name = "~" + scratch->wip_fn_begin.name;
		}
	};

	template <>
	struct run_action<tagged_rule<function_begin, keyword_const>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.is_const = true;
		}
	};

	template <>
	struct run_action<tagged_rule<function_begin, type>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.ret = scratch->wip_type;
		}
	};

	template <>
	struct run_action<$named_rule("fn_normal", success)> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.type = FunctionType::Normal;
		}
	};

	template <>
	struct run_action<$named_rule("structor", success)> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.type = FunctionType::Ctor;
		}
	};

	template <>
	struct run_action<keyword_static> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.is_static = true;
		}
	};

	template <>
	struct run_action<tagged_rule<function_begin, one<'~'>>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.type = FunctionType::Dtor;
		}
	};

	template <>
	struct run_action<keyword_virtual> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_fn_begin.is_virtual = true;
		}
	};

	struct ool_expr : seq<function_begin, sep, tagged_rule<ool_expr, brace_start>> {};

	template <>
	struct run_action<tagged_rule<ool_expr, brace_start>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			OutOfLineField f;
			f.beginning = scratch->wip_fn_begin;
			f.inner = input.string();
			scratch->wip_field.inner = f;
		}
	};

	struct bind_expr : seq<function_begin, sep, bind> {};

	template <>
	struct run_action<bind_expr> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			FunctionBindField f;
			f.beginning = scratch->wip_fn_begin;
			f.binds = scratch->wip_bind;
			scratch->wip_field.inner = f;
		}
	};
} // namespace broma