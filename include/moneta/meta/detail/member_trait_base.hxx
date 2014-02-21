#pragma once
#include <boost/type_traits/integral_constant.hpp> // boost::false_type

// XXX: Re-evaluate the need for boost::false_type
#define MONETA_DECLARE_MEMBER_TRAIT(name, type) \
	namespace moneta { namespace meta { namespace detail { \
		template <class EntityType> \
		struct name : boost::false_type { \
			typedef type trait_type; \
		}; \
	}}}

// FIXME: Do we need this type here?
#define MONETA_MEMBER_TRAIT_BASE(trait_class, member, type, value) \
	template <> \
	struct moneta::meta::detail::trait_class<member> { \
		typedef type trait_type; \
		\
		static trait_type get() { \
			return #value; \
		} \
	};
