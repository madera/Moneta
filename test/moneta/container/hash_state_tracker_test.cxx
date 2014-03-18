#include "stdafx.h"
#include <moneta/container/hash_state_tracker.hxx>
#include "../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(hash_state_tracker_test) {
	Cat cat;
	cat.ID = 1;
	cat.Name = "Garfield";
	cat.Address.ID = 1;
	cat.Address.Number = 123;
	cat.Address.Street = "Infinite Av.";

	moneta::container::hash_state_tracker<Cat> tracker;
	tracker.update(cat);
}
