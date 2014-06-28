#include "stdafx.h"
#include <moneta/serialization/rawbin/encoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/Address.hxx"

BOOST_AUTO_TEST_CASE(default_encoder_test) {
	Address address;
	address.ID = 1;
	address.Number = 10;
	address.Street = "String St.";

	unsigned char buffer[128];
	std::fill(buffer, buffer + 128, 0x55);

	moneta::serialization::rawbin::encode(address, buffer, buffer + 128);
	hexdump(buffer, 128);
}

// TODO: Test custom encoders.
// TODO: Add support for entity-wise encoder override.

// namespace moneta { namespace serialization { namespace rawbin {
// 
// 	namespace detail {
// 
// 		template <>
// 		struct encoder<Person> {
// 
// 			template <class IteratorType>
// 			int operator()(Person& entity, IteratorType begin, IteratorType end) const {
// 				return 0;
// 			}
// 		};
// 
// 	}
// 
// }}}

BOOST_AUTO_TEST_CASE(custom_encoder_test) {
	unsigned char buffer[1024];
	std::fill(buffer, buffer + 1024, 0);
}
