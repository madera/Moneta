#include "stdafx.h"
#include <moneta/codec/debug_dump/debug_dump_encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/Cat.hxx"

using moneta::codec::debug_dump;

BOOST_AUTO_TEST_CASE(simple_debug_dump_encoder_test) {
	unsigned char buffer[128];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	Cat garfield;
	garfield.ID = 1;
	garfield.Name = "Garfield";
	garfield.Address.ID = 10;
	garfield.Address.Number = 123;
	garfield.Address.Street = "Super Street";

	moneta::codec::encode<debug_dump>(garfield, buffer, buffer + 12);
	
	//BOOST_CHECK(std::equal(data, data + 12, reinterpret_cast<unsigned char*>(&ints)));
}
