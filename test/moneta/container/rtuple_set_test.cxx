#include "stdafx.h"
#include <moneta/container/rtuple_set.hxx>
#include "../model/Cat.hxx"
#include "../model/Lizard.hxx"

BOOST_AUTO_TEST_CASE(entity_set_test) {

	Cat cat;
	cat.ID = 1;
	cat.Name = "Somecat";
	cat.Address.ID = 10;

	moneta::container::rtuple_set<Cat> set;
	BOOST_CHECK_EQUAL(set.size(), 0);
	BOOST_CHECK_EQUAL(set.is_bound(1), false);

	set.insert(moneta::sql::traits::to_rtuple(cat), true);

	BOOST_CHECK_EQUAL(set.size(), 1);
	BOOST_CHECK(set.is_bound(1));

	BOOST_CHECK(!set.is_dirty(1));
	BOOST_CHECK(set.is_newcomer(1));

	{ Cat cat; cat.ID = 2; cat.Name = "Garfield"; cat.Address.ID = 15; set.insert(moneta::sql::traits::to_rtuple(cat), true); }
	{ Cat cat; cat.ID = 3; cat.Name = "Felix";    cat.Address.ID = 30; set.insert(moneta::sql::traits::to_rtuple(cat), true); }
	{ Cat cat; cat.ID = 4; cat.Name = "Bob";      cat.Address.ID = 10; set.insert(moneta::sql::traits::to_rtuple(cat), true); }

	BOOST_CHECK_EQUAL(set.size(), 4);
	BOOST_CHECK(set.is_bound(2));
	BOOST_CHECK(set.is_bound(3));
	BOOST_CHECK(set.is_bound(4));
	BOOST_CHECK(set.is_newcomer(2));
	BOOST_CHECK(set.is_newcomer(3));
	BOOST_CHECK(set.is_newcomer(4));
	BOOST_CHECK(!set.is_dirty(2));
	BOOST_CHECK(!set.is_dirty(3));
	BOOST_CHECK(!set.is_dirty(4));

	set.remove(1);
	BOOST_CHECK(set.is_removed(1));

	//set.persist(1);
	//BOOST_CHECK(set.persisted(1));
}
