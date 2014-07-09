#include "stdafx.h"
#include <moneta/codec/debug_dump/debug_dump_encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

BOOST_AUTO_TEST_CASE(simple_debug_dump_encoder_test) {
	unsigned char buffer[128];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	ThreeInts ints;
	ints.One = 0x11111111;
	ints.Two = 0x22222222;
	ints.Three = 0x33333333;

	moneta::codec::encode<moneta::codec::debug_dump>(ints, buffer, buffer + 12);
	
	//BOOST_CHECK(std::equal(data, data + 12, reinterpret_cast<unsigned char*>(&ints)));
}
