#include "stdafx.h"
#include <moneta/serialization/rawbin/decoder.hxx>
#include "../../model/Person.hxx"

// namespace moneta { namespace serialization { namespace rawbin {
// 
// 	namespace detail {
// 
// 		template <>
// 		struct decoder<Person> {
// 			enum {
// 				MINIMUM_SIZE = sizeof(Person)
// 			};
// 
// 			template <class IteratorType>
// 			int operator()(IteratorType begin, IteratorType end, Person& entity) const {
// 				int delta = std::distance(begin, end) - MINIMUM_SIZE; // XXX: No std::distance().
// 				if (delta < 0) {
// 					return delta;
// 				}
// 
// 				//
// 
// 				return 0;
// 			}
// 		};
// 
// 	}
// 
// }}}

// BOOST_AUTO_TEST_CASE(decoder_test) {
// 	unsigned char buffer[1024];
// 	std::fill(buffer, buffer + 1024, 0);
// 
// 	int result;
// 	Person person;
// 
// 	// Zero length input
// 	//
// 	result = moneta::serialization::rawbin::decode(buffer, buffer, person);
// 	BOOST_CHECK_EQUAL(result, 0 - sizeof(Person));
// 
// 	//int x = moneta::decode(buffer, buffer + 1024, person);
// }
