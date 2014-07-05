#include "stdafx.h"
#include <moneta/codec/decoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/Arithmetics.hxx"
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/CascadedInts.hxx"
#include "../model/simple/FixedThreeInts.hxx"

namespace moneta { namespace codec {

	struct test_codec;

	template <class T>
	struct decoder<test_codec, T> {
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
		typename moneta::traits::members<entity_type>::type
	>::value;

	char buffer[member_count];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::decode<moneta::codec::test_codec>(entity_type(), buffer, buffer);
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
		boost::mpl::size<typename moneta::traits::members<CascadedInts>::type>::value - 2 +
		boost::mpl::size<typename moneta::traits::members<TwoInts     >::type>::value     +
		boost::mpl::size<typename moneta::traits::members<FourInts    >::type>::value
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
