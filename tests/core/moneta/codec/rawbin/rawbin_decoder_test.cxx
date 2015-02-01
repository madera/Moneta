#include "stdafx.h"
//#include <moneta/codec/rawbin/rawbin_decoder.hxx>
//#include <moneta/serialization/detail/hexdump.hxx>
//#include "../../model/simple/ThreeInts.hxx"
//#include "../../model/simple/FourInts.hxx"
//
//BOOST_AUTO_TEST_CASE(simple_rawbin_decoder_test) {
//
//	BOOST_STATIC_ASSERT(sizeof(FourInts) == 4*sizeof(int));
//
//	unsigned char data[16] = {
//		0x44, 0x33, 0x22, 0x11, 0x40, 0x30, 0x20, 0x10,
//		0xdd, 0xcc, 0xbb, 0xaa, 0xff, 0xee, 0xdd, 0xcc
//	};
//
//	FourInts ints;
//	int result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 16);
//
//	BOOST_CHECK_EQUAL(result, 4*sizeof(int));
//
//	BOOST_CHECK_EQUAL(ints.One,   0x11223344);
//	BOOST_CHECK_EQUAL(ints.Two,   0x10203040);
//	BOOST_CHECK_EQUAL(ints.Three, 0xaabbccdd);
//	BOOST_CHECK_EQUAL(ints.Four,  0xccddeeff);
//}
//
//BOOST_AUTO_TEST_CASE(fixed_values_rawbin_decoder_test) {
//
//	unsigned char bad[16] = {
//		0xba, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0xba,
//		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
//	};
//
//	ThreeInts ints;
//	int result = moneta::codec::decode<moneta::codec::rawbin>(ints, bad, bad + 16);
//
//	BOOST_CHECK_EQUAL(result, 0);
//
//	unsigned char good[16] = {
//		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
//		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
//	};
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, good, good + 16);
//	BOOST_CHECK_EQUAL(result, 3 * sizeof(int));
//	BOOST_CHECK_EQUAL(ints.One, 0x11111111);
//	BOOST_CHECK_EQUAL(ints.Two, 0x22222222);
//	BOOST_CHECK_EQUAL(ints.Three, 0x55555555);
//}
//
//BOOST_AUTO_TEST_CASE(old_need_more_data_rawbin_decoder_test) {
//
//	BOOST_STATIC_ASSERT(sizeof(int) == 4);
//
//	unsigned char data[16] = {
//		0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22,
//		0x55, 0x55, 0x55, 0x55, 0x00, 0x00, 0x00, 0x00
//	};
//
//	ThreeInts ints;
//
//	int result;
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data);
//	BOOST_CHECK_EQUAL(result, -4);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 1);
//	BOOST_CHECK_EQUAL(result, -3);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 2);
//	BOOST_CHECK_EQUAL(result, -2);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 3);
//	BOOST_CHECK_EQUAL(result, -1);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 4);
//	BOOST_CHECK_EQUAL(result, -4);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 5);
//	BOOST_CHECK_EQUAL(result, -3);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 6);
//	BOOST_CHECK_EQUAL(result, -2);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 7);
//	BOOST_CHECK_EQUAL(result, -1);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 8);
//	BOOST_CHECK_EQUAL(result, -4);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 9);
//	BOOST_CHECK_EQUAL(result, -3);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 10);
//	BOOST_CHECK_EQUAL(result, -2);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 11);
//	BOOST_CHECK_EQUAL(result, -1);
//
//	result = moneta::codec::decode<moneta::codec::rawbin>(ints, data, data + 12);
//	BOOST_CHECK_EQUAL(result, 12);
//}
