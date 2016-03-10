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
#include <moneta/codec/_aux/io/ostringstream.hxx>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

struct callable {
	std::string _text;

	callable(const std::string& text)
	 : _text(text) {}

	template <class Iterator>
	int operator()(Iterator begin, Iterator /*end*/) const {
		std::copy(_text.begin(), _text.end(), begin);
		return _text.size();
	}
};

BOOST_AUTO_TEST_CASE(test_codec_io_ostringstream_test) {
	
	{
		char buffer[1024];
		std::fill(boost::begin(buffer), boost::end(buffer), 0x55);

		char* itr = boost::begin(buffer);
		int result = moneta::codec::io::make_ostringstream(itr, boost::end(buffer))
			<< '<' << "Hello world!" << '>'
			<< std::string(" How do you do?")
			<< callable("\nTesting a callable")
		;

		BOOST_CHECK_EQUAL(result, 48);
	}

	{
		char buffer[10];
		std::fill(boost::begin(buffer), boost::end(buffer), 0x55);

		char* itr = boost::begin(buffer);
		int result = moneta::codec::io::make_ostringstream(itr, boost::end(buffer))
			<< '<' << "Hello world!" << '>'
			<< std::string(" How do you do?")
		;

		// 3 left for the write operation.
		BOOST_CHECK_EQUAL(result, -3);
	}
}
