#pragma once
#include "members.hxx"
#include <boost/type_traits/integral_constant.hpp>
#include <boost/mpl/not.hpp>

namespace moneta { namespace traits {

	template <class Entity>
	struct is_entity : boost::mpl::not_<
		boost::is_same<typename members<Entity>::type, boost::false_type>
	> {};

}}
