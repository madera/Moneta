// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2016                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#include "pch.hxx"
#include <moneta/codec/buffered_decoder.hxx>
#include <moneta/codec/xml/stateless_xml_decoder.hxx>
#include "../model/Person.hxx"

typedef moneta::codec::stateless_xml_decoder<
	moneta::traits::entity_group<Person>
>::type decoder;

static moneta::codec::buffered_decoder<decoder> _buffer;

void round(std::string s) {
	if (!s.empty()) {
		_buffer.append_bytes(s.c_str(), s.size());
	}

	//_buffer.debug_dump();
	const int result = _buffer.decode();
	if (result)
		std::cout << "+++ decode=" << result << std::endl;
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_buffered_decoder) {
	const std::string data = "<Person Name=\"John Smith\" Ratings='10' />";

	decoder::variant_type variant;
	int result = decoder()(boost::begin(data), boost::end(data), variant);
	BOOST_CHECK_EQUAL(result, data.size());

	Person* entity = boost::get<Person>(&variant);
	BOOST_REQUIRE(entity);
	BOOST_CHECK_EQUAL(entity->Name, "John Smith");
	BOOST_CHECK_EQUAL(entity->Ratings, 10);

	round("");

	for (size_t i=0; i<data.size(); ++i) {
		std::string s;
		s.push_back(data[i]);
		round(s);
	}
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_buffered_decoder_deterministic) {
	static const int rounds = 10;

	for (int i=0; i<rounds; ++i) {
		//
	}
}
