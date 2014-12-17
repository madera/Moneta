#include "stdafx.h"
#include <moneta/traits/pk_tie.hxx>
#include <boost/mpl/equal.hpp>
#include <moneta/pp/core.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Cat.hxx"

struct Dummy {
};

static void static_test() {
	using boost::is_same;
	using boost::mpl::equal;
	using moneta::traits::pk;
	using moneta::traits::has_pk;
	using moneta::traits::detail::is_pk;

	namespace fusion = boost::fusion;
	namespace mpl = boost::mpl;



	//
	// detail_mpl_pk_tie
	//

	// Person
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::mpl::pk_tie<Person>::type,
			mpl::vector<int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::mpl::pk_tie<Dog>::type,
			mpl::vector<std::string&, int&>
		>
	));

	// Person
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::mpl::pk_tie<const Person>::type,
			mpl::vector<const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::mpl::pk_tie<const Dog>::type,
			mpl::vector<const std::string&, const int&>
		>
	));



	//
	// detail_fusion_pk_tie
	//

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



	//
	// detail_fusion_pk_tie of const
	//

	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk_tie<const Person>::type,
			boost::fusion::vector<const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::detail::fusion::pk_tie<const Dog>::type,
			boost::fusion::vector<const std::string&, const int&>
		>
	));

	//
	// pk_tie
	//

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

	//
	// pk_tie of const
	//

	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_tie<const Person>::type,
			const int&
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::pk_tie<const Dog>::type,
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

BOOST_AUTO_TEST_CASE(pk_tie_of_const_opfx_test) {
	Address address;
	address.ID = 123;
	address.Number = 10;
	address.Street = "Some St.";

	const Address& const_address = address;

	BOOST_CHECK_EQUAL(moneta::traits::pk_tie<const Address>()(const_address), 123);
}
