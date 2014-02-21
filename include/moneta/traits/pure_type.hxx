#pragma once
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace moneta { namespace traits {

	template <typename T>
	struct pure_type : boost::remove_const<
		typename boost::remove_reference<T>::type
	> {};

}}
