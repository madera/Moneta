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
#include "../model/tree/A.hxx"
#include "../model/Cat.hxx"
#include "../model/SportsTeam.hxx"
#include "../model/Customer.hxx"
#include <moneta/algorithm/detail/stringize_path.hxx>
#include <moneta/algorithm/traverse.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <vector>
#include <string>

inline void static_test() {
	using namespace moneta::algorithm;

	//
	// Testing moneta::algorithm::detail::actions_of<>
	//
	{
		// FIXME: This is some code that shouldn't be able to compile.
		//        Test differently. Use some real handlers.
		typedef moneta::algorithm::traverse<
			enter_actions<char, int, double>,
			member_actions<int>,
			leave_actions<double>,
			enter_actions<bool>
		> traverse_type;


		typedef detail::actions_of<
			traverse_type::mpl_vector,
			detail::traverse_enter
		>::type enter_actions;

		BOOST_MPL_ASSERT((boost::mpl::equal<
			enter_actions,
			boost::mpl::vector<char, int, double, bool>
		>));


		typedef detail::actions_of<
			traverse_type::mpl_vector,
			detail::traverse_member
		>::type member_actions;

		BOOST_MPL_ASSERT((boost::mpl::equal<
			member_actions,
			boost::mpl::vector<int>
		>));


		typedef detail::actions_of<
			traverse_type::mpl_vector,
			detail::traverse_leave
		>::type leave_actions;

		BOOST_MPL_ASSERT((boost::mpl::equal<
			leave_actions,
			boost::mpl::vector<double>
		>));
	}

}

struct test_state {
	std::vector<std::string> lines;
};

//

size_t g_start_count;

struct counting_start_e {
	template <class Entity>
	void operator()(Entity&) const {
		++g_start_count;
	}
};

struct counting_start_ep {
	template <class Entity, class Path>
	void operator()(Entity&, Path) const {
		++g_start_count;
	}
};

