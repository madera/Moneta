#include "stdafx.h"
#include <moneta/traits/pk_tie.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Cat.hxx"

struct Dummy {
};

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

BOOST_AUTO_TEST_CASE(detail_mpl_const_pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::const_pk_tie<Person>::type,
			boost::mpl::vector<const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::mpl::const_pk_tie<Dog>::type,
			boost::mpl::vector<const std::string&, const int&>
		>
	));
}

BOOST_AUTO_TEST_CASE(detail_fusion_pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk_tie<Person>::type,
			boost::fusion::vector<int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk_tie<Dog>::type,
			boost::fusion::vector<std::string&, int&>
		>
	));
}

BOOST_AUTO_TEST_CASE(detail_fusion_const_pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::const_pk_tie<Person>::type,
			boost::fusion::vector<const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::const_pk_tie<Dog>::type,
			boost::fusion::vector<const std::string&, const int&>
		>
	));
}

BOOST_AUTO_TEST_CASE(pk_tie_test) {

	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::pk_tie<int>::type, int&>));
	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::pk_tie<std::string>::type, std::string&>));
	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::pk_tie<Address>::type, int&>));

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

BOOST_AUTO_TEST_CASE(const_pk_tie_test) {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::const_pk_tie<Person>::type,
			const int&
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::const_pk_tie<Dog>::type,
			boost::fusion::vector2<const std::string&, const int&>
		>
	));
}

BOOST_AUTO_TEST_CASE(pk_tie_opfx_test) {
	Address address;
	address.ID = 123;
	address.Number = 10;
	address.Street = "Some St.";

	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::pk_tie<Address>::type, int&>));

	BOOST_CHECK_EQUAL(moneta::traits::pk_tie<Address>()(address), 123);
}

BOOST_AUTO_TEST_CASE(const_pk_tie_opfx_test) {
	Address address;
	address.ID = 123;
	address.Number = 10;
	address.Street = "Some St.";

	const Address& const_address = address;

	BOOST_CHECK_EQUAL(moneta::traits::const_pk_tie<Address>()(const_address), 123);
}
