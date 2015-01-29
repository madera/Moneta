#include "stdafx.h"
#include <moneta/codec/_aux/io/write_cstr.hxx>
#include <boost/iterator/filter_iterator.hpp>

// TODO: Test some more?

using namespace moneta::codec;

BOOST_AUTO_TEST_CASE(test_codec_io_write_cstr_basic) {
	char buffer[128];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	char* itr = std::begin(buffer);
	BOOST_CHECK_EQUAL(io::write_cstr(itr, std::end(buffer), "abcdef"), 6 + 1);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 6);
}

BOOST_AUTO_TEST_CASE(test_codec_io_write_cstr_no_null) {
	char buffer[7] = "XXXXXX";

	char* itr = std::begin(buffer);
	BOOST_CHECK_EQUAL(io::write_cstr(itr, std::end(buffer), "abc", false), 3);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 3);
	BOOST_CHECK_EQUAL(buffer, "abcXXX");
}

BOOST_AUTO_TEST_CASE(test_codec_io_write_cstr_scarce_output) {
	char buffer[7] = "XXXXXX";

	char* itr = std::begin(buffer);

	//
	// Just empty string
	//
	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr, "", false), 0);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr, ""), -1);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	//
	// Progressive writes
	//
	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr, "abcdef", false), -6);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr, "abcdef"), -7);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "abcdef"), -6);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 1);
	BOOST_CHECK_EQUAL(buffer, "aXXXXX");

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "bcdef"), -5);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 2);
	BOOST_CHECK_EQUAL(buffer, "abXXXX");

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "cdef"), -4);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 3);
	BOOST_CHECK_EQUAL(buffer, "abcXXX");

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "def"), -3);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 4);
	BOOST_CHECK_EQUAL(buffer, "abcdXX");

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "ef"), -2);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 5);
	BOOST_CHECK_EQUAL(buffer, "abcdeX");

	BOOST_CHECK_EQUAL(io::write_cstr(itr, itr + 1, "f"), -1); //////////// XXXX
	BOOST_CHECK_EQUAL(itr, std::begin(buffer) + 6);
	BOOST_CHECK_EQUAL(buffer, "abcdef");
}
