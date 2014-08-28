#include "stdafx.h"
#include <moneta/lexical/detail/from_text_impl.hxx>
#include <boost/test/floating_point_comparison.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(from_text_impl_test) {
	moneta::serialization::detail::from_text_impl<Person> text_assigner;

	Person person;
	text_assigner(person, 0, "123");
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	text_assigner(person, 1, "Somebody");
	BOOST_CHECK_EQUAL(person.Name, "Somebody");
	
	text_assigner(person, 2, "1.80");
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);

	text_assigner(person, 3, "100");
	BOOST_CHECK_EQUAL(person.Fingers, 100);
}
