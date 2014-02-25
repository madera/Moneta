#include "stdafx.h"
#include <moneta/traits/extract_pk.hxx>
#include "../model/Person.hxx"

#include <boost/mpl/print.hpp>

BOOST_AUTO_TEST_CASE(extract_pk_test) {
	Person person;
	person.ID = 1;
	person.Name = "John Smith";
	person.Height = 1.80;
	person.Fingers = 10;

	moneta::traits::extract_pk(person) = 555;
	BOOST_CHECK_EQUAL(person.ID, 555);
}
