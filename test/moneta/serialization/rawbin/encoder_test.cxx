#include "stdafx.h"
#include <moneta/serialization/rawbin/encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"

BOOST_AUTO_TEST_CASE(default_encoder_test) {
	ThreeInts ints;
	ints.One   = 0x11111111;
	ints.Two   = 0x22222222;
	ints.Three = 0x33333333;

	unsigned char buffer[128];
	std::fill(buffer, buffer + 128, 0x55);

	moneta::serialization::rawbin::encode(ints, buffer, buffer + 128);

	unsigned char data[12] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33
	};

	BOOST_CHECK(memcmp(data, (void*)&ints, 12) == 0);
}

BOOST_AUTO_TEST_CASE(custom_encoder_test) {
	unsigned char buffer[1024];
	std::fill(buffer, buffer + 1024, 0);
}
