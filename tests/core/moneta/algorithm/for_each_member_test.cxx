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
#include <moneta/algorithm/for_each_member.hxx>
#include "../model/Person.hxx"
#include "../model/Cat.hxx"

// TODO: Continue support for free function operations.
//
//static size_t free_count = 0;
//
//template <class Entity, class Member, class Path>
//void free_counter(Entity& entity) {
//	++free_count;
//	std::cerr << "sadjhfjkadsfhjkadshfjkas" << std::endl;
//}
//
//BOOST_AUTO_TEST_CASE(free_function_for_each_member_test) {
//
//	Cat cat;
//	cat.ID = 10;
//	cat.Name = "Garfield";
//	cat.Address.ID = 20;
//	cat.Address.Number = 200;
//	cat.Address.Street = "St. Street";
//
//	{
//		free_count = 0;
//		moneta::algorithm::for_each_member(cat, free_counter);
//		BOOST_CHECK_EQUAL(free_count, 5);
//	}
//
//	{
//		free_count = 0;
//		const Cat& const_cat = cat;
//		moneta::algorithm::for_each_member(cat, free_counter);
//		BOOST_CHECK_EQUAL(free_count, 5);
//	}
//
//}

struct call_counter {
	size_t& count;

	call_counter(size_t& count_)
	 : count(count_) {}

	template <class Entity, class Member, class Path>
	void operator()(const Entity&) const {
		++count;
	}
};

BOOST_AUTO_TEST_CASE(for_each_member_test) {

	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Ratings = 12;

	{
		size_t count = 0;
		moneta::algorithm::for_each_member(person, call_counter(count));
		BOOST_CHECK_EQUAL(count, 4);
	}

	{
		const Person& const_person = person;
		size_t count = 0;
		moneta::algorithm::for_each_member(const_person, call_counter(count));
		BOOST_CHECK_EQUAL(count, 4);
	}

	Cat cat;
	cat.ID = 10;
	cat.Name = "Garfield";
	cat.Address.ID = 20;
	cat.Address.Number = 200;
	cat.Address.Street = "St. Street";

	{
		size_t count = 0;
		moneta::algorithm::for_each_member(cat, call_counter(count));
		BOOST_CHECK_EQUAL(count, 5);
	}

	{
		const Cat& const_cat = cat;
		size_t count = 0;
		moneta::algorithm::for_each_member(const_cat, call_counter(count));
		BOOST_CHECK_EQUAL(count, 5);
	}

}

//
// Member Incrementor
//

template <class T>
struct increment_it {
	void operator()(T&) {
	}
};

template <>
struct increment_it<int> {
	void operator()(int& x) {
		++x;
	}
};

struct member_incrementor {
	template <class Entity, class Member, class Path>
	void operator()(Entity& entity) const {
		increment_it<typename Member::result_type>()(Member()(entity));
	}
};

BOOST_AUTO_TEST_CASE(mutable_for_each_member_test) {
	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Ratings = 12;

	moneta::algorithm::for_each_member(person, member_incrementor());

	BOOST_CHECK_EQUAL(person.ID, 346);
	BOOST_CHECK_EQUAL(person.Ratings, 13);
}

struct path_tester {
	std::vector<int>& _output;

	path_tester(std::vector<int>& output)
	 : _output(output) {}

	template <class Entity, class Member, class Path>
	void operator()(Entity&) const {
		_output.push_back(boost::mpl::size<Path>::value);
	}
};

BOOST_AUTO_TEST_CASE(path_for_each_member_test) {
	Cat garfield;
	garfield.ID = 1;
	garfield.Name = "Garfield";
	garfield.Address.ID = 10;
	garfield.Address.Number = 123;
	garfield.Address.Street = "Super Street";

	{
		std::vector<int> result;
		moneta::algorithm::for_each_member(garfield, path_tester(result));

		const int expected[] = { 0, 0, 1, 1, 1 };
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 5);
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<int> result;
		moneta::algorithm::for_each_member(const_garfield, path_tester(result));

		const int expected[] = { 0, 0, 1, 1, 1 };
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 5);
	}
}

struct enter_tester {
	std::vector<std::string>& _output;

