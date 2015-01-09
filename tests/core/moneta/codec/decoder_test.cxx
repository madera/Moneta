#include "stdafx.h"
#include <moneta/codec/decoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/Arithmetics.hxx"
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/CascadedInts.hxx"
#include "../model/simple/FixedThreeInts.hxx"

// TODO: Make more tests for member_decoder.

namespace moneta { namespace codec {

	struct test_codec;

	template <class T>
	struct value_decoder<test_codec, T> {
		template <class Iterator>
		int operator()(T& target, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			target = static_cast<unsigned char>(*begin);

			return 1;
		}
	};

}}

BOOST_AUTO_TEST_CASE(insufficient_buffer_decoder_test) {
	typedef ThreeInts entity_type;

	const size_t member_count = boost::mpl::size<
		moneta::traits::members<entity_type>::type
	>::value;

	char buffer[member_count];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	entity_type entity;
	const int result = moneta::codec::decode<moneta::codec::test_codec>(entity, buffer, buffer);
	BOOST_CHECK_EQUAL(result, -1);
}

BOOST_AUTO_TEST_CASE(simple_decoder_test) {

	unsigned char buffer[4] = { 0x55, 0xaa, 0xdd, 0xff };

	FourInts ints;
	const int result = moneta::codec::decode<moneta::codec::test_codec>(ints, buffer, buffer + sizeof(buffer));

	BOOST_CHECK_EQUAL(result, 4);

	BOOST_CHECK_EQUAL(ints.One, 0x55);
	BOOST_CHECK_EQUAL(ints.Two, 0xaa);
	BOOST_CHECK_EQUAL(ints.Three, 0xdd);
	BOOST_CHECK_EQUAL(ints.Four, 0xff);
}

BOOST_AUTO_TEST_CASE(traversal_decoder_test) {
	const size_t member_count =
		boost::mpl::size<moneta::traits::members<CascadedInts>::type>::value - 2 +
		boost::mpl::size<moneta::traits::members<TwoInts     >::type>::value     +
		boost::mpl::size<moneta::traits::members<FourInts    >::type>::value
	;

	char buffer[member_count] = {
		0x22, 0x22, 0x44, 0x44, 0x44, 0x44
	};

	CascadedInts ints;
	const int result = moneta::codec::decode<moneta::codec::test_codec>(
		ints, buffer, buffer + sizeof(buffer)
	);

	BOOST_CHECK_EQUAL(result, member_count);

	BOOST_CHECK_EQUAL(ints.TwoInts.One, 0x22);
	BOOST_CHECK_EQUAL(ints.TwoInts.Two, 0x22);
	BOOST_CHECK_EQUAL(ints.FourInts.One,   0x44);
	BOOST_CHECK_EQUAL(ints.FourInts.Two,   0x44);
	BOOST_CHECK_EQUAL(ints.FourInts.Three, 0x44);
	BOOST_CHECK_EQUAL(ints.FourInts.Four,  0x44);
}

BOOST_AUTO_TEST_CASE(simple_fixed_value_decoder_test) {

	using moneta::codec::decode;
	using moneta::codec::test_codec;

	FixedThreeInts ints;

	{
		unsigned char all_bad[4] = { 0x55, 0xaa, 0xff };
		const int result = decode<test_codec>(ints, all_bad, all_bad + sizeof(all_bad));
		BOOST_CHECK_EQUAL(result, 0);
	}

	{
		unsigned char second_bad[4] = { 0x11, 0x77, 0xff };
		const int result = decode<test_codec>(ints, second_bad, second_bad + sizeof(second_bad));
		BOOST_CHECK_EQUAL(result, 0);
	}

	{
		unsigned char good[4] = { 0x11, 0x22, 0xff };
		const int result = decode<test_codec>(ints, good, good + sizeof(good));
		BOOST_CHECK_EQUAL(result, 3);

		BOOST_CHECK_EQUAL(ints.One, 0x11);
		BOOST_CHECK_EQUAL(ints.Two, 0x22);
		BOOST_CHECK_EQUAL(ints.Three, 0xff);
	}
}

static size_t member_decoder_hits = 0;

namespace moneta { namespace codec {

	struct named_test_codec;

	template <class Member, class Path>
	struct member_decoder<named_test_codec, Member, Path> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			++member_decoder_hits;
			return 1;
		}
	};

}}

BOOST_AUTO_TEST_CASE(member_decoder_test) {
	const size_t member_count = boost::mpl::size <
		moneta::traits::members<Arithmetics>::type
	> ::value;

	Arithmetics entity;
	const int result = moneta::codec::decode<moneta::codec::named_test_codec>(entity, 0, 0);
	BOOST_CHECK_EQUAL(result, member_count);

	BOOST_CHECK_EQUAL(member_decoder_hits, member_count);
}

//
// decode_unknown tests
//
// MAYBE: Move to its own test file?
//

#include "../model/tree/A.hxx"

MONETA_CODEC_DECLARE(dummy_codec)
using moneta::codec::dummy_codec;

MONETA_CODEC_TYPECODE_TYPE(dummy_codec, int)
MONETA_CODEC_TYPECODE(dummy_codec, A, 'a')
MONETA_CODEC_TYPECODE(dummy_codec, B, 'b')
MONETA_CODEC_TYPECODE(dummy_codec, C, 'c')
MONETA_CODEC_TYPECODE(dummy_codec, D, 'd')
MONETA_CODEC_TYPECODE(dummy_codec, E, 'e')

namespace moneta { namespace codec {

	template <>
	struct typecode_reader<dummy_codec> {
		template <class Iterator>
		int operator()(int& code, Iterator begin, Iterator end) const {
			if (std::distance(begin, end) < 1) {
				return -1;
			}

			code = *begin;
			return 1;
		}
	};

	template <class Entity>
	struct entity_decoder<dummy_codec, Entity> {

		template <class Iterator>
		int operator()(Entity& entity, Iterator begin, Iterator end) const {
			// Dumb decoder. We just *say* we consumed 2 bytes (typecode + data).
			// But actually, we don't.
			return 2;
		}

	};

}}

struct entity_lister {
	std::ostringstream& _output;
	
	entity_lister(std::ostringstream& output)
	 : _output(output) {}

	template <class Entity>
	void operator()(Entity& entity) const {
		_output << moneta::traits::detail::entity_name<Entity>::get();
	}
};

BOOST_AUTO_TEST_CASE(decode_unknown_test) {
	const std::string data = "a1b2c3";

	std::ostringstream oss;
	entity_lister counter(oss);

	auto begin = data.begin();
	auto end = data.end();

	const int result = moneta::codec::decode_unknowns<
		moneta::codec::dummy_codec,
		boost::mpl::vector<A, B, C, D, E>
	>(counter, begin, end);

	// Check that three entities were extracted.
	BOOST_CHECK_EQUAL(oss.str(), "ABC");
}
