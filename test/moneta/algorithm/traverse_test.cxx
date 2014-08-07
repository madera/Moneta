#include "stdafx.h"
#include <moneta/algorithm/traverse.hxx>
#include <moneta/codec/debug_dump/stringize_path.hxx>
#include "../model/Cat.hxx"
#include <iostream>

std::vector<std::string> lines;

struct enter_traverse_test {
	template <class Entity, class Path>
	void operator()(Entity&) const {
		std::string tmp = "e:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		lines.push_back(tmp);
	}
};

struct entity_traverse_test {
	template <class Entity>
	void operator()(Entity&) const {
		lines.push_back("custom entity:" << typeid(Entity).name());
	}
};

struct member_traverse_test {
	template <class Entity, class Member, class Path>
	void operator()(Entity& entity) const {
		std::string tmp = "m:" + moneta::traits::detail::member_name<Member>::get();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		lines.push_back(tmp);
	}
};

struct leave_traverse_test {
	template <class Entity, class Path>
	void operator()(Entity&) const {
		std::string tmp = "l:" + moneta::traits::get_entity_name<Entity>();
		const std::string path = moneta::codec::detail::stringize_path<Path>();
		tmp += (path.empty()? "" : "," + path);
		lines.push_back(tmp);
	}
};

struct traits {
	typedef enter_traverse_test enter;
	//typedef entity_traverse_test entity;
	typedef member_traverse_test member;
	typedef leave_traverse_test leave;
};

BOOST_AUTO_TEST_CASE(simple_traverse_test) {
	Cat cat;
	moneta::algorithm::traverse_with_path<traits>(cat);

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

	BOOST_REQUIRE(lines.size() == 9);
	BOOST_CHECK_EQUAL_COLLECTIONS(lines.begin(), lines.end(), expected, expected + 9);
}

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	E,
	((int, m))
	((int, n))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	D,
	((int, l))
	((E  , e))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	C,
	((int, j))
	((int, k))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	B,
	((C,   c))
	((int, i))
	((D  , d))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	A,
	((int, f))
	((int, g))
	((B,   b))
	((int, h))
)

// Test tree:
//
//     A________
//    / \ \     \
//   f  g  B__   h
//        / \ \
//       C  i  D
//     / |    / \
//    j  k   l   E
//              / \
//             m   n
//

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
		lines.clear();
		moneta::algorithm::traverse_with_path<traits>(x);

		BOOST_REQUIRE(lines.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(lines.begin(), lines.end(), expected, expected + 19);
	}

	//{
	//	const A& const_x = x;
	//	
	//	lines.clear();
	//	moneta::algorithm::traverse_with_path<traits>(const_x);

	//	BOOST_REQUIRE(lines.size() == 19);
	//	BOOST_CHECK_EQUAL_COLLECTIONS(lines.begin(), lines.end(), expected, expected + 19);
	//}
}
