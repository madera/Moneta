// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/container/meta_set.hxx>
#include <moneta/container/data_trackers/pk_tracker.hxx>
#include <moneta/container/data_trackers/rtuple_data.hxx>
#include <moneta/container/load_trackers/bitset_load_tracker.hxx>
#include <moneta/container/change_trackers/hash_change_tracker.hxx>
#include "../model/Person.hxx"
#include "../model/CatDataset.hxx"

template <class Master, class Entity>
struct test_tracker_impl {
	typedef Master base_type;
	typedef Entity entity_type;

	typedef test_tracker_impl this_type;

	struct entry {
		int state;

		bool operator<(const int rhs) const {
			return state < rhs;
		}
	};

	//struct get_index {
	//	template <class MasterEntry>
	//	struct apply {
	//		typedef boost::multi_index::hashed_unique<
	//			boost::multi_index::tag<this_type>,
	//			boost::multi_index::member<
	//				entry,
	//				typename entry::state_type,
	//				&entry::pk
	//			>
	//		> type;
	//	};
	//};
};

template <class Entity>
struct test_tracker : boost::mpl::lambda<
	test_tracker_impl<boost::mpl::_1, Entity>
>::type {
	typedef Entity entity_type;
};

//#include <boost/mpl/print.hpp>

//inline void static_test() {
//
//	typedef moneta::container::meta_set<
//		test_tracker<Person>
//	> meta_set_type;
//
//	// TODO: Continue static tests here.
//}

//BOOST_AUTO_TEST_CASE(meta_set_test) {
//	std::cerr << "=== B: test_tracker ===" << std::endl;
//
//	typedef moneta::container::meta_set<
//		test_tracker<Person>
//	> meta_set_type;
//
//	meta_set_type set;
//	BOOST_CHECK_EQUAL(set.size(), 0);
//
//	meta_set_type::entry entry;
//	set.insert(entry);
//	BOOST_CHECK_EQUAL(set.size(), 1);
//
//	std::cerr << "=== E: test_tracker ===" << std::endl;
//}

BOOST_AUTO_TEST_CASE(meta_set_test_2) {
	std::cerr << "=== B: pk_tracker + bitset_load_tracker2 + rtuple_data ===" << std::endl;

	typedef moneta::container::meta_set<
		moneta::container::pk_tracker<Person>,
		moneta::container::bitset_load_tracker2<Person>,
		moneta::container::rtuple_data<Person>
	> meta_set_type;

	Person joe;
	joe.ID = 123;
	joe.Name = "Joe";
	joe.Ratings = 10;
	joe.Height = 1.80;

	meta_set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);

	meta_set_type::entry entry(joe);

	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	boost::optional<meta_set_type::entry> e0 = set.find(123);
	BOOST_REQUIRE(e0.is_initialized());

	BOOST_CHECK_EQUAL(e0->pk, 123);
	BOOST_CHECK_EQUAL(e0->data, moneta::traits::to_rtuple(joe));

	set.erase(123);
	BOOST_CHECK_EQUAL(set.size(), 0);

	set.insert(entry);
	set.insert(entry);
	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	std::cerr << "=== E: pk_tracker + bitset_load_tracker2 + rtuple_data ===" << std::endl;
}

BOOST_AUTO_TEST_CASE(meta_set_test_3) {
	std::cerr << "=== B: pk_tracker + bitset_load_tracker2 + rtuple_data + hash_change_tracker ===" << std::endl;

	typedef moneta::container::meta_set<
		moneta::container::pk_tracker<Person>,
		moneta::container::bitset_load_tracker2<Person>,
		moneta::container::rtuple_data<Person>,
		moneta::container::hash_change_tracker2<Person>
	> meta_set_type;

	Person joe;
	joe.ID = 123;
	joe.Name = "Joe";
	joe.Ratings = 10;
	joe.Height = 1.80;

	meta_set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);

	meta_set_type::entry entry(joe);

	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	boost::optional<meta_set_type::entry> e0 = set.find(123);
	BOOST_REQUIRE(e0.is_initialized());

	BOOST_CHECK_EQUAL(e0->pk, 123);
	BOOST_CHECK_EQUAL(e0->data, moneta::traits::to_rtuple(joe));

	set.erase(123);
	BOOST_CHECK_EQUAL(set.size(), 0);

	// Repeated object insert: should ignore.
	//
	set.insert(entry);
	set.insert(entry);
	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	// Insert object directly (although in this case it's repeated and should be ignored).
	set.insert(joe);
	BOOST_CHECK_EQUAL(set.size(), 1);

	std::cerr << "=== E: pk_tracker + bitset_load_tracker2 + rtuple_data + hash_change_tracker ===" << std::endl;
}

BOOST_AUTO_TEST_CASE(meta_set_PORTED_test) {
	std::cerr << "POOOOORTED" << std::endl;
	const std::vector<Cat> dataset = test_dataset<Cat>()();

	typedef moneta::container::meta_set<
		  moneta::container::pk_tracker<Cat>
		, moneta::container::bitset_load_tracker2<Cat>
		, moneta::container::rtuple_data<Cat>
//		, moneta::container::hash_change_tracker2<Cat>
	> meta_set_type;

	meta_set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);
//	BOOST_CHECK_EQUAL(set.is_bound(1), false);

	set.insert(dataset[0]);

	BOOST_CHECK_EQUAL(set.size(), 1);
//	BOOST_CHECK(set.is_bound(1));
//	BOOST_CHECK(!set.is_dirty(1));
//	BOOST_CHECK(set.is_newcomer(1));

//	set.insert(moneta::traits::to_rtuple(dataset[1]), true);
//	set.insert(moneta::traits::to_rtuple(dataset[2]), true);
//	set.insert(moneta::traits::to_rtuple(dataset[3]), true);

//	BOOST_CHECK_EQUAL(set.size(), 4);
//	BOOST_CHECK(set.is_bound(2));
//	BOOST_CHECK(set.is_bound(3));
//	BOOST_CHECK(set.is_bound(4));
//	BOOST_CHECK(set.is_newcomer(2));
//	BOOST_CHECK(set.is_newcomer(3));
//	BOOST_CHECK(set.is_newcomer(4));
//	BOOST_CHECK(!set.is_dirty(2));
//	BOOST_CHECK(!set.is_dirty(3));
//	BOOST_CHECK(!set.is_dirty(4));

//	set.remove(1);
//	BOOST_CHECK(set.is_removed(1));

//	set.erase(1);

	//set.persist(1);
	//BOOST_CHECK(set.persisted(1));

	std::cerr << "~POOOOORTED" << std::endl;
}
