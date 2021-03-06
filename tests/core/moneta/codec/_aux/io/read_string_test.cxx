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
#include <moneta/codec/_aux/io/read_string.hxx>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

using namespace moneta::codec;

BOOST_AUTO_TEST_CASE(test_codec_io_read_string_basic) {
	const char str[] = "The\0quick\0brown\0fox";
	char buffer[128];

	const char* itr = boost::begin(str);
	int result;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer));
	BOOST_CHECK_EQUAL(result, 3); // "The"
	BOOST_CHECK_EQUAL(buffer, "The");
	itr += result + 1;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer));
	BOOST_CHECK_EQUAL(result, 5); // "quick"
	BOOST_CHECK_EQUAL(buffer, "quick");
	itr += result + 1;
	
	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer));
	BOOST_CHECK_EQUAL(result, 5); // "brown"
	BOOST_CHECK_EQUAL(buffer, "brown");
	itr += result + 1;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer));
	BOOST_CHECK_EQUAL(result, 3); // "fox"
	BOOST_CHECK_EQUAL(buffer, "fox");
}

BOOST_AUTO_TEST_CASE(test_codec_io_read_string_scarce_output) {
	const char str[] = "The\0quick\0brown\0fox";

	{
		const char* itr = boost::begin(str);
		int result;

		char* b0 = 0;
		result = io::read(itr, boost::end(str), b0, b0);
		BOOST_CHECK_EQUAL(result, 0);
	}

	{
		const char* itr = boost::begin(str);
		int result;

		char b1[1];
		result = io::read(itr, boost::end(str), boost::begin(b1), boost::end(b1));
		BOOST_CHECK_EQUAL(result, 1);
		BOOST_CHECK_EQUAL(b1[0], 'T');
	}

	{
		const char* itr = boost::begin(str);
		int result;

		char b2[2];
		result = io::read(itr, boost::end(str), boost::begin(b2), boost::end(b2));
		BOOST_CHECK_EQUAL(result, 2);
		BOOST_CHECK_EQUAL(b2[0], 'T');
		BOOST_CHECK_EQUAL(b2[1], 'h');
	}

	{
		const char* itr = boost::begin(str);
		int result;

		char b3[3];
		result = io::read(itr, boost::end(str), boost::begin(b3), boost::end(b3));
		BOOST_CHECK_EQUAL(result, 3);
		BOOST_CHECK_EQUAL(b3[0], 'T');
		BOOST_CHECK_EQUAL(b3[1], 'h');
		BOOST_CHECK_EQUAL(b3[2], 'e');
	}
}

BOOST_AUTO_TEST_CASE(test_codec_io_read_string_no_null) {
	const char str[] = "The\0quick\0brown\0fox";
	char buffer[7] = "XXXXXX";

	const char* itr = boost::begin(str);
	int result;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer), false);
	BOOST_CHECK_EQUAL(result, 3); // "The"
	BOOST_CHECK_EQUAL(buffer, "TheXXX");
	itr += result + 1;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer), false);
	BOOST_CHECK_EQUAL(result, 5); // "quick"
	BOOST_CHECK_EQUAL(buffer, "quickX");
	itr += result + 1;
	
	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer), false);
	BOOST_CHECK_EQUAL(result, 5); // "brown"
	BOOST_CHECK_EQUAL(buffer, "brownX");
	itr += result + 1;

	result = io::read(itr, boost::end(str), boost::begin(buffer), boost::end(buffer), false);
	BOOST_CHECK_EQUAL(result, 3); // "fox"
	BOOST_CHECK_EQUAL(buffer, "foxwnX");
}
