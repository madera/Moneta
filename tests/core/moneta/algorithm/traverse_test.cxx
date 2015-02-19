#include "stdafx.h"
#include <moneta/algorithm/detail/stringize_path.hxx>
#include <moneta/algorithm/traverse.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <vector>
#include <string>
#include "../model/tree/A.hxx"
#include "../model/Cat.hxx"
#include "../model/SportsTeam.hxx"

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
	void operator()(Entity&, const Path&) const {
		++g_start_count;
	}
};

struct counting_start_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity&, const Path&, State& state) const {
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
	void operator()(Entity&, const Path&) const {
		++g_enter_count;
	}
};

struct counting_enter_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity&, const Path&, State& state) const {
		++g_enter_count;

		std::string tmp = "e:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_member_count = 0;

struct counting_member_e {
	template <class Member, class Entity>
	void operator()(const Member&, Entity&) const {
		++g_member_count;
	}
};

struct counting_member_ep {
	template <class Member, class Entity, class Path>
	void operator()(const Member&, Entity&, const Path&) const {
		++g_member_count;
	}
};

struct counting_member_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(const Member&, Entity&, const Path&, State& state) const {
		++g_member_count;

		std::string tmp = "m:" + moneta::traits::detail::member_name<Member>::get();
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
	void operator()(Entity&, const Path&) const {
		++g_leave_count;
	}
};

struct counting_leave_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity&, const Path&, State& state) const {
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

struct counting_enter_container_e {
	template <class Member, class Entity>
	void operator()(const Member&, Entity&) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_ep {
	template <class Member, class Entity, class Path>
	void operator()(const Member&, Entity&, const Path&) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(const Member&, Entity&, const Path&, State& state) const {
		++g_enter_container_count;

		std::string tmp = "ec:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_container_item_count = 0;

struct counting_container_item_e {
	template <class Value, class Member, class Entity>
	void operator()(Value&, const Member&, Entity&) const {
		++g_container_item_count;
	}
};

struct counting_container_item_ep {
	template <class Value, class Member, class Entity, class Path>
	void operator()(Value&, const Member&, Entity&, const Path&) const {
		++g_container_item_count;
	}
};

struct counting_container_item_eps {
	template <class Value, class Member, class Entity, class Path, class State>
	void operator()(Value& value, const Member&, Entity&, const Path&, State& state) const {
		++g_container_item_count;

		std::string tmp = "ci:" + value;
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_leave_container_count;

struct counting_leave_container_e {
	template <class Member, class Entity>
	void operator()(const Member&, Entity&) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_ep {
	template <class Member, class Entity, class Path>
	void operator()(const Member&, Entity&, const Path&) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(const Member&, Entity&, const Path&, State& state) const {
		++g_leave_container_count;

		std::string tmp = "lc:" + moneta::traits::detail::member_name<Member>::get();
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
	void operator()(Entity&, const Path&) const {
		++g_finish_count;
	}
};

struct counting_finish_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity&, const Path&, State& state) const {
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
		counting_start_eps
	>,
	enter_actions<
		counting_enter_e,
		counting_enter_ep,
		counting_enter_eps
	>,
	member_actions<
		counting_member_e,
		counting_member_ep,
		counting_member_eps
	>,
	leave_actions<
		counting_leave_e,
		counting_leave_ep,
		counting_leave_eps
	>,
	enter_container_actions<
		counting_enter_container_e,
		counting_enter_container_ep,
		counting_enter_container_eps
	>,
	container_item_actions<
		counting_container_item_e,
		counting_container_item_ep,
		counting_container_item_eps
	>,
	leave_container_actions<
		counting_leave_container_e,
		counting_leave_container_ep,
		counting_leave_container_eps
	>,
	finish_actions<
		counting_finish_e,
		counting_finish_ep,
		counting_finish_eps
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
						"m:Fingers,/SportsTeam::Players",
					"l:Person,/SportsTeam::Players",
					"e:Person,/SportsTeam::Players",
						"m:ID,/SportsTeam::Players",
						"m:Name,/SportsTeam::Players",
						"m:Height,/SportsTeam::Players",
						"m:Fingers,/SportsTeam::Players",
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

	const size_t line_count = std::distance(std::begin(expected), std::end(expected));

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

	const size_t line_count = std::distance(std::begin(expected), std::end(expected));

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

	const size_t line_count = std::distance(std::begin(expected), std::end(expected));
	BOOST_REQUIRE_EQUAL(state.lines.size(), line_count);
	BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + line_count);
}
