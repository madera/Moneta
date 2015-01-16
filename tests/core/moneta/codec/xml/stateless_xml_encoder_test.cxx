#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_encoder.hxx>
#include <moneta/make_entity.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/tree/A.hxx"
#include <moneta/serialization/detail/hexdump.hxx>

MONETA_XML_ATTIBUTE(Composite, MONETA_MEMBER(Composite, int, Identifier))
MONETA_XML_ATTIBUTE(Person, MONETA_MEMBER(Person, int, ID))

MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, std::string, Owner))
MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, int        , ID   ))

// Copied from XML traits test.
//
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, f))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, g))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, h))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(D, int, l))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, m))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, n))

static Composite make_composite() {
	Composite composite;
	composite.Identifier = 2600;
	composite.Person.ID = 123;
	composite.Person.Name = "Somedude";
	composite.Person.Height = 1.50;
	composite.Person.Fingers = 10;
	composite.Dog.Owner = "Someowner";
	composite.Dog.ID = 555;
	composite.Dog.Name = "Doggy";
	return composite;
}

using namespace moneta::codec;
typedef encoder<
	enter_actions<stateless_xml_encoder::enter_entity_encoder>,
	member_actions<stateless_xml_encoder::member_encoder>,
	leave_actions<stateless_xml_encoder::leave_entity_encoder>,
	enter_container_actions<stateless_xml_encoder::enter_container_encoder>,
	container_member_actions<stateless_xml_encoder::container_member_encoder>,
	leave_container_actions<stateless_xml_encoder::leave_container_encoder>
> encoder_t;

BOOST_AUTO_TEST_CASE(stateless_xml_encoder_basic_test) {
	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, A());

	static const std::string expected =
		"<A f=\"0\" g=\"0\" h=\"0\">\n"
		"\t<B>\n"
		"\t\t<C>\n"
		"\t\t\t<j>0</j>\n"
		"\t\t\t<k>0</k>\n"
		"\t\t</C>\n"
		"\t\t<i>0</i>\n"
		"\t\t<D l=\"0\">\n"
		"\t\t\t<E m=\"0\" n=\"0\" />\n"
		"\t\t</D>\n"
		"\t</B>\n"
		"</A>\n"
	;

	BOOST_CHECK_EQUAL(expected.size(), result);
	BOOST_CHECK_EQUAL(expected, buffer);
}


BOOST_AUTO_TEST_CASE(composite_encode_stateless_xml_encoder_test) {
	static const std::string expected =
		"<Composite Identifier=\"2600\">\n"
		"\t<Person ID=\"123\">\n"
		"\t\t<Name>Somedude</Name>\n"
		"\t\t<Height>1.5</Height>\n"
		"\t\t<Fingers>10</Fingers>\n"
		"\t</Person>\n"
		"\t<Dog Owner=\"Someowner\" ID=\"555\">\n"
		"\t\t<Name>Doggy</Name>\n"
		"\t</Dog>\n</Composite>\n"
	;
	
	char buffer[256];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	const Composite composite = make_composite();
	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, composite);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}

//
//// Copied from XML traits test.
////
//MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, f))
//MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, g))
//MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, h))
//MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(D, int, l))
//MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, m))
//MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, n))
//
//BOOST_AUTO_TEST_CASE(tree_encode_xml_encoder_test) {
//	static const std::string expected =
//		"<A f=\"0\" g=\"0\" h=\"0\">\n"
//		"\t<B>\n"
//		"\t\t<C>\n"
//		"\t\t\t<j>0</j>\n"
//		"\t\t\t<k>0</k>\n"
//		"\t\t</C>\n"
//		"\t\t<i>0</i>\n"
//		"\t\t<D l=\"0\">\n"
//		"\t\t\t<E m=\"0\" n=\"0\" />\n"
//		"\t\t</D>\n"
//		"\t</B>\n"
//		"</A>\n"
//	;
//	
//	char buffer[256];
//	std::fill(std::begin(buffer), std::end(buffer), 0);
//
//	int result = moneta::codec::encode<moneta::codec::xml>(A(), std::begin(buffer), std::end(buffer));
//
//	BOOST_CHECK_EQUAL(result, expected.size());
//	BOOST_CHECK_EQUAL(buffer, expected);
//}

//
// Test - Next Generation stuff.
//

struct Country {
	std::string              Name;
	std::vector<Person>      Persons;
	std::vector<std::string> Tags;
};

MONETA_DESCRIBE_SQL_ENTITY(
	Country, COUNTRY_WITH_STRING_Persons,
	((std::string,              Name,   COUNTRY_NAME  ))
	((std::vector<Person>,      Persons, COUNTRY_Persons))
	((std::vector<std::string>, Tags,   COUNTRY_TAGS  ))
)

MONETA_XML_CONTAINER_MEMBER_ELEMENT_NAME(MONETA_MEMBER(Country, std::vector<std::string>, Tags), CrazyTag)

BOOST_AUTO_TEST_CASE(country_with_string_Persons_stateless_xml_encoder_test) {
	Person jordan = moneta::make_entity<Person>();
	jordan.Name = "Michael Jordan";
	jordan.Height = 2.00;
	
	Person johnson = moneta::make_entity<Person>();
	johnson.Name = "Magic Johnson";
	johnson.Height = 2.10;

	Country cwss;
	cwss.Name = "Germany";
	cwss.Persons.push_back(jordan);
	cwss.Persons.push_back(johnson);
	
	cwss.Tags.push_back("Sometag");
	cwss.Tags.push_back("Anothertag");

	static const std::string expected =
		""
	;
	
	char buffer[2048];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, cwss);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}
