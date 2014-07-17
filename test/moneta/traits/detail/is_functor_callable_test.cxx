#include "stdafx.h"
#include <moneta/traits/detail/is_functor_callable.hxx>
#include <string>

struct a {
	void operator()()  {}
	void operator()(int)  {}
	void operator()(std::string)  {}
};

struct b : a {
	template <class T, class U, class V>
	void operator()(T x, U y, V z)  {}
};

static void static_test() {
	using moneta::traits::detail::is_functor_callable;

	// Test limitation... unfortunate.
	BOOST_MPL_ASSERT_NOT((is_functor_callable<a, void(void)>));

	BOOST_MPL_ASSERT((is_functor_callable<a, void(int)>));
	BOOST_MPL_ASSERT((is_functor_callable<a, void(std::string)>));
	BOOST_MPL_ASSERT((is_functor_callable<a, void(char*)>));
	BOOST_MPL_ASSERT((is_functor_callable<a, void(const char*)>));
	
	BOOST_MPL_ASSERT_NOT((is_functor_callable<a, void(int, int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<a, void(std::string, int)>));

	BOOST_MPL_ASSERT((is_functor_callable<b, void(int, int, int)>));

	//
	// XXX: This test is broken, and the code should be fixed.
	//	Derived classes' methods should be detected too.
	//

	//BOOST_MPL_ASSERT((is_functor_callable<b, void(int)>));
	//BOOST_MPL_ASSERT((is_functor_callable<b, void(std::string)>));
	//BOOST_MPL_ASSERT((is_functor_callable<b, void(char*)>));
	//BOOST_MPL_ASSERT((is_functor_callable<b, void(const char*)>));
}
