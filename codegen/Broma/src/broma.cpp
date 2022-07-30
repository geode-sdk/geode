#include <tao/pegtl.hpp>
#include <iostream>

#include "attribute.hpp"
#include "basic_components.hpp"
#include "function.hpp"
#include "member.hpp"
#include "state.hpp"
#include "post_process.hpp"
#include "preprocessor.hpp"

using namespace tao::pegtl;

namespace broma {
	struct inline_expr : 
		if_must<
			keyword_inline,
			if_then_else<
				at<rematch<until<eolf>, until<one<'{'>>>>,
				until<brace_start>,
				until<eolf>
			>
		> {};

	template <>
	struct run_action<inline_expr> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			InlineField inf;
			inf.inner = input.string();

			scratch->wip_field.inner = inf;
		}
	};

	struct field : sor<inline_expr, pad_expr, member_expr, bind_expr, ool_expr> {};

	template <>
	struct run_action<field> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_field.parent = scratch->wip_class.name;
			static size_t index = 0;
			scratch->wip_field.field_id = index++;
			scratch->wip_class.fields.push_back(scratch->wip_field);
		}
	};


	struct class_statement : 
		seq<
			opt<attribute>,
			sep,
			must<keyword_class>,
			whitespace,
			must<$named_rule("class name", qualified)>,
			sep,
			opt_must<
				one<':'>,
				sep,
				list<seq<
					sep,
					$named_rule("superclass", qualified),
					sep
				>, one<','>>
			>,
			one<'{'>,
			sep,
			until<one<'}'>, sep, must<field>, sep>
		> {};

	template <>
	struct run_action<$named_rule("superclass", qualified)> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			if (scratch->wip_class.name == input.string())
				throw parse_error("Class subclasses itself", input.position());

			scratch->wip_class.superclasses.push_back(input.string());
			scratch->wip_class.depends.push_back(input.string());
		}
	};

	template <>
	struct run_action<$named_rule("class name", qualified)> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_class.name = input.string();

			if (std::find(root->classes.begin(), root->classes.end(), input.string()) != root->classes.end()) {
				scratch->errors.push_back(parse_error("Class duplicate!", input.position()));
			}
		}
	};


	template <>
	struct run_action<class_statement> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			root->classes.push_back(std::move(scratch->wip_class));
			//std::cout << "class end\n";
			scratch->wip_class = Class();
		}
	};

	struct root_grammar : until<eof, sep, must<sor<include_expr, class_statement>>, sep> {};

	Root parse_file(std::string const& fname) {
		file_input<> input(fname);

		Root root;
		ScratchData scratch;
		parse<must<root_grammar>, run_action>(input, &root, &scratch);
		post_process(root);

		if (scratch.errors.size()) {
			std::cerr << "[Broma] errors found on parsing: \n";

			for (auto& e : scratch.errors) {
				std::cerr << "\t" << e.what() << "\n";
			}

			//abort();
		}

		return root;
	}
} // namespace broma
