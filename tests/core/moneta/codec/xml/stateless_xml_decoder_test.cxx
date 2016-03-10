// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include "../../model/Person.hxx"
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/Cat.hxx"
#include "../../model/tree/A.hxx"
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include <moneta/pp/describe_entity.hxx>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

MONETA_DEFINE_ENTITY(
	Point,
	((int, x))
	((int, y))
)

MONETA_DEFINE_ENTITY(
	Worker,
	((int,         ID         ))
	((std::string, Name       ))
	((Address,     HomeAddress))
	((Address,     WorkAddress))
)

typedef moneta::codec::stateless_xml_decoder<
	moneta::traits::entity_group<Person, ThreeInts, A, Cat, Point, Worker>
>::type decoder;

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_prefix) {
	using moneta::codec::stateless_xml_decoder_implementation::read_prefix;

	{
		// We need at least three bytes for, say,
		// a single letter named entity and separator.
		std::string prefix;
		const std::string s;
		
		// "<a_"
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			-3
		);

		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			-2
		);
		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<e";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			-1
		);
		BOOST_CHECK(prefix.empty());
	}
	{
		std::string prefix;
		const std::string s = "<ee";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			-1
		);

		// Partial result, which shouldn't be used on error.
		BOOST_CHECK_EQUAL(prefix, "ee");
	}
	{
		std::string prefix;
		const std::string s = "<e ";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			2
		);
		BOOST_CHECK_EQUAL(prefix, "e");
	}
	{
		std::string prefix;
		const std::string s = "<ee ";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			3
		);
		BOOST_CHECK_EQUAL(prefix, "ee");
	}
	{
		std::string prefix;
		const std::string s = "<e/";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			2
		);
		BOOST_CHECK_EQUAL(prefix, "e");
	}
	{
		std::string prefix;
		const std::string s = "<an_entity";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			-1
		);
		BOOST_CHECK_EQUAL(prefix, "an_entity");
	}
	{
		std::string prefix;
		const std::string s = "<an_entity ";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			10
		);
		BOOST_CHECK_EQUAL(prefix, "an_entity");
	}
	{
		std::string prefix;
		const std::string s = "<example/>";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			8
		);
		BOOST_CHECK_EQUAL(prefix, "example");
	}
	{
		std::string prefix;
		const std::string s = "<example />";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			8
		);
		BOOST_CHECK_EQUAL(prefix, "example");
	}
	{
		std::string prefix;
		const std::string s = "</a>";
		BOOST_CHECK_EQUAL(
			read_prefix(boost::begin(s), boost::end(s), prefix),
			0
		);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_read_tag) {
	using moneta::codec::stateless_xml_decoder_implementation::tag_reader;

	{
		const std::string s;
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-3
		);
	}
	{
		const std::string s = "<";
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-2
		);
	}
	{
		const std::string s = "<a";
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-1
		);
	}
	{
		const std::string s = "<aa";
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-1
		);
	}
	{
		const std::string s = "<a ";
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-1
		);
	}
	{
		const std::string s = "<a /";
		std::string tag;
		BOOST_CHECK_EQUAL(
			tag_reader<>()(boost::begin(s), boost::end(s), tag),
			-1
		);
	}
	{
		const std::string s = "<a>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(
			reader(boost::begin(s), boost::end(s), tag),
			s.size()
		);
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(reader.last.is_opening);
		BOOST_CHECK(!reader.last.has_attributes);
		BOOST_CHECK(!reader.last.is_singleton);
	}
	{
		const std::string s = "<a/>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(
			reader(boost::begin(s), boost::end(s), tag),
			s.size()
		);
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
		BOOST_CHECK_EQUAL(
			reader(boost::begin(s), boost::end(s), tag),
			0
		);
	}
	{
		// With error on attribute
		const std::string s = "<a k= />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(
			reader(boost::begin(s), boost::end(s), tag),
			0
		);
	}
	{
		// With error on attribute
		const std::string s = "<a k />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), 0);
	}
	{
		// With error on attribute
		const std::string s = "<a k='a />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), 0);
	}
	{
		const std::string s = "<a k=\"v\" />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k='v' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k='' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k0='v0' k1='v1' k2='v2' />";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), s.size());
		BOOST_CHECK_EQUAL(tag, "a");
		BOOST_CHECK(!reader.last.is_opening);
		BOOST_CHECK(reader.last.has_attributes);
		BOOST_CHECK(reader.last.is_singleton);
	}
	{
		const std::string s = "<a k0='v0' k1='v1' k2='v2'>";
		std::string tag;

		tag_reader<> reader;
		BOOST_CHECK_EQUAL(reader(boost::begin(s), boost::end(s), tag), s.size());
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
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b></b></a>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c></c></b></a>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c /></b></a>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a><b><c/></b></a>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		// Invalid closing order.
		const std::string data = "<a><b></a></b>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, 0);
	}
	{
		const std::string data = "<a/>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}
	{
		const std::string data = "<a />";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, data.size());
	}

	//
	// Proper size remaining arithmetic
	//
	// TODO: Be more thorough...
	{
		const std::string data = "<something><blabla>";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("</blabla></something>").size());
	}
	{
		const std::string data = "<something><blabla></bla";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("bla></something>").size());
	}
	{
		const std::string data = "<something><blabla></";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
		BOOST_CHECK_EQUAL(result, 0 - std::string("blabla></something>").size());
	}
	{
		const std::string data = "<something><blabla></blabla";
		const int result = ignore_tag(boost::begin(data), boost::end(data));
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
		>(boost::begin(data), boost::end(data), person);

		BOOST_CHECK_EQUAL(result, data.size());
		BOOST_CHECK_EQUAL(person.Name, "John Smith");
	}

	// TODO: Do more of these.
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_0) {
	{
		const std::string data = "<ThreeInts/>";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		ThreeInts* entity = boost::get<ThreeInts>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<ThreeInts />";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		ThreeInts* entity = boost::get<ThreeInts>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person/>";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());
		
		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person />";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
	}
	{
		const std::string data = "<Person Name=\"John Smith\" Ratings='10' />";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Ratings, 10);
	}
	{
		const std::string data = "<Person Name=\"John Smith\" Ratings='10'/>";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);
		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Ratings, 10);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_incomplete_closing_tag) {
	{
		const std::string data = "<Person>";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("</Person>").size());
	}
	{
		const std::string data = "<Person><";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("/Person>").size());
	}
	{
		const std::string data = "<Person></";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("Person>").size());
	}
	{
		const std::string data = "<Person></P";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("erson>").size());
	}
	{
		const std::string data = "<Person></Pe";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("rson>").size());
	}
	{
		const std::string data = "<Person></Per";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("son>").size());
	}
	{
		const std::string data = "<Person></Pers";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("on>").size());
	}
	{
		const std::string data = "<Person></Perso";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string("n>").size());
	}
	{
		const std::string data = "<Person></Person";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, 0 - std::string(">").size());
	}
	{
		const std::string data = "<Person></Person>";
		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_CHECK_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_CHECK(entity);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_full_decode) {
	{
		static const std::string data =
			"<Person>\n"
			"\t<Name>John Smith</Name>\n"
			"\t<Ratings>8</Ratings>\n"
			"</Person>"
		;

		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		Person* entity = boost::get<Person>(&variant);
		BOOST_REQUIRE(entity);

		BOOST_CHECK_EQUAL(entity->Name, "John Smith");
		BOOST_CHECK_EQUAL(entity->Ratings, 8);
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
		int result = decoder()(boost::begin(data), boost::end(data), variant);
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
		int result = decoder()(boost::begin(data), boost::end(data), variant);
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
			"<Worker ID=\"123\" Name=\"John Smith\">\n"
			"\t<HomeAddress ID=\"555\" Number=\"100\" Street=\"One Infinite Loop\" />\n"
			"\t<WorkAddress ID=\"777\" Number=\"200\" Street=\"Two Infinite Loops\" />\n"
			"</Worker>"
		;

		decoder::variant_type variant;
		int result = decoder()(boost::begin(data), boost::end(data), variant);
		BOOST_REQUIRE_EQUAL(result, data.size());

		Worker* entity = boost::get<Worker>(&variant);
		BOOST_REQUIRE(entity);

		const Worker& worker = *entity;
		BOOST_CHECK_EQUAL(worker.ID, 123);
		BOOST_CHECK_EQUAL(worker.Name, "John Smith");
		BOOST_CHECK_EQUAL(worker.HomeAddress.ID, 555);
		BOOST_CHECK_EQUAL(worker.HomeAddress.Number, 100);
		BOOST_CHECK_EQUAL(worker.HomeAddress.Street, "One Infinite Loop");
		BOOST_CHECK_EQUAL(worker.WorkAddress.ID, 777);
		BOOST_CHECK_EQUAL(worker.WorkAddress.Number, 200);
		BOOST_CHECK_EQUAL(worker.WorkAddress.Street, "Two Infinite Loops");
	}
}

