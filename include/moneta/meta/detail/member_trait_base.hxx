#pragma once
#include <boost/type_traits/integral_constant.hpp> // boost::false_type

#define __MONETA_DECLARE_MEMBER_TRAIT(name) \
	namespace moneta { namespace meta { namespace detail { \
		template <class EntityType> \
		struct name : boost::false_type {}; \
	}}}

#define __MONETA_MEMBER_TRAIT_BASE(ns, trait_class, member, type, value) \
	template <> \
	struct ns::trait_class<member> { \
		static type get() { \
			return #value; \
		} \
	};
