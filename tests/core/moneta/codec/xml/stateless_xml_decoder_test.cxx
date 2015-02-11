#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include <moneta/pp/describe_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Point,
	((int, x))
	((int, y))
)

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_prefix) {
	using moneta::codec::stateless_xml_decoder_implementation::read_prefix;

	// We need at least three bytes for a best case scenario with a single letter named entity and separator.
	{
		std::string prefix;
		const std::string s;
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), -3); // "<a_"
		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), -2);
		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<e";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), -1);
		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<ee";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), -1);
		BOOST_CHECK_EQUAL(prefix, "ee"); // Partial result, which shouldn't be used on error.
	}
	{
		std::string prefix;
		const std::string s = "<e ";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 2);
		BOOST_CHECK_EQUAL(prefix, "e");
	}
	{
		std::string prefix;
		const std::string s = "<e/";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 2);
		BOOST_CHECK_EQUAL(prefix, "e");
	}
	{
		std::string prefix;
		const std::string s = "<an_entity";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), -1);
		BOOST_CHECK_EQUAL(prefix, "an_entity");
	}
	{
		std::string prefix;
		const std::string s = "<an_entity ";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 10);
		BOOST_CHECK_EQUAL(prefix, "an_entity");
	}
	{
		std::string prefix;
		const std::string s = "<example/>";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 8);
		BOOST_CHECK_EQUAL(prefix, "example");
	}
	{
		std::string prefix;
		const std::string s = "<example />";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 8);
		BOOST_CHECK_EQUAL(prefix, "example");
	}
}

#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"

typedef moneta::codec::stateless_xml_decoder<
	moneta::traits::entity_group<Person, ThreeInts>
>::type decoder;

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_0) {
	{
		const std::string data = "<ThreeInts/>";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		ThreeInts* entity = boost::get<ThreeInts>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<ThreeInts />";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		ThreeInts* entity = boost::get<ThreeInts>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person/>";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person />";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person Name=\"John Smith\" Fingers='10' />";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Fingers, 10);
	}
	{
		const std::string data = "<Person Name=\"John Smith\" Fingers='10'/>";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Fingers, 10);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_incomplete_closing_tag) {
	{
		const std::string data = "<Person>";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("</Person>").size());
	}
	{
		const std::string data = "<Person><";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("/Person>").size());
	}
	{
		const std::string data = "<Person></";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("Person>").size());
	}
	{
		const std::string data = "<Person></P";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("erson>").size());
	}
	{
		const std::string data = "<Person></Pe";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("rson>").size());
	}
	{
		const std::string data = "<Person></Per";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("son>").size());
	}
	{
		const std::string data = "<Person></Pers";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("on>").size());
	}
	{
		const std::string data = "<Person></Perso";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("n>").size());
	}
	{
		const std::string data = "<Person></Person";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string(">").size());
	}
	{
		const std::string data = "<Person></Person>";
		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_CHECK(entity);
	}
}

BOOST_AUTO_TEST_CASE(foobar) {

	const std::string data =
		"<Point>\n"
		"\t<x>1</x>\n"
		"\t<y>2</y>\n"
		"</Point>"
	;
	



	//char buffer[1024];
	//std::fill(std::begin(buffer), std::end(buffer), 0);

	//Point point;
	//point.x = 1;
	//point.y = 3;

	//moneta::codec::stateless_xml_encoder()(std::begin(buffer), std::end(buffer), point);

//	std::cout << "===\n" << buffer << std::endl;


}
