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
#include <moneta/traits/rtuple.hxx>
#include <boost/mpl/equal.hpp>

#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

inline void static_test() {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<const Person>::type,
			boost::mpl::vector<const int, const std::string, const double, const int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Cat>::type,
			boost::mpl::vector<int, std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<const Cat>::type,
			boost::mpl::vector<const int, const std::string, const int>
		>
	));
}

BOOST_AUTO_TEST_CASE(rtuple_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Person>::type,
			boost::mpl::vector<int, std::string, double, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Dog>::type,
			boost::mpl::vector<std::string, int, std::string>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Composite>::type,
			boost::fusion::vector3<int, int, boost::fusion::vector2<std::string, int> >
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtuple<Cat>::type,
			boost::fusion::vector3<int, std::string, int>
		>
	));
}
