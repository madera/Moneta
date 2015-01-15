#include "stdafx.h"
#include <moneta/codec/debug_dump/stringize_path.hxx>
#include <moneta/algorithm/traverse_ng.hxx>
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

size_t g_enter_count;

struct counting_enter_e {
	template <class Entity>
	void operator()(Entity& entity) const {
		++g_enter_count;
	}
};

struct counting_enter_ep {
	template <class Entity, class Path>
	void operator()(Entity& entity, const Path&) const {
		++g_enter_count;
	}
};

struct counting_enter_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity& entity, const Path&, State& state) const {
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
	void operator()(Member&, Entity& entity) const {
		++g_member_count;
	}
};

struct counting_member_ep {
	template <class Member, class Entity, class Path>
	void operator()(Member&, Entity& entity, const Path&) const {
		++g_member_count;
	}
};

struct counting_member_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(Member&, Entity& entity, const Path&, State& state) const {
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
	void operator()(Entity& entity) const {
		++g_leave_count;
	}
};

struct counting_leave_ep {
	template <class Entity, class Path>
	void operator()(Entity& entity, const Path&) const {
		++g_leave_count;
	}
};

struct counting_leave_eps {
	template <class Entity, class Path, class State>
	void operator()(Entity& entity, const Path&, State& state) const {
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
	void operator()(Member&, Entity& entity) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_ep {
	template <class Member, class Entity, class Path>
	void operator()(Member&, Entity& entity, const Path&) const {
		++g_enter_container_count;
	}
};

struct counting_enter_container_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(Member&, Entity& entity, const Path&, State& state) const {
		++g_enter_container_count;

		std::string tmp = "ec:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

size_t g_container_member_count = 0;

struct counting_container_member_e {
	template <class Member, class Entity>
	void operator()(Member&, Entity& entity) const {
		++g_container_member_count;
	}
};

struct counting_container_member_ep {
	template <class Member, class Entity, class Path>
	void operator()(Member&, Entity& entity, const Path&) const {
		++g_container_member_count;
	}
};

struct counting_container_member_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(Member&, Entity& entity, const Path&, State& state) const {
		++g_container_member_count;

		std::string tmp = "cm:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);

		typedef typename boost::mpl::if_<
			boost::is_const<Entity>,
			typename boost::add_const<typename Member::result_type>::type,
			typename Member::result_type
		>::type container_type;

		typedef typename boost::mpl::if_<
			boost::is_const<Entity>,
			typename container_type::const_iterator,
			typename container_type::iterator
		>::type iterator_type;

		container_type& container = Member()(entity);
		iterator_type itr = container.begin();		
		for ( ; itr != container.end(); ++itr) {
			state.lines.push_back(std::string("cmv:") + *itr);
		}
	}
};

//

size_t g_leave_container_count;

struct counting_leave_container_e {
	template <class Member, class Entity>
	void operator()(Member&, Entity& entity) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_ep {
	template <class Member, class Entity, class Path>
	void operator()(Member&, Entity& entity, const Path&) const {
		++g_leave_container_count;
	}
};

struct counting_leave_container_eps {
	template <class Member, class Entity, class Path, class State>
	void operator()(Member&, Entity& entity, const Path&, State& state) const {
		++g_leave_container_count;

		std::string tmp = "lc:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

//

void reset_counters() {
	g_enter_count = 0;
	g_member_count = 0;
	g_leave_count = 0;
	
	g_enter_container_count = 0;
	g_container_member_count = 0;
	g_leave_container_count = 0;
}

BOOST_AUTO_TEST_CASE(with_container_members_traverse_test) {
	using namespace moneta::algorithm;

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

	typedef moneta::algorithm::traverse<
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
		container_member_actions<
			counting_container_member_e,
			counting_container_member_ep,
			counting_container_member_eps
		>,
		leave_container_actions<
			counting_leave_container_e,
			counting_leave_container_ep,
			counting_leave_container_eps
		>
	> traverse_type;

	reset_counters();

	test_state state;
	traverse_type()(team, state);
	BOOST_CHECK_EQUAL(g_enter_count, 3 * 3);
	BOOST_CHECK_EQUAL(g_member_count, 9 * 3);
	BOOST_CHECK_EQUAL(g_leave_count, 3 * 3);
	BOOST_CHECK_EQUAL(g_enter_container_count, 2 * 3);
	BOOST_CHECK_EQUAL(g_container_member_count, 1 * 3);
	BOOST_CHECK_EQUAL(g_leave_container_count, 2 * 3);

	const char* expected[] = {
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
				"cm:Tags,/SportsTeam::Tags",
				"cmv:tag0",
				"cmv:tag1",
				"cmv:tag2",
			"lc:Tags,/SportsTeam::Tags",
		"l:SportsTeam"
	};

	const size_t expected_size = sizeof(expected) / sizeof(char*);

	BOOST_REQUIRE(state.lines.size() == expected_size);
	BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + expected_size);

	{
		const SportsTeam& const_team = team;
		test_state state;

		traverse_type()(const_team, state);

		BOOST_REQUIRE(state.lines.size() == expected_size);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + expected_size);
	}

}

BOOST_AUTO_TEST_CASE(traverse_test) {
	using namespace moneta::algorithm;
	
	const char* expected[] = {
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
		"l:A"
	};

	typedef moneta::algorithm::traverse<
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
		container_member_actions<
			counting_container_member_e,
			counting_container_member_ep,
			counting_container_member_eps
		>,
		leave_container_actions<
			counting_leave_container_e,
			counting_leave_container_ep,
			counting_leave_container_eps
		>
	> traverse_type;

	{
		test_state state;

		A a;

		traverse_type traverser;
		traverser(a, state);

		BOOST_REQUIRE(state.lines.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + 19);
	}

	{
		test_state state;		
		traverse_type traverser;

		A a;
		const A& ca = a;
		traverser(ca, state);

		BOOST_REQUIRE(state.lines.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + 19);
	}
}

BOOST_AUTO_TEST_CASE(stateful_traverse_test) {
	using namespace moneta::algorithm;

	typedef moneta::algorithm::traverse<
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
		container_member_actions<
			counting_container_member_e,
			counting_container_member_ep,
			counting_container_member_eps
		>,
		leave_container_actions<
			counting_leave_container_e,
			counting_leave_container_ep,
			counting_leave_container_eps
		>
	> traverse_type;

	Cat cat;
	test_state state;
	traverse_type()(cat, state);

	const char* expected[] = {
		"e:Cat",
		"m:ID",
		"m:Name",
		"e:Address,/Cat::Address",
		"m:ID,/Cat::Address",
		"m:Number,/Cat::Address",
		"m:Street,/Cat::Address",
		"l:Address,/Cat::Address",
		"l:Cat"
	};

	BOOST_REQUIRE(state.lines.size() == 9);
	BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + 9);
}
