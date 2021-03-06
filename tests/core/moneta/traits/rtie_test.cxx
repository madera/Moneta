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
#include <moneta/traits/rtie.hxx>
#include <boost/mpl/equal.hpp>

#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

inline void static_test() {
	// Person
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<Person>::type,
			boost::mpl::vector<int&, std::string&, double&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<const Person>::type,
			boost::mpl::vector<const int&, const std::string&, const double&, const int&>
		>
	));

	// Cat
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<Cat>::type,
			boost::mpl::vector<int&, std::string&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<const Cat>::type,
			boost::mpl::vector<const int&, const std::string&, const int&>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<Dog>::type,
			boost::mpl::vector3<std::string&, int&, std::string&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<const Dog>::type,
			boost::mpl::vector3<const std::string&, const int&, const std::string&>
		>
	));

	// Composite
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<Composite>::type,
			boost::fusion::vector3<int&, int&, boost::fusion::vector2<std::string&, int&> >
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<const Composite>::type,
			boost::fusion::vector3<const int&, const int&,
				boost::fusion::vector2<const std::string&, const int&> >
		>
	));
}

BOOST_AUTO_TEST_CASE(rtie_test) {
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::traits::rtie<Person>::type,
			boost::mpl::vector4<int&, std::string&, double&, int&>
		>
	));
}
