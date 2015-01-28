#include "stdafx.h"
#include <moneta/codec/_aux/io/copy_n.hxx>
#include <boost/iterator/filter_iterator.hpp>

//
// RandomAccessIterator tests
//

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_randomaccessiterator) {
	const int ints[] = { 1, 2, 3, 4, 5 };
	int out[128];
	std::fill(std::begin(out), std::end(out), 0);

	using namespace moneta::codec;

	const int* p0 = std::begin(ints);
	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, 0, out),  0);
	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, 0, out),  0);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, 1, out), -1);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, 2, out), -2);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, 3, out), -3);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, INT_MAX, out), 0 - INT_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, ULONG_MAX, out), 0 - ULONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, LLONG_MAX, out), 0 - LLONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(p0, p0, ULLONG_MAX, out), 0 - ULLONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	//

	const int* itr = std::begin(ints);
	const int* end = std::end(ints);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 0, out),  0);
	BOOST_CHECK_EQUAL(itr, std::begin(ints));
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 1, out),  1);
	BOOST_CHECK_EQUAL(itr, std::begin(ints) + 1);
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 2, out + 1), 2);
	BOOST_CHECK_EQUAL(itr, std::begin(ints) + 1 + 2);
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[4], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 3, out + 1 + 2), -1);
	BOOST_CHECK_EQUAL(itr, std::begin(ints) + 5);
	BOOST_CHECK_EQUAL(itr, std::end(ints));
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[3], 4);
	BOOST_CHECK_EQUAL(out[4], 5);
	BOOST_CHECK_EQUAL(out[5], 0);
}

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_randomaccessiterator2) {
	const char src[] = "abcdefghijklmnopqrstuvwxyz";
	
	char buffer[256];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	using namespace moneta::codec;

	const char* itr = std::begin(src);
	const int r0 = io::copy_n(itr, std::end(src), 12, std::begin(buffer));
	BOOST_CHECK_EQUAL(r0, 12);
	BOOST_CHECK_EQUAL(itr, std::begin(src) + 12);
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src), std::begin(src) + 12, buffer, buffer + 12);

	const int r1 = io::copy_n(itr, std::end(src), 6, std::begin(buffer));
	BOOST_CHECK_EQUAL(r1, 6);
	BOOST_CHECK_EQUAL(itr, std::begin(src) + 12 + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src) + 12, std::begin(src) + 12 + 6, buffer, buffer + 6);

	const int r2 = io::copy_n(itr, std::end(src), 8, std::begin(buffer));
	BOOST_CHECK_EQUAL(r2, 8);
	BOOST_CHECK_EQUAL(itr, std::begin(src) + 12 + 6 + 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src) + 18, std::begin(src) + 18 + 8, buffer, buffer + 8);
}

//
// InputIterator tests
//

struct yes {
	bool operator()(int i) const {
		return true;
	}
};

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_inputiterator) {
	const int ints[] = { 1, 2, 3, 4, 5 };
	int out[128];
	std::fill(std::begin(out), std::end(out), 0);

	typedef boost::filter_iterator<yes, const int*> input_iterator;
	input_iterator begin(std::begin(ints), std::end(ints));
	input_iterator end(std::end(ints), std::end(ints));

	using namespace moneta::codec;

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, 0, out),  0);
	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, 0, out),  0);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, 1, out), -1);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, 2, out), -2);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, 3, out), -3);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, INT_MAX, out), 0 - INT_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, ULONG_MAX, out), 0 - ULONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, LLONG_MAX, out), 0 - LLONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(begin, begin, ULLONG_MAX, out), 0 - ULLONG_MAX);
	BOOST_CHECK_EQUAL(out[0], 0);

	//

	input_iterator itr(std::begin(ints), std::end(ints));

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 0, out), 0);
	BOOST_CHECK_EQUAL(*itr, *std::begin(ints));
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 1, out), 1);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(ints) + 1));
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 2, out + 1), 2);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(ints) + 1 + 2));
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[4], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 3, out + 1 + 2), -1);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(ints) + 5));
	BOOST_CHECK(itr == end);
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[3], 4);
	BOOST_CHECK_EQUAL(out[4], 5);
	BOOST_CHECK_EQUAL(out[5], 0);
}

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_inputiterator2) {
	const char src[] = "abcdefghijklmnopqrstuvwxyz";
	
	char buffer[256];
	std::fill(std::begin(buffer), std::end(buffer), 0);

	typedef boost::filter_iterator<yes, const char*> input_iterator;
	input_iterator begin(std::begin(src), std::end(src));
	input_iterator end(std::end(src), std::end(src));

	using namespace moneta::codec;

	input_iterator itr = begin;
	const int r0 = io::copy_n(itr, end, 12, std::begin(buffer));
	BOOST_CHECK_EQUAL(r0, 12);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(src) + 12));
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src), std::begin(src) + 12, buffer, buffer + 12);

	const int r1 = io::copy_n(itr, end, 6, std::begin(buffer));
	BOOST_CHECK_EQUAL(r1, 6);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(src) + 12 + 6));
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src) + 12, std::begin(src) + 12 + 6, buffer, buffer + 6);

	const int r2 = io::copy_n(itr, end, 8, std::begin(buffer));
	BOOST_CHECK_EQUAL(r2, 8);
	BOOST_CHECK_EQUAL(*itr, *(std::begin(src) + 12 + 6 + 8));
	BOOST_CHECK_EQUAL_COLLECTIONS(std::begin(src) + 18, std::begin(src) + 18 + 8, buffer, buffer + 8);
}
