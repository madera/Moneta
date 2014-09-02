#include "stdafx.h"
#include <moneta/make_entity.hxx>
#include <moneta/lexical/dispatch_member.hxx>
#include "../model/Person.hxx"
#include <boost/lexical_cast.hpp>

template <class Entity>
struct test_lexical_setter {
	Entity& _entity;
	const std::string& _value;

	test_lexical_setter(Entity& entity, const std::string& value)
	 : _entity(entity), _value(value) {}

	template <class Member>
	void operator()() const {
		Member()(_entity) = boost::lexical_cast<typename Member::result_type>(_value);
	}
};

BOOST_AUTO_TEST_CASE(dispatch_member_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Height, 0.0);
	BOOST_CHECK_EQUAL(person.Fingers, 0);

	moneta::lexical::dispatch_member<Person>("ID", test_lexical_setter<Person>(person, "123"));
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	moneta::lexical::dispatch_member<Person>("Name", test_lexical_setter<Person>(person, "Somebody"));
	BOOST_CHECK_EQUAL(person.Name, "Somebody");

	moneta::lexical::dispatch_member<Person>("Height", test_lexical_setter<Person>(person, "1.80"));
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);
	
	moneta::lexical::dispatch_member<Person>("Fingers", test_lexical_setter<Person>(person, "100"));
	BOOST_CHECK_EQUAL(person.Fingers, 100);
}

BOOST_AUTO_TEST_CASE(dispatch_member_ordinal_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.Name, "");
	BOOST_CHECK_EQUAL(person.Height, 0.0);
	BOOST_CHECK_EQUAL(person.Fingers, 0);

	const int id_ordinal = moneta::traits::get_member_name_index<Person>("ID");
	moneta::lexical::dispatch_member<Person>(id_ordinal, test_lexical_setter<Person>(person, "123"));
	BOOST_CHECK_EQUAL(person.ID, 123);
	
	const int name_ordinal = moneta::traits::get_member_name_index<Person>("Name");
	moneta::lexical::dispatch_member<Person>(name_ordinal, test_lexical_setter<Person>(person, "Somebody"));
	BOOST_CHECK_EQUAL(person.Name, "Somebody");

	const int height_ordinal = moneta::traits::get_member_name_index<Person>("Height");
	moneta::lexical::dispatch_member<Person>(height_ordinal, test_lexical_setter<Person>(person, "1.80"));
	BOOST_CHECK_CLOSE(person.Height, 1.80, 0.1);
	
	const int fingers_ordinal = moneta::traits::get_member_name_index<Person>("Fingers");
	moneta::lexical::dispatch_member<Person>(fingers_ordinal, test_lexical_setter<Person>(person, "100"));
	BOOST_CHECK_EQUAL(person.Fingers, 100);
}
