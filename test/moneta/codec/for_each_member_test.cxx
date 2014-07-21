#include "stdafx.h"
#include <moneta/codec/for_each_member.hxx>

#include "../model/Person.hxx"
#include "../model/Cat.hxx"

struct call_counter {
	size_t& count;

	call_counter(size_t& count_)
	 : count(count_) {}

	template <class Entity, class Member>
	void operator()(const Entity& entity, Member member) const {
		++count;
	}
};

BOOST_AUTO_TEST_CASE(for_each_member_test) {

	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Fingers = 12;

	{
		size_t count = 0;
		moneta::codec::for_each_member(person, call_counter(count));
		BOOST_CHECK_EQUAL(count, 4);
	}

	{
		const Person& const_person = person;
		size_t count = 0;
		moneta::codec::for_each_member(const_person, call_counter(count));
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
		moneta::codec::for_each_member(cat, call_counter(count));
		BOOST_CHECK_EQUAL(count, 5);
	}

	{
		const Cat& const_cat = cat;
		size_t count = 0;
		moneta::codec::for_each_member(const_cat, call_counter(count));
		BOOST_CHECK_EQUAL(count, 5);
	}

}

//
// Member Incrementor
//

template <class T>
struct increment_it {
	void operator()(T& x) {
	}
};

template <>
struct increment_it<int> {
	void operator()(int& x) {
		++x;
	}
};

struct member_incrementor {
	template <class Entity, class Member>
	void operator()(Entity& entity, Member member) const {
		increment_it<typename Member::result_type>()(member(entity));
	}
};

BOOST_AUTO_TEST_CASE(mutable_for_each_member_test) {
	Person person;
	person.ID = 345;
	person.Name = "Anonymous";
	person.Height = 1.75;
	person.Fingers = 12;

	moneta::codec::for_each_member(person, member_incrementor());

	BOOST_CHECK_EQUAL(person.ID, 346);
	BOOST_CHECK_EQUAL(person.Fingers, 13);
}

struct path_tester {
	std::vector<int>& _output;

	path_tester(std::vector<int>& output)
	 : _output(output) {}

	template <class Entity, class Member, class Path>
	void operator()(Entity& entity, Member member, Path path) const {
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
		moneta::codec::for_each_member(garfield, path_tester(result));

		const int expected[] = { 0, 0, 1, 1, 1 };
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 5);
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<int> result;
		moneta::codec::for_each_member(const_garfield, path_tester(result));

		const int expected[] = { 0, 0, 1, 1, 1 };
		BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected, expected + 5);
	}
}

struct enter_tester {
	std::vector<std::string>& _output;

	enter_tester(std::vector<std::string>& output)
	 : _output(output) {}

	template <class FromEntity, class ToEntity, class Member>
	void enter(FromEntity& from, ToEntity& to, Member member) const {
		std::ostringstream oss;
		oss << "From " << moneta::traits::get_entity_name<FromEntity>()
		    << " to "  << moneta::traits::get_entity_name<ToEntity>();

		_output.push_back(oss.str());
	}

	template <class Entity, class Member, class Path>
	void operator()(Entity& entity, Member member, Path path) const {
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
		moneta::codec::for_each_member(garfield, enter_tester(result));

		BOOST_REQUIRE(result.size() == 6);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], "From Cat to Address");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], ".");
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<std::string> result;
		moneta::codec::for_each_member(const_garfield, enter_tester(result));

		BOOST_REQUIRE(result.size() == 6);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], "From Cat to Address");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], ".");
	}
}

struct leave_tester {
	std::vector<std::string>& _output;

	leave_tester(std::vector<std::string>& output)
	 : _output(output) {}

	template <class FromEntity, class ToEntity, class Member>
	void leave(FromEntity& from, ToEntity& to, Member member) const {
		std::ostringstream oss;
		oss << "From " << moneta::traits::get_entity_name<FromEntity>()
		    << " to "  << moneta::traits::get_entity_name<ToEntity>();

		_output.push_back(oss.str());
	}

	template <class Entity, class Member, class Path>
	void operator()(Entity& entity, Member member, Path path) const {
		_output.push_back(".");
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
		moneta::codec::for_each_member(garfield, leave_tester(result));

		BOOST_REQUIRE(result.size() == 6);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], "From Address to Cat");
	}

	{
		const Cat& const_garfield = garfield;
		std::vector<std::string> result;
		moneta::codec::for_each_member(const_garfield, leave_tester(result));

		BOOST_REQUIRE(result.size() == 6);
		BOOST_CHECK_EQUAL(result[0], ".");
		BOOST_CHECK_EQUAL(result[1], ".");
		BOOST_CHECK_EQUAL(result[2], ".");
		BOOST_CHECK_EQUAL(result[3], ".");
		BOOST_CHECK_EQUAL(result[4], ".");
		BOOST_CHECK_EQUAL(result[5], "From Address to Cat");
	}
}
