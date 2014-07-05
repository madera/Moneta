#include "stdafx.h"
#include <moneta/codec/encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/Arithmetics.hxx"
#include "../model/Cat.hxx"

namespace moneta { namespace codec {

	struct test_codec;

	template <class T>
	struct encoder<test_codec, T, typename boost::disable_if<boost::is_arithmetic<T> >::type> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'X';
			return 1;
		}
	};

	template <class T>
	struct encoder<test_codec, T, typename boost::enable_if<boost::is_arithmetic<T> >::type> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'A';
			return 1;
		}
	};

	template <>
	struct encoder<test_codec, int> {
		template <class Iterator>
		int operator()(const int value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'i';
			return 1;
		}
	};

	template <>
	struct encoder<test_codec, std::string> {
		template <class Iterator>
		int operator()(const std::string& value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'S';
			return 1;
		}
	};

}}

BOOST_AUTO_TEST_CASE(insufficient_buffer_encoder_test) {
	typedef ThreeInts entity_type;

	const size_t member_count = boost::mpl::size<
		typename moneta::traits::members<entity_type>::type
	>::value;

	char buffer[member_count];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::test_codec>(entity_type(), buffer, buffer);
	BOOST_CHECK_EQUAL(result, -1);
}

BOOST_AUTO_TEST_CASE(simple_encoder_test) {
	typedef Arithmetics entity_type;

	const size_t member_count = boost::mpl::size<
		typename moneta::traits::members<entity_type>::type
	>::value;

	char buffer[member_count];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::test_codec>(
		entity_type(), buffer, buffer + sizeof(buffer)
	);

	BOOST_CHECK_EQUAL(result, member_count);

	const std::string expected = "AAAiA";
	BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + sizeof(buffer), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(traversal_encoder_test) {
	const size_t member_count =
		boost::mpl::size<typename moneta::traits::members<Cat    >::type>::value -1 +
		boost::mpl::size<typename moneta::traits::members<Address>::type>::value
	;

	char buffer[member_count];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::test_codec>(
		Cat(), buffer, buffer + sizeof(buffer)
	);

	BOOST_CHECK_EQUAL(result, member_count);

	const std::string expected = "iSiiS";
	BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + sizeof(buffer), expected.begin(), expected.end());
}
