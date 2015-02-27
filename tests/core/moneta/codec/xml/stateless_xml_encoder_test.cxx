#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_encoder.hxx>
#include <moneta/make_entity.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/tree/A.hxx"
#include "../../model/Customer.hxx"
#include <moneta/serialization/detail/hexdump.hxx>

using moneta::codec::stateless_xml_encoder;

MONETA_XML_ATTIBUTE(Composite, MONETA_MEMBER(Composite, int, Identifier))
MONETA_XML_ATTIBUTE(Person,    MONETA_MEMBER(Person,    int, ID        ))

MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, std::string, Owner))
MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, int        , ID   ))

//
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

BOOST_AUTO_TEST_CASE(stateless_xml_encoder_basic_test) {
	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const int result = stateless_xml_encoder()(std::begin(buffer), buffer + sizeof(buffer) - 1, A());

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
	const int result = stateless_xml_encoder()(std::begin(buffer), buffer + sizeof(buffer) - 1, composite);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}

BOOST_AUTO_TEST_CASE(tree_encode_stateless_xml_encoder_test) {
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
	
	char buffer[256];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	const int result = stateless_xml_encoder()(std::begin(buffer), buffer + sizeof(buffer) - 1, A());

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}

//
// --------------------------------------------------------------------------------------------------------------------
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

MONETA_XML_CONTAINER_ITEM_NAME(MONETA_MEMBER(Country, std::vector<std::string>, Tags), CrazyTag)

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
		"<Country>\n"
		"\t<Name>Germany</Name>\n"
		"\t<Persons>\n"
		"\t\t<Person ID=\"0\">\n"
		"\t\t\t<Name>Michael Jordan</Name>\n"
		"\t\t\t<Height>2</Height>\n"
		"\t\t\t<Fingers>0</Fingers>\n"
		"\t\t</Person>\n"
		"\t\t<Person ID=\"0\">\n"
		"\t\t\t<Name>Magic Johnson</Name>\n"
		"\t\t\t<Height>2.1000000000000001</Height>\n"
		"\t\t\t<Fingers>0</Fingers>\n"
		"\t\t</Person>\n"
		"\t</Persons>\n"
		"\t<Tags>\n"
		"\t\t<CrazyTag>Sometag</CrazyTag>\n"
		"\t\t<CrazyTag>Anothertag</CrazyTag>\n"
		"\t</Tags>\n"
		"</Country>\n"
	;
	
	char buffer[2048];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	using namespace moneta::codec;

	const int result = stateless_xml_encoder()(std::begin(buffer), std::end(buffer), cwss);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_stateless_xml_encoder_optionals) {
	Address address;
	address.ID = 0;
	address.Number = 1;
	address.Street = "Something St.";

	Dog lassie;
	lassie.ID = 1;
	lassie.Name = "Lassie";
	lassie.Owner = "Somebody";

	Dog yoddy;
	yoddy.ID = 1;
	yoddy.Name = "Yoddy";
	yoddy.Owner = "Sam";

	Customer customer;
	customer.DOB = 0;
	customer.HomeAddress = address;
	customer.Rating = 10;
	customer.Dogs = std::vector<Dog>();
	customer.Dogs->push_back(lassie);
	customer.Dogs->push_back(yoddy);

	customer.Tags = std::vector<std::string>();
	customer.Tags->push_back("tag0");
	customer.Tags->push_back("tag1");
	customer.Tags->push_back("tag2");

	static const std::string expected =
		"<Customer>\n"
		"\t<Name></Name>\n"
		"\t<DOB>0</DOB>\n"
		"\t<Address>\n"
		"\t\t<ID>0</ID>\n"
		"\t\t<Number>1</Number>\n"
		"\t\t<Street>Something St.</Street>\n"
		"\t</Address>\n"
		"\t<Rating>10</Rating>\n"
		"\t<Dogs>\n"
		"\t\t<Dog Owner=\"Somebody\" ID=\"1\">\n"
		"\t\t\t<Name>Lassie</Name>\n"
		"\t\t</Dog>\n"
		"\t\t<Dog Owner=\"Sam\" ID=\"1\">\n"
		"\t\t\t<Name>Yoddy</Name>\n"
		"\t\t</Dog>\n"
		"\t</Dogs>\n"
		"\t<Tags>\n"
		"\t\t<TagsItem>tag0</TagsItem>\n"
		"\t\t<TagsItem>tag1</TagsItem>\n"
		"\t\t<TagsItem>tag2</TagsItem>\n"
		"\t</Tags>\n"
		"</Customer>\n"
	;

	char buffer[2048];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	using namespace moneta::codec;
	const int result = stateless_xml_encoder()(std::begin(buffer), std::end(buffer), customer);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_stateless_xml_encoder_optionals_2) {

	Customer customer;
	customer.DOB = 0;

	static const std::string expected =
		"<Customer>\n"
		"\t<Name></Name>\n"
		"\t<DOB>0</DOB>\n"
		"</Customer>\n"
	;

	char buffer[2048];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	using namespace moneta::codec;
	const int result = stateless_xml_encoder()(std::begin(buffer), std::end(buffer), customer);

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}
