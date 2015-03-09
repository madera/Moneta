#include "stdafx.h"
#include <moneta/codec/group_decoder.hxx>
#include <moneta/codec/rawbin/rawbin_decoder.hxx>
#include <moneta/codec/decode_many.hxx>
#include <boost/variant/static_visitor.hpp>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"
#include "../model/Person.hxx"
#include "../model/Address.hxx"
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"

MONETA_DEFINE_ENTITY(
	FiveInts,
	((int, One  ))
	((int, Two  ))
	((int, Three))
	((int, Four ))
	((int, Five ))
)

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

	void operator()(const FiveInts& entity) const {
		_output << "FiveInts:"
			<< std::hex << entity.One   << ':'
			<< std::hex << entity.Two   << ':'
			<< std::hex << entity.Three << ':'
			<< std::hex << entity.Four  << ':'
			<< std::hex << entity.Five
		;
	}
};

BOOST_AUTO_TEST_CASE(test_moneta_codec_group_decoder) {
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

BOOST_AUTO_TEST_CASE(test_moneta_codec_group_decoder_with_bad_fixed_values) {
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

struct example_prefix {
	typedef char type;
	
	template <class Iterator>
	int operator()(Iterator begin, Iterator end, type& prefix) const {
		if (begin == end) {
			return -1;
		}

		prefix = *begin;
		return 1;
	}
};

struct acme_enter_entity {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Path) const {
		if (begin == end) {
			return -1;
		}

		//const char ignored_entity_id = *begin++;

		return 1;
	}
};

struct acme_member_decoder {
	template <class Iterator, class Entity, class Member, class Path>
	int operator()(Iterator begin, Iterator end, Entity& entity, Member, Path) const {
		typedef typename Member::result_type value_type;
		const size_t value_size = sizeof(value_type);
		const size_t available = std::distance(begin, end);
		if (available < value_size) {
			return available - value_size;
		}
			
		Member()(entity) = *((value_type*)begin);
		return value_size;
	}
};

MONETA_CODEC_PREFIX_VALUE(example_prefix, ThreeInts, 0x30)
MONETA_CODEC_PREFIX_VALUE(example_prefix, FourInts,  0x40)
MONETA_CODEC_PREFIX_VALUE(example_prefix, FiveInts,  0x50)

BOOST_AUTO_TEST_CASE(test_moneta_codec_group_decoder_prefixed_test) {
	unsigned char buffer[] = {
		0x30,
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55,
		0x40,
		0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
		0x55, 0x55, 0x55, 0x55, 0x77, 0x77, 0x77, 0x77
	};

	typedef moneta::codec::group_decoder<
		moneta::codec::decoder<
			moneta::codec::enter_actions<acme_enter_entity>,
			moneta::codec::member_actions<acme_member_decoder>
		>,
		moneta::traits::entity_group<
			FourInts, ThreeInts
		>,
		example_prefix
	> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	int result = decoder()(std::begin(buffer), std::end(buffer), visitor);

	BOOST_CHECK_EQUAL(result, 1 + 12);
	BOOST_CHECK_EQUAL(oss.str(), "ThreeInts:11111111:22222222:55555555");
}

// TODO: Shouldn't this test be in its own file?
BOOST_AUTO_TEST_CASE(test_moneta_codec_group_decoder_decode_many_test) {
	unsigned char buffer[] = {
		0x30,
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55,
		0x40,
		0x11, 0x11, 0x11, 0x11, 0xaa, 0xaa, 0xaa, 0xaa,
		0x55, 0x55, 0x55, 0x55, 0x77, 0x77, 0x77, 0x77,
		0x50,
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x33, 0x33, 0x33, 0x33, 0x44, 0x44, 0x44, 0x44,
		0x55, 0x55, 0x55, 0x55,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
	};

	typedef moneta::codec::group_decoder<
		moneta::codec::decoder<
			moneta::codec::enter_actions<acme_enter_entity>,
			moneta::codec::member_actions<acme_member_decoder>
		>,
		moneta::traits::entity_group<
			FiveInts, ThreeInts, FourInts
		>,
		example_prefix
	> decoder;

	std::ostringstream oss;
	entity_visitor visitor(oss);

	int result = moneta::codec::decode_many<decoder>(std::begin(buffer), std::end(buffer), visitor);

	BOOST_CHECK_EQUAL(result, 1 + sizeof(ThreeInts) + 1 + sizeof(FourInts) + 1 + sizeof(FiveInts));
	BOOST_CHECK_EQUAL(
		oss.str(),
		"ThreeInts:11111111:22222222:55555555"
		"FourInts:11111111:aaaaaaaa:55555555:77777777"
		"FiveInts:11111111:22222222:33333333:44444444:55555555"
	);
}
