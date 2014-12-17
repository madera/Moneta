#include "stdafx.h"
#include <moneta/codec/typecode.hxx>
#include <boost/optional.hpp>
#include "../model/tree/A.hxx"

MONETA_CODEC_DECLARE(dummy_codec)
using moneta::codec::dummy_codec;

MONETA_CODEC_TYPECODE_TYPE(dummy_codec, int)
MONETA_CODEC_TYPECODE(dummy_codec, A, 10)
MONETA_CODEC_TYPECODE(dummy_codec, B, 12)
MONETA_CODEC_TYPECODE(dummy_codec, C, 14)
MONETA_CODEC_TYPECODE(dummy_codec, D, 16)
MONETA_CODEC_TYPECODE(dummy_codec, E, 18)

namespace moneta { namespace codec {

	template <>
	struct typecode_reader<dummy_codec> {
		template <class Iterator>
		int operator()(int& code, Iterator begin, Iterator end) const {
			code = *begin;
			return 1;
		}
	};

}}

// TODO: Test variable typecode_type<> types (depending on the entity at hand).
//	 This will need more design and thinking.

BOOST_AUTO_TEST_CASE(read_typecode_test) {
	char buffer[] = { 0x0a, 0x55, 0x0b, 0x55, 0x0c, 0x55 };

	moneta::codec::detail::typecode_type<moneta::codec::dummy_codec>::type code;
	char* p = &buffer[0];
	int result;

	result = moneta::codec::read_typecode<moneta::codec::dummy_codec>(code, p, std::end(buffer));
	BOOST_REQUIRE_GT(result, 0);
	p += result; ++p;
	BOOST_CHECK_EQUAL(result, 1);
	BOOST_CHECK_EQUAL(code, 0x0a);

	result = moneta::codec::read_typecode<moneta::codec::dummy_codec>(code, p, std::end(buffer));
	BOOST_REQUIRE_GT(result, 0);
	p += result; ++p;
	BOOST_CHECK_EQUAL(result, 1);
	BOOST_CHECK_EQUAL(code, 0x0b);
}
