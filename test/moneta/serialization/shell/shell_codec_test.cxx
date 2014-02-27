#include "stdafx.h"
#include <moneta/serialization/shell/shell_codec.hxx>
#include <boost/test/floating_point_comparison.hpp>

#include "../../model/Person.hxx"
#include "../../model/Dog.hxx"

BOOST_AUTO_TEST_CASE(shell_codec_test) {
	Person person;
	person.ID = 123;
	person.Name = "Somedude";
	person.Height = 1.50;
	person.Fingers = 10;

//	std::cerr << moneta::serialization::shell::to_line(person) << std::endl;
}
