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
#include <moneta/codec/shell/shell_encoder.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/tree/A.hxx"
#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/make_entity.hxx>

static Composite make_composite() {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Ratings = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";
	return composite;
}

BOOST_AUTO_TEST_CASE(shell_encoder_test_1) {	
	char buffer[1024];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	const std::string expected =
		"Composite={ Identifier=2600 Person={ ID=123 Name=\"Somedude\" Height=1.5 Ratings=10 } "
		"Dog={ Owner=\"Someowner\" ID=555 Name=\"Doggy\" } }"
	;

	int result = moneta::codec::shell_encoder()(std::begin(buffer), std::end(buffer), make_composite());
	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}


BOOST_AUTO_TEST_CASE(shell_encoder_test_2) {
	char buffer[1024];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	const std::string expected = "A={ f=0 g=0 B={ C={ j=0 k=0 } i=0 D={ l=0 E={ m=0 n=0 } } } h=0 }";

	int result = moneta::codec::shell_encoder()(std::begin(buffer), std::end(buffer), moneta::make_entity<A>());
	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}
