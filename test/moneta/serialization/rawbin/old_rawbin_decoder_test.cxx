#include "stdafx.h"
#include <moneta/serialization/rawbin/old_rawbin_decoder.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

BOOST_AUTO_TEST_CASE(old_simple_decoder_test) {

	BOOST_STATIC_ASSERT(sizeof(FourInts) == 4*sizeof(int));

	unsigned char data[16] = {
		0x44, 0x33, 0x22, 0x11, 0x40, 0x30, 0x20, 0x10,
		0xdd, 0xcc, 0xbb, 0xaa, 0xff, 0xee, 0xdd, 0xcc
	};

	FourInts ints;
	int result = moneta::serialization::rawbin::decode(data, data + 16, ints);

	BOOST_CHECK_EQUAL(result, 4 * sizeof(int));

	BOOST_CHECK_EQUAL(ints.One,   0x11223344);
	BOOST_CHECK_EQUAL(ints.Two,   0x10203040);
	BOOST_CHECK_EQUAL(ints.Three, 0xaabbccdd);
	BOOST_CHECK_EQUAL(ints.Four,  0xccddeeff);

}

BOOST_AUTO_TEST_CASE(old_fixed_values_decoder_test) {

	unsigned char bad[16] = {
		0xba, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0xba,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	ThreeInts ints;
	int result = moneta::serialization::rawbin::decode(bad, bad + 16, ints);

	BOOST_CHECK_EQUAL(result, 0);

	unsigned char good[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	result = moneta::serialization::rawbin::decode(good, good + 16, ints);
	BOOST_CHECK_EQUAL(result, 3*sizeof(int));
	BOOST_CHECK_EQUAL(ints.One,   0x11111111);
	BOOST_CHECK_EQUAL(ints.Two,   0x22222222);
	BOOST_CHECK_EQUAL(ints.Three, 0x55555555);
}

BOOST_AUTO_TEST_CASE(old_need_more_data_decoder_test) {

	BOOST_STATIC_ASSERT(sizeof(int) == 4);

	unsigned char data[16] = {
		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
	};

	ThreeInts ints;

	int result;

	result = moneta::serialization::rawbin::decode(data, data, ints);
	BOOST_CHECK_EQUAL(result, -4);

	result = moneta::serialization::rawbin::decode(data, data + 1, ints);
	BOOST_CHECK_EQUAL(result, -3);

	result = moneta::serialization::rawbin::decode(data, data + 2, ints);
	BOOST_CHECK_EQUAL(result, -2);

	result = moneta::serialization::rawbin::decode(data, data + 3, ints);
	BOOST_CHECK_EQUAL(result, -1);

	result = moneta::serialization::rawbin::decode(data, data + 4, ints);
	BOOST_CHECK_EQUAL(result, -4);

	result = moneta::serialization::rawbin::decode(data, data + 5, ints);
	BOOST_CHECK_EQUAL(result, -3);

	result = moneta::serialization::rawbin::decode(data, data + 6, ints);
	BOOST_CHECK_EQUAL(result, -2);

	result = moneta::serialization::rawbin::decode(data, data + 7, ints);
	BOOST_CHECK_EQUAL(result, -1);

	result = moneta::serialization::rawbin::decode(data, data + 8, ints);
	BOOST_CHECK_EQUAL(result, -4);

	result = moneta::serialization::rawbin::decode(data, data + 9, ints);
	BOOST_CHECK_EQUAL(result, -3);

	result = moneta::serialization::rawbin::decode(data, data + 10, ints);
	BOOST_CHECK_EQUAL(result, -2);

	result = moneta::serialization::rawbin::decode(data, data + 11, ints);
	BOOST_CHECK_EQUAL(result, -1);

	result = moneta::serialization::rawbin::decode(data, data + 12, ints);
	BOOST_CHECK_EQUAL(result, 12);
}
