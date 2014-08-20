#include "stdafx.h"
#include <moneta/codec/shell/shell_encoder.hxx>
#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"
#include "../../model/Composite.hxx"

#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/serialization/iostream.hxx>

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

BOOST_AUTO_TEST_CASE(shell_encoder_test_1) {
	const Composite composite = make_composite();
	
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));

	std::cerr << std::endl;

	moneta::codec::encode<moneta::codec::shell>(composite, buffer, buffer + sizeof(buffer));

	//"{Identifier=2600 Person={ID=123 Name=Somedude Height=1.5 Fingers=10} Dog={Owner=Someowner ID=555 Name=Doggy}}"
}
