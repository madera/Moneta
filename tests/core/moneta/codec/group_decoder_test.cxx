#include "stdafx.h"
#include <moneta/codec/rawbin/rawbin_decoder.hxx>
#include <moneta/codec/decoder.hxx>
#include <moneta/codec/group_decoder.hxx>
#include <boost/variant/static_visitor.hpp>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"
#include "../model/Person.hxx"
#include "../model/Address.hxx"
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"

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

BOOST_AUTO_TEST_CASE(simple_group_decoder_test) {
	unsigned char good_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	moneta::codec::group_decoder<
		moneta::codec::rawbin_decoder,
		moneta::traits::entity_group<
			ThreeInts, FourInts
		>
	> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	int result = decoder(std::begin(good_threeint), std::end(good_threeint), visitor);
	BOOST_CHECK_EQUAL(result, 12);
	BOOST_CHECK_EQUAL(oss.str(), "ThreeInts:11111111:22222222:55555555");
}

BOOST_AUTO_TEST_CASE(simple2_group_decoder_test) {
	unsigned char bad_threeint[16] = {
		0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
		0x55, 0x55, 0x55, 0x55, 0x77, 0x77, 0x77, 0x77
	};

	moneta::codec::group_decoder<
		moneta::codec::rawbin_decoder,
		moneta::traits::entity_group<
			ThreeInts, FourInts
		>
	> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	// Should fail to decode a ThreeInts and chain to the FourInts decoder to successfully decode.
	int result = decoder(std::begin(bad_threeint), std::end(bad_threeint), visitor);
	BOOST_CHECK_EQUAL(result, 16);
	BOOST_CHECK_EQUAL(oss.str(), "FourInts:11111111:aaaaaaaa:55555555:77777777");
}

//
//
//

BOOST_AUTO_TEST_CASE(test_moneta_codec_group_decoder_test) {
	using namespace moneta::codec;

	struct codec_enter_entity {};
	struct codec_member {};
	struct codec_leave_entity {};
	struct codec_enter_container {};
	struct codec_container_item {};
	struct codec_leave_container {};

	struct rawbin_prefix_reader {};
	struct prefix_reader {};
	struct prefix_getter {};

	typedef moneta::codec::group_decoder<
		moneta::codec::decoder<
			enter_actions<codec_enter_entity>,
			member_actions<codec_member>,
			leave_actions<codec_leave_entity>,
			enter_container_actions<codec_enter_container>,
			container_item_actions<codec_container_item>,
			leave_container_actions<codec_leave_container>
		>,
		moneta::traits::entity_group<
			Person, Address, Cat
		>
		//with_prefix_reader<
		//	prefix_reader<
		//		rawbin_prefix_reader
		//	>,
		//	prefix_getter<
		//		get_rawbin_entity_prefix
		//	>
		//>
	> decoder_type;

}