// ---

MONETA_DEFINE_ENTITY(
	Point3D,
	((boost::uint32_t, x))
	((boost::uint32_t, y))
	((boost::uint32_t, z))
)

MONETA_XML_ITEM_NAME(Point3D, point_3d)
MONETA_XML_ITEM_NAME(MONETA_MEMBER(Point3D, boost::uint32_t, x), x_item)
MONETA_XML_ITEM_NAME(MONETA_MEMBER(Point3D, boost::uint32_t, y), y_item)
MONETA_XML_ITEM_NAME(MONETA_MEMBER(Point3D, boost::uint32_t, z), z_item)

BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_xml_item_name) {
	static const std::string data =
		"<point_3d>\n"
		"\t<x_item>123</x_item>\n"
		"\t<y_item>456</y_item>\n"
		"\t<z_item>789</z_item>\n"
		"</point_3d>"
	;

	typedef moneta::codec::stateless_xml_decoder<
		moneta::traits::entity_group<Person, ThreeInts, A, Cat, Point, Worker, Point3D>
	>::type decoder;

	decoder::variant_type variant;
	const int result = decoder()(boost::begin(data), boost::end(data), variant);
	BOOST_REQUIRE_EQUAL(result, data.size());

	Point3D* entity = boost::get<Point3D>(&variant);
	BOOST_REQUIRE(entity);

	Point3D& point = *entity;
	BOOST_CHECK_EQUAL(point.x, 123);
	BOOST_CHECK_EQUAL(point.y, 456);
	BOOST_CHECK_EQUAL(point.z, 789);
}

