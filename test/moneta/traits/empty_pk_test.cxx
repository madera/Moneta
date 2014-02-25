#include "stdafx.h"
#include <moneta/traits/empty_pk.hxx>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(empty_pk_test) {
	moneta::traits::empty_pk<Person>();
	moneta::traits::empty_pk<Dog>();


}