struct counting_start_esp {
	template <class Entity, class State, class Path>
	void operator()(Entity&, State& state, Path) const {
		++g_start_count;

		std::string tmp = "S:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_enter_count;

struct counting_enter_e {
	template <class Entity>
	void operator()(Entity&) const {
		++g_enter_count;
	}
};

struct counting_enter_ep {
	template <class Entity, class Path>
	void operator()(Entity&, Path) const {
		++g_enter_count;
	}
};

struct counting_enter_esp {
	template <class Entity, class State, class Path>
	void operator()(Entity&, State& state, Path) const {
		++g_enter_count;

		std::string tmp = "e:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_member_count = 0;

struct counting_member_em {
	template <class Entity, class Member>
	void operator()(Entity&, Member) const {
		++g_member_count;
	}
};

struct counting_member_emp {
	template <class Entity, class Member, class Path>
	void operator()(Entity&, Member, Path) const {
		++g_member_count;
	}
};

struct counting_member_esmp {
	template <class Entity, class State, class Member, class Path>
	void operator()(Entity&, State& state, Member, Path) const {
		++g_member_count;

		std::string tmp = "m:" + moneta::traits::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_present_member_count = 0;

struct counting_present_member_evm {
	template <class Entity, class Value, class Member>
	void operator()(Entity&, Value&, Member) const {
		++g_present_member_count;
	}
};

struct counting_present_member_evmp {
	template <class Entity, class Value, class Member, class Path>
	void operator()(Entity&, Value&, Member, Path) const {
		++g_present_member_count;
	}
};

struct counting_present_member_evsmp {
	template <class Entity, class Value, class State, class Member, class Path>
	void operator()(Entity&, Value&, State& state, Member, Path) const {
		++g_present_member_count;

		std::string tmp = "pm:" + moneta::traits::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_leave_count = 0;

struct counting_leave_e {
	template <class Entity>
	void operator()(Entity&) const {
		++g_leave_count;
	}
};

struct counting_leave_ep {
	template <class Entity, class Path>
	void operator()(Entity&, Path) const {
		++g_leave_count;
	}
};

struct counting_leave_esp {
	template <class Entity, class State, class Path>
	void operator()(Entity&, State& state, Path) const {
		++g_leave_count;

		std::string tmp = "l:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//
//
//

size_t g_enter_container_count;

struct counting_enter_container_em {
	template <class Entity, class Member>
	void operator()(Entity&, Member) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_emp {
	template <class Entity, class Member, class Path>
	void operator()(Entity&, Member, Path) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_esmp {
	template <class Entity, class State, class Member, class Path>
	void operator()(Entity&, State& state, Member, Path) const {
		++g_enter_container_count;

		std::string tmp = "ec:" + moneta::traits::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_container_item_count = 0;

struct counting_container_item_evm {
	template <class Entity, typename Value, class Member>
	void operator()(Entity&, Value&, Member) const {
		++g_container_item_count;
	}
};

struct counting_container_item_evmp {
	template <class Entity, typename Value, class Member, class Path>
	void operator()(Entity&, Value&, Member, Path) const {
		++g_container_item_count;
	}
};

struct counting_container_item_evsmp {
	template <class Entity, typename Value, class State, class Member, class Path>
	void operator()(Entity&, Value& value, State& state, Member, Path) const {
		++g_container_item_count;

		std::string tmp = "ci:" + value;
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_leave_container_count;

struct counting_leave_container_em {
	template <class Entity, class Member>
	void operator()(Entity&, Member) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_emp {
	template <class Entity, class Member, class Path>
	void operator()(Entity&, Member, Path) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_esmp {
	template <class Entity, class State, class Member, class Path>
	void operator()(Entity&, State& state, Member, Path) const {
		++g_leave_container_count;

		std::string tmp = "lc:" + moneta::traits::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_finish_count;

struct counting_finish_e {
	template <class Entity>
	void operator()(Entity&) const {
		++g_finish_count;
	}
};

struct counting_finish_ep {
	template <class Entity, class Path>
	void operator()(Entity&, Path) const {
		++g_finish_count;
	}
};

struct counting_finish_esp {
	template <class Entity, class State, class Path>
	void operator()(Entity&, State& state, Path) const {
		++g_finish_count;

		std::string tmp = "F:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

void reset_counters() {
	g_start_count = 0;

	g_enter_count = 0;
	g_member_count = 0;
	g_leave_count = 0;
	
	g_enter_container_count = 0;
	g_container_item_count = 0;
	g_leave_container_count = 0;

	g_finish_count = 0;
}

using namespace moneta::algorithm;
typedef moneta::algorithm::traverse<
	start_actions<
		counting_start_e,
		counting_start_ep,
		counting_start_esp
	>,
	enter_actions<
		counting_enter_e,
		counting_enter_ep,
		counting_enter_esp
	>,
	member_actions<
		counting_member_em,
		counting_member_emp,
		counting_member_esmp
	>,
	leave_actions<
		counting_leave_e,
		counting_leave_ep,
		counting_leave_esp
	>,
	enter_container_actions<
		counting_enter_container_em,
		counting_enter_container_emp,
		counting_enter_container_esmp
	>,
	container_item_actions<
		counting_container_item_evm,
		counting_container_item_evmp,
		counting_container_item_evsmp
	>,
	leave_container_actions<
		counting_leave_container_em,
		counting_leave_container_emp,
		counting_leave_container_esmp
	>,
	finish_actions<
		counting_finish_e,
		counting_finish_ep,
		counting_finish_esp
	>
> traverse_type;

BOOST_AUTO_TEST_CASE(with_container_members_traverse_test) {
	Person jordan;
	jordan.Name = "Michael Jordan";
	jordan.Height = 2.00;
	
	Person johnson;
	johnson.Name = "Magic Johnson";
	johnson.Height = 2.10;

	SportsTeam team;
	team.Players.push_back(jordan);
	team.Players.push_back(johnson);

	team.Tags.push_back("tag0");
	team.Tags.push_back("tag1");
	team.Tags.push_back("tag2");

	reset_counters();

	test_state state;
	traverse_type()(team, state);
	BOOST_CHECK_EQUAL(g_start_count, 3);
	BOOST_CHECK_EQUAL(g_enter_count, 3 * 3);
	BOOST_CHECK_EQUAL(g_member_count, 9 * 3);
	BOOST_CHECK_EQUAL(g_leave_count, 3 * 3);
	BOOST_CHECK_EQUAL(g_enter_container_count, 2 * 3);
	BOOST_CHECK_EQUAL(g_container_item_count, 3 * 3);
	BOOST_CHECK_EQUAL(g_leave_container_count, 2 * 3);
	BOOST_CHECK_EQUAL(g_finish_count, 3);

	const char* expected[] = {
		"S:SportsTeam",
			"e:SportsTeam",
				"m:Name",
				"ec:Players,/SportsTeam::Players",
					"e:Person,/SportsTeam::Players",
						"m:ID,/SportsTeam::Players",
						"m:Name,/SportsTeam::Players",
						"m:Height,/SportsTeam::Players",
						"m:Ratings,/SportsTeam::Players",
					"l:Person,/SportsTeam::Players",
					"e:Person,/SportsTeam::Players",
						"m:ID,/SportsTeam::Players",
						"m:Name,/SportsTeam::Players",
						"m:Height,/SportsTeam::Players",
						"m:Ratings,/SportsTeam::Players",
					"l:Person,/SportsTeam::Players",
				"lc:Players,/SportsTeam::Players",
				"ec:Tags,/SportsTeam::Tags",
					"ci:tag0,/SportsTeam::Tags",
					"ci:tag1,/SportsTeam::Tags",
					"ci:tag2,/SportsTeam::Tags",
				"lc:Tags,/SportsTeam::Tags",
			"l:SportsTeam",
		"F:SportsTeam"
	};

	const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));

	BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
	BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);

	{
		const SportsTeam& const_team = team;
		test_state state;

		traverse_type()(const_team, state);

		BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);
	}

}

BOOST_AUTO_TEST_CASE(traverse_test) {
	const char* expected[] = {
		"S:A",
			"e:A",
				"m:f",
				"m:g",
				"e:B,/A::b",
					"e:C,/A::b/B::c",
						"m:j,/A::b/B::c",
						"m:k,/A::b/B::c",
					"l:C,/A::b/B::c",
					"m:i,/A::b",
					"e:D,/A::b/B::d",
						"m:l,/A::b/B::d",
						"e:E,/A::b/B::d/D::e",
							"m:m,/A::b/B::d/D::e",
							"m:n,/A::b/B::d/D::e",
						"l:E,/A::b/B::d/D::e",
					"l:D,/A::b/B::d",
				"l:B,/A::b",
				"m:h",
			"l:A",
		"F:A"
	};

	const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));

	{
		test_state state;

		A a;

		traverse_type traverser;
		traverser(a, state);

		BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);
	}

	{
		test_state state;		
		traverse_type traverser;

		A a;
		const A& ca = a;
		traverser(ca, state);

		BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);
	}
}

BOOST_AUTO_TEST_CASE(stateful_traverse_test) {
	Cat cat;
	test_state state;
	traverse_type()(cat, state);

	const char* expected[] = {
		"S:Cat",
			"e:Cat",
				"m:ID",
				"m:Name",
				"e:Address,/Cat::Address",
					"m:ID,/Cat::Address",
					"m:Number,/Cat::Address",
					"m:Street,/Cat::Address",
				"l:Address,/Cat::Address",
			"l:Cat",
		"F:Cat"
	};

	const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));
	BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
	BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);
}

BOOST_AUTO_TEST_CASE(test_moneta_algorithm_traverse_present_members) {
	typedef moneta::algorithm::traverse<
		start_actions<
			counting_start_e,
			counting_start_ep,
			counting_start_esp
		>,
		enter_actions<
			counting_enter_e,
			counting_enter_ep,
			counting_enter_esp
		>,
		member_actions<
			counting_member_em,
			counting_member_emp,
			counting_member_esmp
		>,
		present_member_actions<
			counting_present_member_evm,
			counting_present_member_evmp,
			counting_present_member_evsmp
		>,
		leave_actions<
			counting_leave_e,
			counting_leave_ep,
			counting_leave_esp
		>,
		enter_container_actions<
			counting_enter_container_em,
			counting_enter_container_emp,
			counting_enter_container_esmp
		>,
		container_item_actions<
			counting_container_item_evm,
			counting_container_item_evmp,
			counting_container_item_evsmp
		>,
		leave_container_actions<
			counting_leave_container_em,
			counting_leave_container_emp,
			counting_leave_container_esmp
		>,
		finish_actions<
			counting_finish_e,
			counting_finish_ep,
			counting_finish_esp
		>
	> traverse_type;

	{
		reset_counters();

		Customer customer;

		test_state state;
		traverse_type()(customer, state);

		const char* expected[] = {
			"S:Customer",
				"e:Customer",
					 "m:Name",
					"pm:Name",
					 "m:DOB",
					"pm:DOB",
					 "m:Rating",
				"l:Customer",
			"F:Customer"
		};

		const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));
		BOOST_CHECK_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);

		BOOST_CHECK_EQUAL(g_start_count, 1 * 3);
		BOOST_CHECK_EQUAL(g_enter_count, 1 * 3);
		BOOST_CHECK_EQUAL(g_member_count, 3 * 3);
		BOOST_CHECK_EQUAL(g_leave_count, 1 * 3);
		BOOST_CHECK_EQUAL(g_enter_container_count, 0);
		BOOST_CHECK_EQUAL(g_container_item_count, 0);
		BOOST_CHECK_EQUAL(g_leave_container_count, 0);
		BOOST_CHECK_EQUAL(g_finish_count, 1 * 3);
	}

	{
		reset_counters();

		Address address;
		address.ID = 0;
		address.Number = 1;
		address.Street = "Something St.";

		Customer customer;
		customer.HomeAddress = address;
		customer.Rating = 10;

		test_state state;
		traverse_type()(customer, state);

		const char* expected[] = {
			"S:Customer",
				"e:Customer",
					 "m:Name",
					"pm:Name",
					 "m:DOB",
					"pm:DOB",
					"e:Address,/Customer::HomeAddress",
						 "m:ID,/Customer::HomeAddress",
						"pm:ID,/Customer::HomeAddress",
						 "m:Number,/Customer::HomeAddress",
						"pm:Number,/Customer::HomeAddress",
						 "m:Street,/Customer::HomeAddress",
						"pm:Street,/Customer::HomeAddress",
					"l:Address,/Customer::HomeAddress",
					 "m:Rating",
					"pm:Rating",
				"l:Customer",
			"F:Customer"
		};

		const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));
		BOOST_CHECK_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);

		BOOST_CHECK_EQUAL(g_start_count, 1 * 3);
		BOOST_CHECK_EQUAL(g_enter_count, 2 * 3);
		BOOST_CHECK_EQUAL(g_member_count, 6 * 3);
		BOOST_CHECK_EQUAL(g_leave_count, 2 * 3);
		BOOST_CHECK_EQUAL(g_enter_container_count, 0);
		BOOST_CHECK_EQUAL(g_container_item_count, 0);
		BOOST_CHECK_EQUAL(g_leave_container_count, 0);
		BOOST_CHECK_EQUAL(g_finish_count, 1 * 3);
	}
	{
		reset_counters();

		Address address;
		address.ID = 0;
		address.Number = 1;
		address.Street = "Something St.";

		Customer customer;
		customer.HomeAddress = address;
		customer.Rating = 10;
		customer.Dogs = std::vector<Dog>();
		
		Dog lassie;
		lassie.ID = 1;
		lassie.Name = "Lassie";
		lassie.Owner = "Somebody";
		customer.Dogs->push_back(lassie);

		Dog yoddy;
		yoddy.ID = 1;
		yoddy.Name = "Yoddy";
		yoddy.Owner = "Sam";
		customer.Dogs->push_back(yoddy);

		customer.Tags = std::vector<std::string>();
		customer.Tags->push_back("tag0");
		customer.Tags->push_back("tag1");
		customer.Tags->push_back("tag2");

		test_state state;
		traverse_type()(customer, state);

		const char* expected[] = {
			"S:Customer",
				"e:Customer",
					 "m:Name",
					"pm:Name",
					 "m:DOB",
					"pm:DOB",
					"e:Address,/Customer::HomeAddress",
						 "m:ID,/Customer::HomeAddress",
						"pm:ID,/Customer::HomeAddress",
						 "m:Number,/Customer::HomeAddress",
						"pm:Number,/Customer::HomeAddress",
						 "m:Street,/Customer::HomeAddress",
						"pm:Street,/Customer::HomeAddress",
					"l:Address,/Customer::HomeAddress",
					 "m:Rating",
					"pm:Rating",
					"ec:Dogs,/Customer::Dogs",
						"e:Dog,/Customer::Dogs",
							 "m:Owner,/Customer::Dogs",
							"pm:Owner,/Customer::Dogs",
							 "m:ID,/Customer::Dogs",
							"pm:ID,/Customer::Dogs",
							 "m:Name,/Customer::Dogs",
							"pm:Name,/Customer::Dogs",
						"l:Dog,/Customer::Dogs",
						"e:Dog,/Customer::Dogs",
							 "m:Owner,/Customer::Dogs",
							"pm:Owner,/Customer::Dogs",
							 "m:ID,/Customer::Dogs",
							"pm:ID,/Customer::Dogs",
							 "m:Name,/Customer::Dogs",
							"pm:Name,/Customer::Dogs",
						"l:Dog,/Customer::Dogs",
					"lc:Dogs,/Customer::Dogs",
					"ec:Tags,/Customer::Tags",
						"ci:tag0,/Customer::Tags",
						"ci:tag1,/Customer::Tags",
						"ci:tag2,/Customer::Tags",
					"lc:Tags,/Customer::Tags",
				"l:Customer",
			"F:Customer"
		};

		const size_t line_count = std::distance(boost::begin(expected), boost::end(expected));
		BOOST_CHECK_EQUAL(state.lines.size(), line_count);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);

		BOOST_CHECK_EQUAL(g_start_count, 1 * 3);
		BOOST_CHECK_EQUAL(g_enter_count, 4 * 3);
		BOOST_CHECK_EQUAL(g_member_count, 12 * 3);
		BOOST_CHECK_EQUAL(g_leave_count, 4 * 3);
		BOOST_CHECK_EQUAL(g_enter_container_count, 2 * 3);
		BOOST_CHECK_EQUAL(g_container_item_count, 3 * 3);
		BOOST_CHECK_EQUAL(g_leave_container_count, 2 * 3);
		BOOST_CHECK_EQUAL(g_finish_count, 1 * 3);
	}
}
