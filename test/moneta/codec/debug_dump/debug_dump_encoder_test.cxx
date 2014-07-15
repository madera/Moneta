#include "stdafx.h"
#include <moneta/codec/debug_dump/debug_dump_encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

using moneta::codec::debug_dump;

BOOST_AUTO_TEST_CASE(simple_debug_dump_encoder_test) {
	unsigned char buffer[128];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	ThreeInts ints;
	ints.One = 1;
	ints.Two = 22;
	ints.Three = 333;

	moneta::codec::encode<debug_dump>(ints, buffer, buffer + 12);
	
	//BOOST_CHECK(std::equal(data, data + 12, reinterpret_cast<unsigned char*>(&ints)));
}
