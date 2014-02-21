#pragma once
#include "../traits/pure_type.hxx"
#include <boost/type_traits/integral_constant.hpp> // boost::false_type

namespace moneta { namespace meta {
	
	namespace detail {
		template <class EntityType>
		struct members_of : boost::false_type {};
	}

	template <class EntityType>
	struct members_of : detail::members_of<
		typename traits::pure_type<EntityType>::type
	> {};

}}
