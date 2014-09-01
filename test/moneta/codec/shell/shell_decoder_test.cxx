#include "stdafx.h"
#include <moneta/codec/shell/shell_decoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

BOOST_AUTO_TEST_CASE(shell_decoder_test) {
	const std::string line = "Person={ID=5 Name=John Height=1.8 Fingers=12}";
	
	Person person;
	//const int result = moneta::codec::decode<moneta::codec::shell>(person, line.begin(), line.end());
}
