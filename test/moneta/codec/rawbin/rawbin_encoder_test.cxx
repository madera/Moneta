#include "stdafx.h"
#include <moneta/codec/rawbin/rawbin_encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

BOOST_AUTO_TEST_CASE(simple_rawbin_encoder_test) {
	unsigned char buffer[12];
	std::fill(buffer, buffer + 12, 0x55);

	ThreeInts ints;
	ints.One = 0x11111111;
	ints.Two = 0x22222222;
	ints.Three = 0x33333333;

	moneta::codec::encode<moneta::codec::rawbin>(ints, buffer, buffer + 12);

	unsigned char data[12] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33
	};
	
	BOOST_CHECK(std::equal(data, data + 12, reinterpret_cast<unsigned char*>(&ints)));
}

BOOST_AUTO_TEST_CASE(simple2_rawbin_encoder_test) {
	unsigned char buffer[128];
	std::fill(buffer, buffer + 128, 0x55);

	FourInts ints;
	ints.One   = 0x11111111;
	ints.Two   = 0x22222222;
	ints.Three = 0x33333333;
	ints.Four  = 0x44444444;

	moneta::codec::encode<moneta::codec::rawbin>(ints, buffer, buffer + 128);

	unsigned char data[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44
	};

	BOOST_CHECK(std::equal(data, data + 16, reinterpret_cast<unsigned char*>(&ints)));
}
