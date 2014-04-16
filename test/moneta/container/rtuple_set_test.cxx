#include "stdafx.h"
#include <moneta/container/rtuple_set.hxx>
#include "../model/Cat.hxx"
#include "../model/CatDataset.hxx"
#include "../model/Lizard.hxx"

BOOST_AUTO_TEST_CASE(entity_set_test) {

	std::vector<Cat> dataset = test_dataset<Cat>()();

	moneta::container::rtuple_set<Cat> set;
	BOOST_CHECK_EQUAL(set.size(), 0);
	BOOST_CHECK_EQUAL(set.is_bound(1), false);

	set.insert(moneta::traits::to_rtuple(dataset[0]), true);

	BOOST_CHECK_EQUAL(set.size(), 1);
	BOOST_CHECK(set.is_bound(1));
	BOOST_CHECK(!set.is_dirty(1));
	BOOST_CHECK(set.is_newcomer(1));

	set.insert(moneta::traits::to_rtuple(dataset[1]), true);
	set.insert(moneta::traits::to_rtuple(dataset[2]), true);
	set.insert(moneta::traits::to_rtuple(dataset[3]), true);

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
