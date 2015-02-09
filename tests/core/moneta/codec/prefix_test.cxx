#include "stdafx.h"
#include <moneta/codec/prefix.hxx>
#include <boost/optional.hpp>
#include "../model/Person.hxx"
#include "../model/Address.hxx"
#include "../model/tree/A.hxx"

struct acme_prefix {
	typedef char type;
	
	template <class Iterator>
	int operator()(Iterator, Iterator, type&) const {
		// We don't use this for our tests.
		return 0;
	}
};

MONETA_CODEC_PREFIX_VALUE(acme_prefix, Person,  0x0a)
MONETA_CODEC_PREFIX_VALUE(acme_prefix, Address, 0x0b)

inline void static_test() {
	BOOST_MPL_ASSERT((
		boost::is_same<
			moneta::codec::detail::prefix_value<acme_prefix, A>::type,
			boost::false_type
		>
	));
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_prefix) {
	BOOST_CHECK_EQUAL((moneta::codec::detail::prefix_value<acme_prefix, Person >::get()), 0x0a);
	BOOST_CHECK_EQUAL((moneta::codec::detail::prefix_value<acme_prefix, Address>::get()), 0x0b);
}
