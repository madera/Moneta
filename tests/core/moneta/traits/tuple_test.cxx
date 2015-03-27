// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/traits/tuple.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Cat.hxx"

inline void static_test() {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::tuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	// const Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::tuple<const Person>::type,
			boost::mpl::vector<const int, const std::string, const double, const int>
		>
	));



	// Cat
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::tuple<Cat>::type,
			boost::mpl::vector<int, std::string, Address>
		>
	));

	// const Cat
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::tuple<const Cat>::type,
			boost::mpl::vector<const int, const std::string, const Address>
		>
	));
}

BOOST_AUTO_TEST_CASE(tuple_types_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	typedef moneta::traits::detail::mpl::vector<Person>::type mpl_vector_type;
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<mpl_vector_type, 3>::type, int        >)); // Ratings

	typedef moneta::traits::tuple<Person>::type tuple_type;
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 0>::type, int        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 1>::type, std::string>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 2>::type, double     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<tuple_type, 3>::type, int        >)); // Ratings
}
