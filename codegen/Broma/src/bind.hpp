#pragma once

#include <tao/pegtl.hpp>
using namespace tao::pegtl;

#include "basic_components.hpp"
#include "state.hpp"
#include <sstream>

namespace broma {
	struct bind :
		seq<rule_begin<bind>, opt_must<
			one<'='>,
			sep, 
			list<opt<
				sep,
				sor<keyword_mac, keyword_win, keyword_ios, keyword_android>,
				sep,
				tagged_rule<bind, hex>
			>, one<','>>,
			sep
		>, one<';'>> {};

	template <>
	struct run_action<rule_begin<bind>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_bind = PlatformNumber();
		}
	};

	template <>
	struct run_action<tagged_rule<bind, hex>> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			size_t out = std::stoul(input.string(), nullptr, 16);

			switch (scratch->wip_bind_platform) {
				case Platform::Mac:
					scratch->wip_bind.mac = out;
					break;
				case Platform::iOS:
					scratch->wip_bind.ios = out;
					break;
				case Platform::Windows:
					scratch->wip_bind.win = out;
					break;
				case Platform::Android:
					scratch->wip_bind.android = out;
					break;
			}
		}
	};

	template <>
	struct run_action<keyword_mac> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_bind_platform = Platform::Mac;
		}
	};
	template <>
	struct run_action<keyword_ios> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_bind_platform = Platform::iOS;
		}
	};
	template <>
	struct run_action<keyword_win> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_bind_platform = Platform::Windows;
		}
	};
	template <>
	struct run_action<keyword_android> {
		template <typename T>
		static void apply(T& input, Root* root, ScratchData* scratch) {
			scratch->wip_bind_platform = Platform::Android;
		}
	};
} // namespace broma