#include "stdafx.h"
#include <moneta/codec/encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../model/tree/A.hxx"
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/Arithmetics.hxx"
#include "../model/Cat.hxx"

namespace moneta { namespace codec {

	struct test_codec;

	template <class T>
	struct value_encoder<test_codec, T, typename boost::disable_if<boost::is_arithmetic<T> >::type> {
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
	struct value_encoder<test_codec, T, typename boost::enable_if<boost::is_arithmetic<T> >::type> {
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
	struct value_encoder<test_codec, int> {
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
	struct value_encoder<test_codec, std::string> {
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

BOOST_AUTO_TEST_CASE(value_encoder_only_encoder_test) {
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

BOOST_AUTO_TEST_CASE(simple_traversal_encoder_test) {
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

namespace moneta { namespace codec {

	struct named_test_codec;

	template <class Member, class Path>
	struct member_encoder<named_test_codec, Member, Path> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			typedef typename Member::result_type value_type;

			Iterator itr = begin;

			std::ostringstream oss;
			oss << traits::detail::member_name<Member>::get() << ": ";

			std::string str = oss.str();
			int size = str.size();

			int available = end - itr;
			if (available < size) {
				return available - size;
			}

			std::copy(str.begin(), str.end(), itr);
			itr += size;

			int result = value_encoder<test_codec, value_type>()(Member()(entity), itr, end);
			if (result <= 0) {
				return result;
			}

			itr += result;
			
			if (itr != end) {
				*itr++ = '\n';
			} else {
				return -1; // Need one more...
			}

			return itr - begin;
		}
	};

}}

BOOST_AUTO_TEST_CASE(member_encoder_test) {
	typedef Arithmetics entity_type;

	const char expected[] = "Bool: A\nChar: A\nShort: A\nInt: i\nLong: A\n";
	const size_t expected_size = sizeof(expected) - 1; // Minus null char.

	char buffer[expected_size];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::named_test_codec>(
		entity_type(), buffer, buffer + sizeof(buffer)
	);

	BOOST_CHECK_EQUAL(result, expected_size);

	BOOST_CHECK_EQUAL_COLLECTIONS(buffer, buffer + sizeof(buffer), expected, expected + expected_size);
}

namespace moneta { namespace codec {

	struct entity_encoder_test_codec;

	template <class T>
	struct value_encoder<
		entity_encoder_test_codec,
		T,
		typename boost::disable_if<
			moneta::traits::is_entity<T>
		>::type
	> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			std::cerr << "--> " << typeid(T).name() << std::endl;
			*begin = 'v';
			return 1;
		}
	};

	template <class Member, class Path>
	struct member_encoder<entity_encoder_test_codec, Member, Path> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			*begin = 'm';
			return 1;
		}
	};

	template <class T>
	struct value_encoder<entity_encoder_test_codec, T> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'V';
			return 1;
		}
	};

	template <class Path, class Entity>
	struct enter_entity_encoder<entity_encoder_test_codec, Path, Entity> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'E';
			return 1;
		}
	};

	template <class Path, class Entity>
	struct leave_entity_encoder<entity_encoder_test_codec, Path, Entity> {
		template <class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			if (begin == end) {
				return -1;
			}

			*begin = 'L';
			return 1;
		}
	};

}}

BOOST_AUTO_TEST_CASE(entity_encoder_test) {
	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::entity_encoder_test_codec>(
		Cat(), buffer, buffer + sizeof(buffer)
	);

	const std::string expected = "EmmEmmmLL";

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}

BOOST_AUTO_TEST_CASE(simple_traversal_encoder_test2) {
	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = moneta::codec::encode<moneta::codec::entity_encoder_test_codec>(
		A(), buffer, buffer + sizeof(buffer)
	);

	const std::string expected = "EmmEEmmLmEmEmmLLLmL";

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}