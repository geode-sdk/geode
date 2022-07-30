#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

#include "basic_components.hpp"
#include "state.hpp"

namespace broma {
	template <typename Name, typename ...Parse>
	struct basic_attribute : seq<
			Name,
			one<'('>,

			if_then_else<
				at<one<')'>>,
				success,
				seq<Parse...>
			>,

			one<')'>
		> {};

	struct docs_attribute : basic_attribute<TAO_PEGTL_KEYWORD("docs"), string_literal> {};
	struct depends_attribute : basic_attribute<TAO_PEGTL_KEYWORD("depends"), tagged_rule<depends_attribute, qualified>> {};

	struct attribute : 
		if_must<ascii::string<'[', '['>, 
			sor<docs_attribute, depends_attribute>,
			ascii::string<']', ']'>
		> {};

	template <>
	struct run_action<tagged_rule<depends_attribute, qualified>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_class.depends.push_back(input.string());
		}
	};
} // namespace broma