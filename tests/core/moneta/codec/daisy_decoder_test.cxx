#include "stdafx.h"
#include <moneta/codec/rawbin/rawbin_decoder.hxx>
#include <moneta/codec/daisy_decoder.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"
#include <boost/variant/static_visitor.hpp>

struct entity_visitor : boost::static_visitor<void> {
	std::ostringstream& _output;

	entity_visitor(std::ostringstream& output)
	 : _output(output) {
	}

	void operator()(const ThreeInts& entity) const {
		_output << "ThreeInts:"
			<< std::hex << entity.One << ':'
			<< std::hex << entity.Two << ':'
			<< std::hex << entity.Three
		;
	}

	void operator()(const FourInts& entity) const {
		_output << "FourInts:"
			<< std::hex << entity.One   << ':'
			<< std::hex << entity.Two   << ':'
			<< std::hex << entity.Three << ':'
			<< std::hex << entity.Four
		;
	}
};

BOOST_AUTO_TEST_CASE(simple_daisy_decoder_test) {
	using moneta::codec::daisy_decoder;

	unsigned char good_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	daisy_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	int result = decoder(std::begin(good_threeint), std::end(good_threeint), visitor);
	BOOST_CHECK_EQUAL(result, 12);
	BOOST_CHECK_EQUAL(oss.str(), "ThreeInts:11111111:22222222:55555555");
}

BOOST_AUTO_TEST_CASE(simple2_daisy_decoder_test) {
	using moneta::codec::daisy_decoder;

	unsigned char bad_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
		0x55, 0x55, 0x55, 0x55, 0x77, 0x77, 0x77, 0x77
	};

	daisy_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	// Should fail to decode a ThreeInts and chain to the FourInts decoder to successfully decode.
	int result = decoder(std::begin(bad_threeint), std::end(bad_threeint), visitor);
	BOOST_CHECK_EQUAL(result, 16);
	BOOST_CHECK_EQUAL(oss.str(), "FourInts:11111111:aaaaaaaa:55555555:77777777");
}