	enter_tester(std::vector<std::string>& output)
	 : _output(output) {}

	template <class Entity, class Path>
	void enter(Entity&) const {
		_output.push_back(std::string("e:") + moneta::traits::get_entity_name<Entity>());
	}

	template <class Entity, class Member, class Path>
	void operator()(Entity&) const {
		_output.push_back(".");
	}
};

BOOST_AUTO_TEST_CASE(enter_for_each_member_test) {
	Cat garfield;
	garfield.ID = 1;
	garfield.Name = "Garfield";
	garfield.Address.ID = 10;
	garfield.Address.Number = 123;
	garfield.Address.Street = "Super Street";

	{
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(garfield, enter_tester(result));

		BOOST_REQUIRE(result.size() == 7);
		BOOST_CHECK_EQUAL(result[0], "e:Cat");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], "e:Address");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], ".");
		BOOST_CHECK_EQUAL(result[6], ".");
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(const_garfield, enter_tester(result));

		BOOST_REQUIRE(result.size() == 7);
		BOOST_CHECK_EQUAL(result[0], "e:Cat");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], "e:Address");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], ".");
		BOOST_CHECK_EQUAL(result[6], ".");
	}
}

struct leave_tester {
	std::vector<std::string>& _output;

	leave_tester(std::vector<std::string>& output)
	 : _output(output) {}

	template <class Entity, class Member, class Path>
	void operator()(Entity&) const {
		_output.push_back(".");
	}

	template <class Entity, class Path>
	void leave(Entity&) const {
		_output.push_back(std::string("l:") + moneta::traits::get_entity_name<Entity>());
	}
};

BOOST_AUTO_TEST_CASE(leave_for_each_member_test) {
	Cat garfield;
	garfield.ID = 1;
	garfield.Name = "Garfield";
	garfield.Address.ID = 10;
	garfield.Address.Number = 123;
	garfield.Address.Street = "Super Street";

	{
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(garfield, leave_tester(result));

		BOOST_REQUIRE(result.size() == 7);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], "l:Address");
		BOOST_CHECK_EQUAL(result[6], "l:Cat");
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(const_garfield, leave_tester(result));

		BOOST_REQUIRE(result.size() == 7);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], "l:Address");
		BOOST_CHECK_EQUAL(result[6], "l:Cat");
	}
}

struct traversal_tester {
	std::vector<std::string>& _output;

	traversal_tester(std::vector<std::string>& output)
	 : _output(output) {}

	template <class Entity, class Path>
	void enter(Entity&) const {
		_output.push_back(std::string("e:") + moneta::traits::get_entity_name<Entity>());
	}

	template <class Entity, class Member, class Path>
	void operator()(Entity&) const {
		_output.push_back(std::string("m:") + moneta::traits::member_name<Member>::get());
	}

	template <class Entity, class Path>
	void leave(Entity&) const {
		_output.push_back(std::string("l:") + moneta::traits::get_entity_name<Entity>());
	}
};

#include <moneta/pp/describe_entity.hxx>

MONETA_DEFINE_ENTITY(
	E,
	((int, m))
	((int, n))
)

MONETA_DEFINE_ENTITY(
	D,
	((int, l))
	((E  , e))
)

MONETA_DEFINE_ENTITY(
	C,
	((int, j))
	((int, k))
)

MONETA_DEFINE_ENTITY(
	B,
	((C,   c))
	((int, i))
	((D  , d))
)

MONETA_DEFINE_ENTITY(
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

BOOST_AUTO_TEST_CASE(traversal_for_each_member_test) {
	
	A x;

	const char* expected[] = {
		"e:A", "m:f", "m:g", "e:B", "e:C", "m:j", "m:k", "l:C", "m:i", "e:D",
		"m:l", "e:E", "m:m", "m:n", "l:E", "l:D", "l:B", "m:h", "l:A"
	};

	{
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(x, traversal_tester(result));

		BOOST_REQUIRE(result.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 19);
	}

	{
		const A& const_x = x;
		std::vector<std::string> result;
		moneta::algorithm::for_each_member(const_x, traversal_tester(result));

		BOOST_REQUIRE(result.size() == 19);
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 19);
	}
}
