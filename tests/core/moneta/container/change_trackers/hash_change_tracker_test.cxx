#include "stdafx.h"
#include <moneta/container/change_trackers/hash_change_tracker.hxx>
#include "../../model/Person.hxx"
#include "../../model/Cat.hxx"

BOOST_AUTO_TEST_CASE(hash_state_tracker_test) {
	Cat cat;
	cat.ID = 1;
	cat.Name = "Garfield";
	cat.Address.ID = 1;
	cat.Address.Number = 123;
	cat.Address.Street = "Infinite Av.";
}

BOOST_AUTO_TEST_CASE(hash_state_tracker_test_2) {
	Person joe;
	joe.ID = 123;
	joe.Name = "Joe";
	joe.Fingers = 10;
	joe.Height = 1.80;

	typedef moneta::container::detail::hash_change_tracker_impl<void, Person> hasher_type;
	
	hasher_type::entry e0(joe);
	std::cerr << '[' << e0.to_string() << ']' << std::endl;

	joe.Height += 0.01;
	hasher_type::entry e1(joe);
	std::cerr << '[' << e1.to_string() << ']' << std::endl;

	e0.update(joe);
	std::cerr << '[' << e0.to_string() << ']' << std::endl;

	std::ostringstream oss;
	oss << "asdf";
	oss.str();

}
