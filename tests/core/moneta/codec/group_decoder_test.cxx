#include "stdafx.h"
#include <moneta/codec/rawbin/rawbin_decoder.hxx>
#include <moneta/codec/group_decoder.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"

#include <boost/variant/static_visitor.hpp>

struct printer : boost::static_visitor<int> {
	int operator()(const ThreeInts& entity) const {
		return 3;
	}

	int operator()(const FourInts& entity) const {
		return 4;
	}
};

BOOST_AUTO_TEST_CASE(simple_group_decoder_test) {
	using moneta::codec::group_decoder;

	unsigned char good_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	group_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts> decoder;
	group_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts>::variant_type entity;

	int result = decoder(entity, good_threeint, good_threeint + 16);
	BOOST_CHECK_EQUAL(result, 12);

	const int type_ordinal = boost::apply_visitor(printer(), entity);
	BOOST_CHECK_EQUAL(type_ordinal, 3);
}

BOOST_AUTO_TEST_CASE(simple2_group_decoder_test) {
	using moneta::codec::group_decoder;

	unsigned char bad_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0xba, 0xd2,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	group_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts> decoder;
	group_decoder<moneta::codec::rawbin_decoder, ThreeInts, FourInts>::variant_type entity;

	int result = decoder(entity, bad_threeint, bad_threeint + 16);
	BOOST_CHECK_EQUAL(result, 16);

	const int type_ordinal = boost::apply_visitor(printer(), entity);
	BOOST_CHECK_EQUAL(type_ordinal, 4);
}
