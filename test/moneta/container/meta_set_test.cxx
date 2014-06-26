#include "stdafx.h"
#include <moneta/container/meta_set.hxx>
#include <moneta/container/data_trackers/pk_tracker.hxx>
#include <moneta/container/data_trackers/rtuple_data.hxx>
#include <moneta/container/load_trackers/bitset_load_tracker.hxx>
#include <moneta/container/change_trackers/hash_change_tracker.hxx>
#include "../model/Person.hxx"

template <class Master, class EntityType>
struct test_tracker_impl {
	typedef test_tracker_impl this_type;

	struct entry {
		int state;

		bool operator<(const int rhs) const {
			return state < rhs.state;
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

template <class EntityType>
struct test_tracker : boost::mpl::lambda<
	test_tracker_impl<boost::mpl::_1, EntityType>
>::type {};

BOOST_AUTO_TEST_CASE(meta_set_test) {
	typedef moneta::container::meta_set<
		test_tracker<std::string>
	> set_type;

	set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);

	set_type::entry entry;
	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);
}

BOOST_AUTO_TEST_CASE(meta_set_test_2) {
	typedef moneta::container::meta_set<
		moneta::container::pk_tracker<Person>,
		moneta::container::bitset_load_tracker2<Person>,
		moneta::container::rtuple_data<Person>
	> set_type;

	Person joe;
	joe.ID = 123;
	joe.Name = "Joe";
	joe.Fingers = 10;
	joe.Height = 1.80;

	set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);

	set_type::entry entry(joe);

	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	boost::optional<set_type::entry> e0 = set.find(123);
	BOOST_REQUIRE(e0.is_initialized());

	BOOST_CHECK_EQUAL(e0->pk, 123);
	BOOST_CHECK_EQUAL(e0->data, moneta::traits::to_rtuple(joe));

	set.erase(123);
	BOOST_CHECK_EQUAL(set.size(), 0);

	set.insert(entry);
	set.insert(entry);
	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);
}

BOOST_AUTO_TEST_CASE(meta_set_test_3) {
	typedef moneta::container::meta_set<
		moneta::container::pk_tracker<Person>,
		moneta::container::bitset_load_tracker2<Person>,
		moneta::container::rtuple_data<Person>,
		moneta::container::hash_change_tracker2<Person>
	> set_type;

	Person joe;
	joe.ID = 123;
	joe.Name = "Joe";
	joe.Fingers = 10;
	joe.Height = 1.80;

	set_type set;
	BOOST_CHECK_EQUAL(set.size(), 0);

	set_type::entry entry(joe);

	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);

	boost::optional<set_type::entry> e0 = set.find(123);
	BOOST_REQUIRE(e0.is_initialized());

	BOOST_CHECK_EQUAL(e0->pk, 123);
	BOOST_CHECK_EQUAL(e0->data, moneta::traits::to_rtuple(joe));

	set.erase(123);
	BOOST_CHECK_EQUAL(set.size(), 0);

	set.insert(entry);
	set.insert(entry);
	set.insert(entry);
	BOOST_CHECK_EQUAL(set.size(), 1);
}
