#include "stdafx.h"
#include <moneta/algorithm/traverse.hxx>
#include <moneta/codec/debug_dump/stringize_path.hxx>
#include <iostream>
#include "../model/Cat.hxx"
#include "../model/tree/A.hxx"

struct test_state {
	std::vector<std::string> lines;
	size_t enter_count;

	test_state()
	 : enter_count(0) {}
};

struct enter_traverse_test : moneta::algorithm::traverse_enter {
	template <class Path, class Entity>
	void operator()(Entity&, test_state& state) const {
		std::string tmp = "e:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

struct member_traverse_test : moneta::algorithm::traverse_member {
	template <class Path, class Member, class Entity>
	void operator()(Entity&, test_state& state) const {
		std::string tmp = "m:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

struct leave_traverse_test : moneta::algorithm::traverse_leave {
	template <class Path, class Entity>
	void operator()(Entity&, test_state& state) const {
		std::string tmp = "l:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		state.lines.push_back(tmp);
	}
};

BOOST_AUTO_TEST_CASE(traversal_traverse_test) {
	
	A x;

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

	{
		test_state state;

		moneta::algorithm::traverse<
			boost::mpl::vector<
				enter_traverse_test,
				member_traverse_test,
				leave_traverse_test
			>
		>(x, state);

		BOOST_REQUIRE(state.lines.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + 19);
	}

	{
		test_state state;
		const A& const_x = x;
		
		moneta::algorithm::traverse<
			boost::mpl::vector<
				enter_traverse_test,
				member_traverse_test,
				leave_traverse_test
			>
		>(const_x, state);

		BOOST_REQUIRE(state.lines.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(state.lines.begin(), state.lines.end(), expected, expected + 19);
	}
}

BOOST_AUTO_TEST_CASE(stateful_traverse_test) {

	Cat cat;
	test_state state;

	moneta::algorithm::traverse<
		boost::mpl::vector<
			enter_traverse_test,
			member_traverse_test,
			leave_traverse_test
		>
	>(cat, state);

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
