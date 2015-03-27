#include "pch.hxx"
#include <moneta/traits/is_entity.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

struct Dummy {
};

BOOST_AUTO_TEST_CASE(is_entity_test) {
	BOOST_MPL_ASSERT((moneta::traits::is_entity<Person>));
	BOOST_MPL_ASSERT((moneta::traits::is_entity<Dog>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_entity<Dummy>));
}
