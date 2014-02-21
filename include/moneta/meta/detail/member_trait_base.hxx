#pragma once

#define __MONETA_MEMBER_TRAIT_BASE(ns, trait_class, member, type, value) \
	template <> \
	struct ns::trait_class<member> { \
		static type get() { \
			return #value; \
		} \
	};
