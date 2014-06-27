#include "stdafx.h"
#include <moneta/serialization/for_each_member.hxx>

#include "../model/Person.hxx"
#include "../model/Cat.hxx"

struct value_printer {
	template <typename T>
	void operator()(T x) const {
		std::cerr << typeid(T).name() << std::endl;
	}
};

BOOST_AUTO_TEST_CASE(for_each_member_test) {
	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Fingers = 12;
	//moneta::serialization::for_each_member(person, value_printer());

	Cat cat;
	cat.ID = 10;
	cat.Name = "Garfield";
	cat.Address.ID = 20;
	cat.Address.Number = 200;
	cat.Address.Street = "St. Street";
	moneta::serialization::for_each_member(cat, value_printer());

	const Cat& const_cat = cat;
	moneta::serialization::for_each_member(const_cat, value_printer());
}

