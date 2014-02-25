#include "stdafx.h"
#include <moneta/traits/pk.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

#include <boost/mpl/print.hpp>

struct Dummy {
};

BOOST_AUTO_TEST_CASE(pk_type_test) {
	// Person
	BOOST_MPL_ASSERT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int, ID)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, std::string, Name   )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, double,      Height )>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, int,         Fingers)>));

	// Dog
	BOOST_MPL_ASSERT((moneta::traits::detail::is_pk<MONETA_MEMBER(Dog, std::string, Owner)>));
	BOOST_MPL_ASSERT((moneta::traits::detail::is_pk<MONETA_MEMBER(Dog, int, ID)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::detail::is_pk<MONETA_MEMBER(Person, std::string, Name   )>));
}

BOOST_AUTO_TEST_CASE(has_pk_test) {
	BOOST_MPL_ASSERT((moneta::traits::has_pk<Person>));
	BOOST_MPL_ASSERT((moneta::traits::has_pk<Dog>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::has_pk<Dummy>));
}

BOOST_AUTO_TEST_CASE(detail_mpl_pk_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::pk<Person>::type,
			boost::mpl::vector<int>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::pk<Dog>::type,
			boost::mpl::vector<std::string, int>
		>
	));
}

BOOST_AUTO_TEST_CASE(detail_fusion_pk_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk<Person>::type,
			boost::fusion::vector<int>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));
}


BOOST_AUTO_TEST_CASE(pk_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk<Person>::type,
			int
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));
}

BOOST_AUTO_TEST_CASE(detail_mpl_pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::pk_tie<Person>::type,
			boost::mpl::vector<int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::pk_tie<Dog>::type,
			boost::mpl::vector<std::string&, int&>
		>
	));
}

BOOST_AUTO_TEST_CASE(pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_tie<Person>::type,
			int&
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_tie<Dog>::type,
			boost::fusion::vector2<std::string&, int&>
		>
	));
}
