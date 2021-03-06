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
#include <moneta/codec/_aux/io/copy_n.hxx>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

//
// RandomAccessIterator tests
//

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_randomaccessiterator) {
	const int ints[] = { 1, 2, 3, 4, 5 };
	int out[128];
	std::fill(boost::begin(out), boost::end(out), 0);

	using namespace moneta::codec;
	
	const int* p0 = boost::begin(ints);
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

	const int* itr = boost::begin(ints);
	const int* end = boost::end(ints);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 0, out),  0);
	BOOST_CHECK_EQUAL(itr, boost::begin(ints));
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 1, out),  1);
	BOOST_CHECK_EQUAL(itr, boost::begin(ints) + 1);
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 2, out + 1), 2);
	BOOST_CHECK_EQUAL(itr, boost::begin(ints) + 1 + 2);
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[4], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 3, out + 1 + 2), -1);
	BOOST_CHECK_EQUAL(itr, boost::begin(ints) + 5);
	BOOST_CHECK_EQUAL(itr, boost::end(ints));
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
	std::fill(boost::begin(buffer), boost::end(buffer), 0);

	using namespace moneta::codec;

	const char* itr = boost::begin(src);
	const int r0 = io::copy_n(itr, boost::end(src), 12, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r0, 12);
	BOOST_CHECK_EQUAL(itr, boost::begin(src) + 12);
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src), boost::begin(src) + 12, buffer, buffer + 12);

	const int r1 = io::copy_n(itr, boost::end(src), 6, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r1, 6);
	BOOST_CHECK_EQUAL(itr, boost::begin(src) + 12 + 6);
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src) + 12, boost::begin(src) + 12 + 6, buffer, buffer + 6);

	const int r2 = io::copy_n(itr, boost::end(src), 8, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r2, 8);
	BOOST_CHECK_EQUAL(itr, boost::begin(src) + 12 + 6 + 8);
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src) + 18, boost::begin(src) + 18 + 8, buffer, buffer + 8);
}

//
// InputIterator tests
//

struct yes {
	bool operator()(int) const {
		return true;
	}
};

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_inputiterator) {
	const int ints[] = { 1, 2, 3, 4, 5 };
	int out[128];
	std::fill(boost::begin(out), boost::end(out), 0);

	typedef boost::filter_iterator<yes, const int*> input_iterator;
	input_iterator begin(boost::begin(ints), boost::end(ints));
	input_iterator end(boost::end(ints), boost::end(ints));

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

	input_iterator itr(boost::begin(ints), boost::end(ints));

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 0, out), 0);
	BOOST_CHECK_EQUAL(*itr, *boost::begin(ints));
	BOOST_CHECK_EQUAL(out[0], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 1, out), 1);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(ints) + 1));
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 2, out + 1), 2);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(ints) + 1 + 2));
	BOOST_CHECK_EQUAL(out[0], 1);
	BOOST_CHECK_EQUAL(out[1], 2);
	BOOST_CHECK_EQUAL(out[2], 3);
	BOOST_CHECK_EQUAL(out[4], 0);

	BOOST_CHECK_EQUAL(io::copy_n(itr, end, 3, out + 1 + 2), -1);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(ints) + 5));
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
	std::fill(boost::begin(buffer), boost::end(buffer), 0);

	typedef boost::filter_iterator<yes, const char*> input_iterator;
	input_iterator begin(boost::begin(src), boost::end(src));
	input_iterator end(boost::end(src), boost::end(src));

	using namespace moneta::codec;

	input_iterator itr = begin;
	const int r0 = io::copy_n(itr, end, 12, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r0, 12);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(src) + 12));
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src), boost::begin(src) + 12, buffer, buffer + 12);

	const int r1 = io::copy_n(itr, end, 6, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r1, 6);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(src) + 12 + 6));
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src) + 12, boost::begin(src) + 12 + 6, buffer, buffer + 6);

	const int r2 = io::copy_n(itr, end, 8, boost::begin(buffer));
	BOOST_CHECK_EQUAL(r2, 8);
	BOOST_CHECK_EQUAL(*itr, *(boost::begin(src) + 12 + 6 + 8));
	BOOST_CHECK_EQUAL_COLLECTIONS(boost::begin(src) + 18, boost::begin(src) + 18 + 8, buffer, buffer + 8);
}

BOOST_AUTO_TEST_CASE(test_codec_io_copy_n_looped_test) {

	for (int i=0; i<=16; ++i) {
		char v0[16];
		std::fill(boost::begin(v0), boost::end(v0), 0x55);

		char v1[16];
		std::fill(boost::begin(v1), boost::end(v1), 0xaa);

		char* itr = boost::begin(v1);
		const int result = moneta::codec::io::copy_n(itr, boost::begin(v1) + i, 16, boost::begin(v0));

		const int expected = ((i == 16)? 16 : 0) - (sizeof(v0) - i);
		BOOST_CHECK_EQUAL(result, expected);

		// Check that at least some data got through.
		BOOST_CHECK_EQUAL_COLLECTIONS(v0, v0+i, v1, v1+i);
	}

}
