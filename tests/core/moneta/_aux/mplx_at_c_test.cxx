#include "stdafx.h"
#include <moneta/traits/member_name.hxx>
#include <moneta/_aux/mplx_at_c.hxx>
#include "../model/Person.hxx"
#include "../model/tree/A.hxx"

template <class T>
struct printer {
	enum _ { value = 0 };
};

template <>
struct printer<int> {
	enum _ { value = 123 };
};

struct test_0 {
	template <class T>
	void operator()() const {
		BOOST_CHECK_EQUAL(printer<T>::value, 123);
	}
};

BOOST_AUTO_TEST_CASE(at_c_0_mplx_test) {
	mplx::at_c<boost::mpl::vector5<short, char, double, int, float> >(3, test_0());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct test_1 {
	template <class T>
	void operator()() const {
		BOOST_CHECK_EQUAL(moneta::traits::member_name<T>::get(), "Height");
	}
};

BOOST_AUTO_TEST_CASE(at_c_1_mplx_test) {
	mplx::at_c<moneta::traits::members<Person>::type>(2, test_1());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct test_2 {
	template <class T>
	void operator()() const {
		BOOST_CHECK_EQUAL(moneta::traits::member_name<T>::get(), "b");
	}
};

BOOST_AUTO_TEST_CASE(at_c_2_mplx_test) {
	mplx::at_c<moneta::traits::members<A>::type>(2, test_2());
}

