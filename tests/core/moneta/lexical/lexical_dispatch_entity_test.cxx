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
#include <moneta/make_entity.hxx>
#include <moneta/lexical/dispatch_entity.hxx>
#include <sstream>
#include "../model/Person.hxx"
#include "../model/Composite.hxx"
#include "../model/Cat.hxx"

struct test_operation {
	std::ostringstream& _oss;

	test_operation(std::ostringstream& oss)
	 : _oss(oss) {}

	template <class Entity>
	void operator()(Entity*) const {
		_oss << moneta::traits::entity_name<Entity>::get();
	}
};

BOOST_AUTO_TEST_CASE(dispatch_entity_test) {
	
	typedef boost::mpl::vector5<Person, Address, Cat, Composite, Dog> entities;

	std::ostringstream oss;

	moneta::lexical::dispatch_entity<entities>("Cat",       test_operation(oss));
	moneta::lexical::dispatch_entity<entities>("Address",   test_operation(oss));
	moneta::lexical::dispatch_entity<entities>("Person",    test_operation(oss));
	moneta::lexical::dispatch_entity<entities>("Dog",       test_operation(oss));
	moneta::lexical::dispatch_entity<entities>("Composite", test_operation(oss));

	BOOST_CHECK_EQUAL(oss.str(), "CatAddressPersonDogComposite");
}