// ---

//MONETA_DEFINE_ENTITY(
//	AddressList,
//	((Address, HomeAddress))
//)
//
//BOOST_AUTO_TEST_CASE(test_moneta_codec_xml_stateless_xml_decoder_xml_item_name_entities) {
//	static const std::string data =
//		"<point_3d>\n"
//		"\t<x_item>123</x_item>\n"
//		"\t<y_item>456</y_item>\n"
//		"\t<z_item>789</z_item>\n"
//		"</point_3d>"
//	;
//
//	typedef moneta::codec::stateless_xml_decoder<
//		moneta::traits::entity_group<Person, ThreeInts, A, Cat, Point, Worker, Point3D>
//	>::type decoder;
//
//	decoder::variant_type variant;
//	int result = decoder()(boost::begin(data), boost::end(data), variant);
//	BOOST_REQUIRE_EQUAL(result, data.size());
//
//	Point3D* entity = boost::get<Point3D>(&variant);
//	BOOST_REQUIRE(entity);
//
//	Point3D& point = *entity;
//	BOOST_CHECK_EQUAL(point.x, 123);
//	BOOST_CHECK_EQUAL(point.y, 456);
//	BOOST_CHECK_EQUAL(point.z, 789);
//}

// TODO: Round trip encoding and decoding (on a separate file).
