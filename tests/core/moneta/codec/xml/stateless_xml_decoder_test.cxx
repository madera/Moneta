#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include <moneta/pp/describe_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Point,
	((int, x))
	((int, y))
)

// TEMP TEMP TEMP
#include <moneta/codec/xml/stateless_xml_encoder.hxx>
// TEMP TEMP TEMP

BOOST_AUTO_TEST_CASE(unknown_tag_skip_stateless_xml_decoder_test) {

	char buffer[1024];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	Point point;
	point.x = 1;
	point.y = 3;

	char* itr = std::begin(buffer);
	moneta::codec::stateless_xml_encoder()(itr, std::end(buffer), point);

//	std::cout << "===\n" << buffer << std::endl;

	const std::string data = "<Point><x>1</x><y>2</y></Point>";

}
