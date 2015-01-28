#include "stdafx.h"
#include <moneta/codec/_aux/codec_io.hxx>
#include <moneta/codec/_aux/tabs.hxx>

BOOST_AUTO_TEST_CASE(copy_codec_io_test_) {

	for (int i=0; i<=16; ++i) {
		char v0[16];
		std::fill(std::begin(v0), std::end(v0), 0x55);

		char v1[16];
		std::fill(std::begin(v1), std::end(v1), 0xaa);

		BOOST_CHECK_EQUAL(
			moneta::codec::detail::copy(
				std::begin(v0), std::end(v0),
				v1, v1 + i
			),
			(i == 16)? 16 : 0 - (sizeof(v0) - i)
		);

		// Check that at least some data got through.
		BOOST_CHECK_EQUAL_COLLECTIONS(v0, v0+i, v1, v1+i);
	}

}


BOOST_AUTO_TEST_CASE(string_copy_codec_io_test) {

	char v0[16];
	std::fill(std::begin(v0), std::end(v0), 0x55);

	const char* text = "abcdef";

	moneta::codec::detail::copy(text, std::begin(v0), std::end(v0));
	BOOST_CHECK_EQUAL_COLLECTIONS(v0, v0+6, text, text+6);


	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+0), -6);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+1), -5);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+2), -4);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+3), -3);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+4), -2);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+5), -1);
	BOOST_CHECK_EQUAL(moneta::codec::detail::copy(text, v0, v0+6),  6);

}

BOOST_AUTO_TEST_CASE(tabs_codec_io_test) {
	using moneta::codec::aux::tabs;

	// Generate test for every tab<N> value.
	//
	#define __IMPL_TOKEN__(z, n, text) BOOST_CHECK_EQUAL(std::string(n, '\t'), tabs<n>::get());
	BOOST_PP_REPEAT(MONETA_CODEC_TABS_MAX, __IMPL_TOKEN__, _)
	#undef __IMPL_TOKEN__

	// Generate test for every tab<N> value, testing modular indices.
	//
	#define __IMPL_TOKEN__(z, n, text) BOOST_CHECK_EQUAL(std::string(n, '\t'), tabs<n+MONETA_CODEC_TABS_MAX>::get());
	BOOST_PP_REPEAT(MONETA_CODEC_TABS_MAX, __IMPL_TOKEN__, _)
	#undef __IMPL_TOKEN__

	BOOST_CHECK_EQUAL(std::string(), tabs<MONETA_CODEC_TABS_MAX>::get());
}

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
