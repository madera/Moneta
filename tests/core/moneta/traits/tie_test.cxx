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
#include <moneta/traits/tie.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(tie_types_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	typedef moneta::traits::tie<Person>::type tie_type;
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 0>::type, int&        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 1>::type, std::string&>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 2>::type, double&     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<tie_type, 3>::type, int&        >)); // Ratings

	typedef moneta::traits::tie<const Person>::type const_tie_type;
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 0>::type, const int&        >)); // ID
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 1>::type, const std::string&>)); // Name
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 2>::type, const double&     >)); // Height
	BOOST_MPL_ASSERT((is_same<at_c<const_tie_type, 3>::type, const int&        >)); // Ratings
}
