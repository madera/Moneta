#include "stdafx.h"
#include <moneta/make_entity.hxx>
#include "model/Person.hxx"
#include "model/Dog.hxx"

struct Composite {
	int x;
	char y;
	short z;
	Person Person;
	Dog Dog;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Composite> : boost::mpl::vector<
		MONETA_MEMBER(Composite, int,    x     ),
		MONETA_MEMBER(Composite, char,   y     ),
		MONETA_MEMBER(Composite, short,  z     ),
		MONETA_MEMBER(Composite, Person, Person),
		MONETA_MEMBER(Composite, Dog,    Dog   )
	> {};
}}}

BOOST_AUTO_TEST_CASE(make_entity_test) {
	Person person = moneta::make_entity<Person>();
	BOOST_CHECK_EQUAL(person.ID, 0);
	BOOST_CHECK(person.Name.empty());
	BOOST_CHECK_EQUAL(person.Height, 0.00);
	BOOST_CHECK_EQUAL(person.Fingers, 0);

	Dog dog = moneta::make_entity<Dog>();
	BOOST_CHECK(dog.Owner.empty());
	BOOST_CHECK_EQUAL(dog.ID, 0);
	BOOST_CHECK(dog.Name.empty());

	Composite composite = moneta::make_entity<Composite>();
	BOOST_CHECK_EQUAL(composite.x, 0);
	BOOST_CHECK_EQUAL(composite.y, 0);
	BOOST_CHECK_EQUAL(composite.z, 0);

	BOOST_CHECK_EQUAL(composite.Person.ID, 0);
	BOOST_CHECK(composite.Person.Name.empty());
	BOOST_CHECK_EQUAL(composite.Person.Height, 0.00);
	BOOST_CHECK_EQUAL(composite.Person.Fingers, 0);

	BOOST_CHECK(composite.Dog.Owner.empty());
	BOOST_CHECK_EQUAL(composite.Dog.ID, 0);
	BOOST_CHECK(composite.Dog.Name.empty());
}
