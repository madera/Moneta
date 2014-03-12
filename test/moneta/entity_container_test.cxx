#include "stdafx.h"
#include <moneta/entity_container.hxx>
#include "model/Cat.hxx"

#include <moneta/sql/traits/db_tuple.hxx>

BOOST_AUTO_TEST_CASE(entity_container_test) {
	moneta::entity_container<Cat> cat_container;

	BOOST_CHECK_EQUAL(cat_container.size(), 0);

	Cat cat = moneta::make_entity<Cat>();
	cat_container.insert(cat);
	BOOST_CHECK_EQUAL(cat_container.size(), 1);
	//BOOST_CHECK(cat_container.is_bound(cat));
	//BOOST_CHECK(cat_container.is_new(cat));


	std::vector<moneta::sql::traits::db_tuple<Cat>::type> v;
	v.push_back(moneta::sql::traits::db_tuple<Cat>::type(1, "Text", 3));



	//john.Name = "John";
	//container.insert(john);
	//assert(container.size() == 1);

	//container.erase(john);
	//assert(container.size() == 0);
	//assert(!container.is_bound(john));

	//container.insert(john);
	//assert(container.size() == 1);
	//assert(container.is_bound(john));
	//assert(container.is_new(john));
	//assert(!container.is_persisted(john));
	//assert(!container.is_deleted(john));

	//container.set_deleted(john);
	//assert(container.size() == 1);
	//assert(!container.is_new(john));
	//assert(container.is_deleted(john));

	//std::cerr << "New entities:" << std::endl;
	//container.for_each_new([](const Person& entity) {
	//	std::cerr << entity << std::endl;
	//});

	//std::cerr << "Deleted entities:" << std::endl;
	//container.for_each_deleted([](const Person& entity) {
	//	std::cerr << entity << std::endl;
	//});
}
