#include "pch.hxx"
#include <moneta/codec/_aux/io/write_string.hxx>
#include <boost/iterator/filter_iterator.hpp>

using namespace moneta::codec;

BOOST_AUTO_TEST_CASE(test_codec_io_write_string_basic) {
	char buffer[128];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	char* itr = std::begin(buffer);
	BOOST_CHECK_EQUAL(io::write(itr, std::end(buffer), "abcdef"), 6 + 1);
}

BOOST_AUTO_TEST_CASE(test_codec_io_write_string_no_null) {
	char buffer[7] = "XXXXXX";

	char* itr = std::begin(buffer);
	BOOST_CHECK_EQUAL(io::write(itr, std::end(buffer), "abc", false), 3);
	BOOST_CHECK_EQUAL(buffer, "abcXXX");
}

BOOST_AUTO_TEST_CASE(test_codec_io_write_string_scarce_output) {
	char buffer[7] = "XXXXXX";

	char* itr = std::begin(buffer);

	//
	// Just empty string
	//
	BOOST_CHECK_EQUAL(io::write(itr, itr, "", false), 0);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	BOOST_CHECK_EQUAL(io::write(itr, itr, ""), -1);
	BOOST_CHECK_EQUAL(itr, std::begin(buffer));

	//
	// Progressive residual writes tests
	//
	BOOST_CHECK_EQUAL(io::write(itr, itr, "abcdef", false), -6);
	BOOST_CHECK_EQUAL(io::write(itr, itr, "abcdef"), -7);

	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "abcdef"), -6);
	BOOST_CHECK_EQUAL(buffer, "aXXXXX");

	++itr;
	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "bcdef"), -5);
	BOOST_CHECK_EQUAL(buffer, "abXXXX");

	++itr;
	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "cdef"), -4);
	BOOST_CHECK_EQUAL(buffer, "abcXXX");

	++itr;
	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "def"), -3);
	BOOST_CHECK_EQUAL(buffer, "abcdXX");

	++itr;
	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "ef"), -2);
	BOOST_CHECK_EQUAL(buffer, "abcdeX");

	++itr;
	BOOST_CHECK_EQUAL(io::write(itr, itr + 1, "f"), -1);
	BOOST_CHECK_EQUAL(buffer, "abcdef");
}

// Old test code. Still valid.
BOOST_AUTO_TEST_CASE(test_codec_io_write_string_scarce_output_2) {

	char v0[16];
	std::fill(std::begin(v0), std::end(v0), 0x55);

	const char* text = "abcdef";

	char* itr = std::begin(v0);
	BOOST_CHECK_EQUAL(io::write(itr, std::end(v0), text), 7);
	BOOST_CHECK_EQUAL_COLLECTIONS(v0, v0+7, text, text+7);

	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 0, text), -7);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 1, text), -6);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 2, text), -5);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 3, text), -4);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 4, text), -3);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 5, text), -2);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 6, text), -1);
	BOOST_CHECK_EQUAL(io::write(itr, std::begin(v0) + 7, text),  7);
}
