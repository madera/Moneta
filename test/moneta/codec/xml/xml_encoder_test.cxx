#include "stdafx.h"
#include <moneta/codec/xml/xml_encoder.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"

#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/serialization/iostream.hxx>

MONETA_XML_ATTIBUTE(Composite, MONETA_MEMBER(Composite, int, Identifier))
MONETA_XML_ATTIBUTE(Person, MONETA_MEMBER(Person, int, ID))

MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, std::string, Owner))
MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, int        , ID   ))
//MONETA_XML_ATTIBUTE(Dog, MONETA_MEMBER(Dog, std::string, Name ))

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

BOOST_AUTO_TEST_CASE(xml_encoder_test_1) {
	const Composite composite = make_composite();
	
	char buffer[1024];
	std::fill(std::begin(buffer), std::end(buffer), 0x55);

	moneta::codec::encode<moneta::codec::xml>(composite, buffer, buffer + sizeof(buffer));

	//BOOST_CHECK_EQUAL(line, "{Identifier=2600 Person={ID=123 Name=Somedude Height=1.5 Fingers=10} Dog={Owner=Someowner ID=555 Name=Doggy}}");
}
