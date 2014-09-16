#include "stdafx.h"
#include <moneta/codec/typecode.hxx>
#include <boost/optional.hpp>
#include "../model/tree/A.hxx"

namespace moneta { namespace codec {
	
	struct dummy_codec;

	namespace detail {

		template <class Entity>
		struct typecode_type<dummy_codec, Entity> : boost::mpl::identity<int> {};

		template <>
		struct typecode<dummy_codec, A> {
			static int get() {
				return 10;
			}
		};

	}

	template <>
	struct typecode_reader<dummy_codec> {
		template <class Iterator>
		int operator()(int& code, Iterator begin, Iterator end) const {
			code = *begin;
			return 1;
		}
	};

}}

BOOST_AUTO_TEST_CASE(type_tag_test) {
	char buffer[] = { 0x0a, 0x55, 0x0b, 0xaa, 0x0c, 0x55 };

	moneta::codec::detail::typecode_type<moneta::codec::dummy_codec>::type code;
	int result = moneta::codec::read_typecode<moneta::codec::dummy_codec>(code, std::begin(buffer), std::end(buffer));

	BOOST_CHECK_EQUAL(result, 1);
	BOOST_CHECK_EQUAL(code, 0x0a);
}
