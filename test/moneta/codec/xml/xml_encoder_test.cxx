#include "stdafx.h"
#include <moneta/codec/xml/xml_encoder.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"
#include "../../model/tree/A.hxx"
#include <moneta/serialization/detail/hexdump.hxx>

MONETA_XML_ATTIBUTE(Composite, MONETA_MEMBER(Composite, int, Identifier))
MONETA_XML_ATTIBUTE(Person, MONETA_MEMBER(Person, int, ID))

MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, std::string, Owner))
MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, int        , ID   ))

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

BOOST_AUTO_TEST_CASE(composite_encode_xml_encoder_test) {
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
	int result = moneta::codec::encode<moneta::codec::xml>(composite, buffer, buffer + sizeof(buffer));

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}


// Copied from XML traits test.
//
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, f))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, g))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, h))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(D, int, l))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, m))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, n))

BOOST_AUTO_TEST_CASE(tree_encode_xml_encoder_test) {
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

	int result = moneta::codec::encode<moneta::codec::xml>(A(), std::begin(buffer), std::end(buffer));

	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(buffer, expected);
}
