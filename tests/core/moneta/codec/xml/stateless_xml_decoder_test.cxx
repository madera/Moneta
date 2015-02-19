#include "stdafx.h"
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include <moneta/pp/describe_entity.hxx>

#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/Cat.hxx"
#include "../../model/tree/A.hxx"

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Point,
	((int, x))
	((int, y))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Customer,
	((int,         ID         ))
	((std::string, Name       ))
	((Address,     HomeAddress))
	((Address,     WorkAddress))
)

typedef moneta::codec::stateless_xml_decoder<
	moneta::traits::entity_group<Person, ThreeInts, A, Cat, Point, Customer>
>::type decoder;

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

BOOST_AUTO_TEST_CASE(test_moneta_codec_stateless_xml_decoder_implementation_ignore_tag) {
	using moneta::codec::stateless_xml_decoder_implementation::ignore_tag;
	
	{
		const std::string data = "<a></a>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b></b></a>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c></c></b></a>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c /></b></a>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c/></b></a>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		// Invalid closing order.
		const std::string data = "<a><b></a></b>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, 0);
	}
	{
		const std::string data = "<a/>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a />";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}

	//
	// Proper size remaining arithmetic
	//
	// TODO: Be more thorough...
	{
		const std::string data = "<something><blabla>";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("</blabla></something>").size());
	}
	{
		const std::string data = "<something><blabla></bla";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("bla></something>").size());
	}
	{
		const std::string data = "<something><blabla></";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("blabla></something>").size());
	}
	{
		const std::string data = "<something><blabla></blabla";
		const int result = ignore_tag(std::begin(data), std::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("></something>").size());
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_element_member) {
	using moneta::codec::stateless_xml_decoder_implementation::read_element_member;

	{
		Person person;

		const std::string data = "<Name>John Smith</Name>";
		const int result = read_element_member<
			MONETA_MEMBER(Person, std::string, Name)
		>(std::begin(data), std::end(data), person);

		BOOST_CHECK_EQUAL(result, data.size());
		BOOST_CHECK_EQUAL(person.Name, "John Smith");
	}

	// TODO: Do more of these.
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

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_decode_with_members) {
	{
		static const std::string data =
			"<Person>\n"
			"\t<Name>John Smith</Name>\n"
			"\t<Fingers>8</Fingers>\n"
			"</Person>"
		;

		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);

		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Fingers, 8);
	}
	{
		static const std::string data =
			"<A f=\"1\" g=\"2\" h=\"3\">\n"
			"\t<b>\n"
			"\t\t<c>\n"
			"\t\t\t<j>10</j>\n"
			"\t\t\t<k>20</k>\n"
			"\t\t</c>\n"
			"\t\t<i>100</i>\n"
			"\t\t<d l=\"500\">\n"
			"\t\t\t<e m=\"1000\" n=\"2000\" />\n"
			"\t\t</d>\n"
			"\t</b>\n"
			"</A>"
		;

		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		A* entity = boost::get<A>(&variant);
		BOOST_REQUIRE(entity);

		A& a = *entity;
		BOOST_CHECK_EQUAL(a.f, 1);
		BOOST_CHECK_EQUAL(a.g, 2);
		BOOST_CHECK_EQUAL(a.h, 3);
		BOOST_CHECK_EQUAL(a.b.c.j, 10);
		BOOST_CHECK_EQUAL(a.b.c.k, 20);
		BOOST_CHECK_EQUAL(a.b.i, 100);
		BOOST_CHECK_EQUAL(a.b.d.l, 500);
		BOOST_CHECK_EQUAL(a.b.d.e.m, 1000);
		BOOST_CHECK_EQUAL(a.b.d.e.n, 2000);
	}
	{
		static const std::string data =
			"<Cat>\n"
			"\t<ID>123</ID>\n"
			"\t<Name>Mr. Garfield</Name>\n"
			"\t<Address ID='12345' Number='1' Street='5th Avenue' />\n"
			"</Cat>"
		;

		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		Cat* entity = boost::get<Cat>(&variant);
		BOOST_REQUIRE(entity);

		Cat& cat = *entity;
		BOOST_CHECK_EQUAL(cat.ID, 123);
		BOOST_CHECK_EQUAL(cat.Name, "Mr. Garfield");
		BOOST_CHECK_EQUAL(cat.Address.ID, 12345);
		BOOST_CHECK_EQUAL(cat.Address.Number, 1);
		BOOST_CHECK_EQUAL(cat.Address.Street, "5th Avenue");
	}
	{
		static const std::string data =
			"<Customer ID=\"123\" Name=\"John Smith\">\n"
			"\t<HomeAddress ID=\"555\" Number=\"100\" Street=\"One Infinite Loop\" />\n"
			"\t<WorkAddress ID=\"777\" Number=\"200\" Street=\"Two Infinite Loops\" />\n"
			"</Customer>"
		;

		decoder::variant_type variant;
		int result = decoder()(std::begin(data), std::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		Customer* entity = boost::get<Customer>(&variant);
		BOOST_REQUIRE(entity);

		Customer& customer = *entity;
		BOOST_CHECK_EQUAL(customer.ID, 123);
		BOOST_CHECK_EQUAL(customer.Name, "John Smith");
		BOOST_CHECK_EQUAL(customer.HomeAddress.ID, 555);
		BOOST_CHECK_EQUAL(customer.HomeAddress.Number, 100);
		BOOST_CHECK_EQUAL(customer.HomeAddress.Street, "One Infinite Loop");
		BOOST_CHECK_EQUAL(customer.WorkAddress.ID, 777);
		BOOST_CHECK_EQUAL(customer.WorkAddress.Number, 200);
		BOOST_CHECK_EQUAL(customer.WorkAddress.Street, "Two Infinite Loops");
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
