#include "stdafx.h"
#include <moneta/codec/_aux/io/copy_n.hxx>
#include <moneta/codec/_aux/codec_io.hxx>
#include <moneta/codec/_aux/tabs.hxx>

struct callable {
	std::string _text;

	callable(const std::string& text)
	 : _text(text) {}

	template <class Iterator>
	int operator()(Iterator begin, Iterator end) const {
		std::copy(_text.begin(), _text.end(), begin);
		return _text.size();
	}
};

BOOST_AUTO_TEST_CASE(ostringstream_codec_io_test) {
	
	{
		char buffer[1024];
		std::fill(std::begin(buffer), std::end(buffer), 0x55);

		int result = moneta::codec::detail::make_ostringstream(std::begin(buffer), std::end(buffer))
			<< '<' << "Hello world!" << '>'
			<< std::string(" How do you do?")
			<< callable("\nTesting a callable")
		;

		BOOST_CHECK_EQUAL(result, 48);
	}

	{
		char buffer[10];
		std::fill(std::begin(buffer), std::end(buffer), 0x55);

		int result = moneta::codec::detail::make_ostringstream(std::begin(buffer), std::end(buffer))
			<< '<' << "Hello world!" << '>'
			<< std::string(" How do you do?")
		;

		// 3 left for the write operation.
		BOOST_CHECK_EQUAL(result, -3);
	}
}
