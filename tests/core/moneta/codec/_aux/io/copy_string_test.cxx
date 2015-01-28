#include "stdafx.h"
#include <moneta/codec/_aux/io/copy_string.hxx>
#include <boost/iterator/filter_iterator.hpp>

BOOST_AUTO_TEST_CASE(test_codec_io_copy_string) {
	const char str[] = "The\0quick\0brown\0fox";
	char buffer[128];

	using namespace moneta::codec;

	const char* itr = std::begin(str);
	int result;

	result = io::copy_cstr(itr, std::end(str), buffer);
	BOOST_CHECK_EQUAL(result, 3); // "The"
	BOOST_CHECK_EQUAL(*itr, 0);
	BOOST_CHECK_EQUAL(buffer, "The");
	++itr;

	result = io::copy_cstr(itr, std::end(str), buffer);
	BOOST_CHECK_EQUAL(result, 5); // "quick"
	BOOST_CHECK_EQUAL(*itr, 0);
	BOOST_CHECK_EQUAL(buffer, "quick");
	++itr;
	
	result = io::copy_cstr(itr, std::end(str), buffer);
	BOOST_CHECK_EQUAL(result, 5); // "brown"
	BOOST_CHECK_EQUAL(*itr, 0);
	BOOST_CHECK_EQUAL(buffer, "brown");
	++itr;

	result = io::copy_cstr(itr, std::end(str), buffer);
	BOOST_CHECK_EQUAL(result, 3); // "fox"
	BOOST_CHECK_EQUAL(*itr, 0);
	BOOST_CHECK_EQUAL(buffer, "fox");
	++itr;
}
