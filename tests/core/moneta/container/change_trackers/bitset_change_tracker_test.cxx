#include "stdafx.h"
#include <moneta/container/change_trackers/bitset_change_tracker.hxx>
#include "../../model/Cat.hxx"

//BOOST_AUTO_TEST_CASE(bitset_change_tracker_test) {
//	Cat cat;
//	cat.ID = 1;
//	cat.Name = "Garfield";
//	cat.Address.ID = 1;
//	cat.Address.Number = 123;
//	cat.Address.Street = "Infinite Av.";
//
//	moneta::container::bitset_change_tracker<Cat> tracker;
//	BOOST_CHECK_EQUAL(tracker.dirty(0), false);
//	BOOST_CHECK_EQUAL(tracker.dirty(1), false);
//	BOOST_CHECK_EQUAL(tracker.dirty(2), false);
//
//	tracker.loaded(0, true);
//	tracker.loaded(1, true);
//	tracker.loaded(2, true);
//	BOOST_CHECK_EQUAL(tracker.loaded(0), true);
//	BOOST_CHECK_EQUAL(tracker.loaded(1), true);
//	BOOST_CHECK_EQUAL(tracker.loaded(2), true);
//
//
//
//	moneta::container::bitset_change_tracker<Cat> tracker2(true);
//	BOOST_CHECK_EQUAL(tracker2.loaded(0), true);
//	BOOST_CHECK_EQUAL(tracker2.loaded(1), true);
//	BOOST_CHECK_EQUAL(tracker2.loaded(2), true);
//
//	tracker.loaded(0, false);
//	tracker.loaded(1, false);
//	tracker.loaded(2, false);
//	BOOST_CHECK_EQUAL(tracker.loaded(0), false);
//	BOOST_CHECK_EQUAL(tracker.loaded(1), false);
//	BOOST_CHECK_EQUAL(tracker.loaded(2), false);
//}
