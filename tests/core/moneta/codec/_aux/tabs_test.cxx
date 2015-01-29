#include "stdafx.h"
#include <moneta/codec/_aux/tabs.hxx>

BOOST_AUTO_TEST_CASE(test_codec_tabs_test) {
	using moneta::codec::aux::tabs;
	
	BOOST_CHECK_EQUAL(tabs<-1>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-2>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-3>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-4>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-5>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-10>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-100>::get(), "");
	BOOST_CHECK_EQUAL(tabs<-999>::get(), "");

	BOOST_CHECK_EQUAL(tabs< 0>::get(), "");
	BOOST_CHECK_EQUAL(tabs< 1>::get(), "\t");
	BOOST_CHECK_EQUAL(tabs< 2>::get(), "\t\t");
	BOOST_CHECK_EQUAL(tabs< 3>::get(), "\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 4>::get(), "\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 5>::get(), "\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 6>::get(), "\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 7>::get(), "\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 8>::get(), "\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs< 9>::get(), "\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<10>::get(), "\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<11>::get(), "\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<12>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<13>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<14>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<15>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<16>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<17>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<18>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<19>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<20>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<21>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<22>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<23>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<24>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<25>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<26>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<27>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<28>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<29>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<30>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<31>::get(), "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL(tabs<32>::get(), "");
}

BOOST_AUTO_TEST_CASE(test_codec_tabs_test_pp) {
	using moneta::codec::aux::tabs;

	// Generate test for every tab<N> value.
	//
	#define __IMPL_TOKEN__(z, n, text) \
		BOOST_CHECK_EQUAL(std::string(n, '\t'), tabs<n>::get());

	BOOST_PP_REPEAT(MONETA_CODEC_TABS_MAX, __IMPL_TOKEN__, _)
	
	#undef __IMPL_TOKEN__

	// Generate test for every tab<N> value, testing big modular indices.
	//
	#define __IMPL_TOKEN__(z, n, text) \
		BOOST_CHECK_EQUAL(std::string(n, '\t'), tabs<n + (16 * MONETA_CODEC_TABS_MAX)>::get());
	
	BOOST_PP_REPEAT(MONETA_CODEC_TABS_MAX, __IMPL_TOKEN__, _)
	
	#undef __IMPL_TOKEN__

	// Test modulo on max value. Should be empty.
	BOOST_CHECK_EQUAL(std::string(), tabs<MONETA_CODEC_TABS_MAX>::get());
}
