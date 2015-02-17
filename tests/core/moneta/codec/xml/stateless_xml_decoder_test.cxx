#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include <moneta/pp/describe_entity.hxx>

#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"

typedef moneta::codec::stateless_xml_decoder<
	moneta::traits::entity_group<Person, ThreeInts>
>::type decoder;

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Point,
	((int, x))
	((int, y))
)

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_prefix) {
	using moneta::codec::stateless_xml_decoder_implementation::read_prefix;

	{
		// We need at least three bytes for, say, a single letter named entity and separator.
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
		const std::string s = "<ee ";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 3);
		BOOST_CHECK_EQUAL(prefix, "ee");
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
	{
		std::string prefix;
		const std::string s = "</a>";
		BOOST_CHECK_EQUAL(read_prefix(std::begin(s), std::end(s), prefix), 0);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_tag) {
	using moneta::codec::stateless_xml_decoder_implementation::tag_reader;

	{
		const std::string s;
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -3);
	}
	{
		const std::string s = "<";
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -2);
	}
	{
		const std::string s = "<a";
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -1);
	}
	{
		const std::string s = "<aa";
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -1);
	}
	{
		const std::string s = "<a ";
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -1);
	}
	{
		const std::string s = "<a /";
		std::string tag;
		BOOST_CHECK_EQUAL(tag_reader<>()(std::begin(s), std::end(s), tag), -1);
	}
	{
		const std::string s = "<a>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(reader.last.is_opening);
		BOOST_CHECK(!reader.last.has_attributes);
		BOOST_CHECK(!reader.last.is_singleton);
	}
	{
		const std::string s = "<a/>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(!reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		// With error on attribute
		const std::string s = "<a k='v />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), 0);
	}
	{
		// With error on attribute
		const std::string s = "<a k= />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), 0);
	}
	{
		// With error on attribute
		const std::string s = "<a k />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), 0);
	}
	{
		// With error on attribute
		const std::string s = "<a k='a />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), 0);
	}
	{
		const std::string s = "<a k=\"v\" />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k='v' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k='' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k0='v0' k1='v1' k2='v2' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k0='v0' k1='v1' k2='v2'>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(std::begin(s), std::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(!reader.last.is_singleton);
	}
}

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
//		BOOST_CHECK_EQUAL(result, 0 - std::string("erson>").size());
		BOOST_CHECK_EQUAL(result, -6);
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

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_decode_with_members) {
	{
		const std::string data =
			"<Person>\n"
			"\t<Name>John Smith</Name>\n"
			"</Person>\n"
		;

		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("</Person>").size());
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
