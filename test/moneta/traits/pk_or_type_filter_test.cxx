#include "stdafx.h"
#include <moneta/traits/pk_or_type_filter.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(pk_or_type_filter_test) {
	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::detail::pk_or_type<int>::type, int>));
	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::detail::pk_or_type<std::string>::type, std::string>));
	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::detail::pk_or_type<Cat>::type, int>));
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::pk_or_type<Dog>::type,
			boost::mpl::vector2<std::string, int>
		>
	));
}
