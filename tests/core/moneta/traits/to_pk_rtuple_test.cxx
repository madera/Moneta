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
#include <moneta/traits/to_pk_rtuple.hxx>
#include <moneta/traits/rtuple.hxx>
#include <moneta/traits/to_rtuple.hxx>
#include <boost/mpl/vector_c.hpp>
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(to_pk_rtuple_test) {
	Cat felix;
	felix.ID = 1;
	felix.Name = "Felix the cat";
	felix.Address.ID = 555;

	typedef moneta::traits::rtuple<Cat>::type rtuple_t;
	rtuple_t rtuple = moneta::traits::to_rtuple<Cat>(felix);

	int pk = moneta::traits::to_pk_rtuple<Cat>(rtuple);
	// TODO: Test pk's value.
}

BOOST_AUTO_TEST_CASE(const_to_pk_rtuple_test) {
	Cat felix;
	felix.ID = 1;
	felix.Name = "Felix the cat";
	felix.Address.ID = 555;

	typedef moneta::traits::rtuple<const Cat>::type rtuple_t;
	const rtuple_t rtuple = moneta::traits::to_rtuple<Cat>(felix);

	int pk = moneta::traits::to_pk_rtuple<Cat>(rtuple);
	// TODO: Test pk's value.
}

BOOST_AUTO_TEST_CASE(to_pk_rtuple_test_2) {
	Dog snoopy;
	snoopy.Owner = "Charlie";
	snoopy.ID = 1;
	snoopy.Name = "Snoopy";

	typedef moneta::traits::rtuple<Dog>::type rtuple_t;
	rtuple_t rtuple = moneta::traits::to_rtuple(snoopy);

	// FIXME: Use proper type.
	/*auto pk = */ moneta::traits::to_pk_rtuple<Dog>(rtuple);
}
