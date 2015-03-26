#include "stdafx.h"
#include <moneta/traits/trait_array.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"

inline void static_test() {

	BOOST_MPL_ASSERT((boost::is_same<
		moneta::traits::member_name<
			boost::mpl::front<
				moneta::traits::members<Person>::type
			>::type
		>::trait_type,
		std::string
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		moneta::traits::member_name<Person>::trait_type,
		std::string
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		moneta::traits::trait_array<moneta::traits::member_name, Person>::type,
		boost::array<std::string, 4>
	>));

}

BOOST_AUTO_TEST_CASE(test_moneta_traits_trait_array_test) {
	moneta::traits::trait_array<moneta::traits::member_name, Person>::type traits =
	moneta::traits::collect_member_traits<moneta::traits::member_name, Person>();

	const char* expected[] = { "ID", "Name", "Height", "Ratings" };
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(traits), std::end(traits), std::begin(expected), std::end(expected));
}
