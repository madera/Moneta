#include "stdafx.h"
#include <moneta/meta/member_names.hxx>

struct Person {
	int ID;
	std::string Name;
	double Height;
};

MONETA_MEMBER_NAME(MONETA_MEMBER(Person, int,         ID    ), ID    )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, std::string, Name  ), Name  )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, double,      Height), Height)



BOOST_AUTO_TEST_CASE(member_name_of_test) {
	BOOST_CHECK_EQUAL((moneta::meta::detail::member_name_of<MONETA_MEMBER(Person, int,         ID    )>::get()), "ID");
	BOOST_CHECK_EQUAL((moneta::meta::detail::member_name_of<MONETA_MEMBER(Person, std::string, Name  )>::get()), "Name");
	BOOST_CHECK_EQUAL((moneta::meta::detail::member_name_of<MONETA_MEMBER(Person, double,      Height)>::get()), "Height");
}

BOOST_AUTO_TEST_CASE(member_names_test) {
	std::vector<std::string> names;
	BOOST_CHECK(names.size() == 3);
	BOOST_CHECK(names[0] == "ID");
	BOOST_CHECK(names[1] == "Name");
	BOOST_CHECK(names[2] == "Height");
}
