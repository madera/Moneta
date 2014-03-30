#include "stdafx.h"
#include <moneta/container/relational_container.hxx>
#include "../model/Cat.hxx"
#include "../model/Lizard.hxx"

BOOST_AUTO_TEST_CASE(entity_container_test) {
	Cat cat;
	cat.ID = 1;
	cat.Name = "Somecat";
	cat.Address.ID = 10;

	moneta::container::relational_container<Cat> container;
	BOOST_CHECK_EQUAL(container.size(), 0);
	BOOST_CHECK_EQUAL(container.bound(cat), false);

	container.insert(cat, true);

	BOOST_CHECK_EQUAL(container.size(), 1);
	BOOST_CHECK(container.bound(cat));
	BOOST_CHECK(container.bound(1));

	BOOST_CHECK(!container.dirty(cat));
	BOOST_CHECK(container.newcomer(cat));

	{ Cat cat; cat.ID = 2; cat.Name = "Garfield"; cat.Address.ID = 15; container.insert(cat, true); }
	{ Cat cat; cat.ID = 3; cat.Name = "Felix";    cat.Address.ID = 30; container.insert(cat, true); }
	{ Cat cat; cat.ID = 4; cat.Name = "Bob";      cat.Address.ID = 10; container.insert(cat, true); }

	BOOST_CHECK_EQUAL(container.size(), 4);
	BOOST_CHECK(container.bound(2));
	BOOST_CHECK(container.bound(3));
	BOOST_CHECK(container.bound(4));
	BOOST_CHECK(container.newcomer(2));
	BOOST_CHECK(container.newcomer(3));
	BOOST_CHECK(container.newcomer(4));
	BOOST_CHECK(!container.dirty(2));
	BOOST_CHECK(!container.dirty(3));
	BOOST_CHECK(!container.dirty(4));

	container.dbg();

	container.remove(1);
	BOOST_CHECK(container.removed(1));

	//container.persist(1);
	//BOOST_CHECK(container.persisted(1));
}
