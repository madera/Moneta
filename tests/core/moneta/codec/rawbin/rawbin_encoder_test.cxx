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
#include <moneta/codec/rawbin/rawbin_encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

BOOST_AUTO_TEST_CASE(simple_rawbin_encoder_test) {
	unsigned char buffer[12];
	std::fill(std::begin(buffer), std::end(buffer), 0x55);

	const unsigned char expected[12] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33
	};
	
	ThreeInts ints;
	ints.One = 0x11111111;
	ints.Two = 0x22222222;
	ints.Three = 0x33333333;

	int result = moneta::codec::rawbin_encoder()(std::begin(buffer), std::end(buffer), ints);
	BOOST_CHECK_EQUAL(result, 12);
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(buffer), std::end(buffer), std::begin(expected), std::end(expected));
}

BOOST_AUTO_TEST_CASE(simple2_rawbin_encoder_test) {
	unsigned char buffer[16];
	std::fill(std::begin(buffer), std::end(buffer), 0x55);

	const unsigned char expected[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44
	};

	FourInts ints;
	ints.One   = 0x11111111;
	ints.Two   = 0x22222222;
	ints.Three = 0x33333333;
	ints.Four  = 0x44444444;

	moneta::codec::rawbin_encoder()(std::begin(buffer), std::end(buffer), ints);
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(buffer), std::end(buffer), std::begin(expected), std::end(expected));
}
