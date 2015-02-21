#include "stdafx.h"
#include <moneta/traits/detail/is_functor_callable.hxx>

struct a {
	void operator()() {}
	void operator()(int) {}
	void operator()(std::string) {}
};

struct b : a {
	template <class T, class U, class V>
	void operator()(T, U, V) {}
};

inline void static_test() {
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct some_struct {};

struct test_state {};

struct e0 {
	template <class Entity>
	void operator()(Entity&) const {
	}
};

struct e1 {
	template <class Entity, class Path>
	void operator()(Entity&, Path) const {
	}
};

struct e2 {
	template <class Entity, class Path>
	void operator()(Entity&, Path, test_state&) const {
	}
};

inline void traverse_inspired_static_test() {
	using moneta::traits::detail::is_functor_callable;

	// Test limitation... unfortunate.
	BOOST_MPL_ASSERT_NOT((is_functor_callable<a, void(void)>));

	//

	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const some_struct )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      some_struct )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const some_struct&)>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      some_struct&)>));

	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const int )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      int )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const int&)>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      int&)>));

	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const char* )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      char* )>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(const char*&)>));
	BOOST_MPL_ASSERT((is_functor_callable<e0, void(      char*&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const some_struct , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      some_struct , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const some_struct&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      some_struct&, const int)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const int , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      int , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const int&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      int&, const int)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const char* , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      char* , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const char*&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      char*&, const int)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const some_struct&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      some_struct&, const int, test_state)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const int , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      int , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const int&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      int&, const int, test_state)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const char* , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      char* , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(const char*&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e0, void(      char*&, const int, test_state)>));

	//

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const some_struct )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      some_struct )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const some_struct&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      some_struct&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const int )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      int )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const int&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      int&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const char* )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      char* )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const char*&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      char*&)>));

	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const some_struct , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      some_struct , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const some_struct&, const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      some_struct&, const int)>));

	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const int , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      int , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const int&, const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      int&, const int)>));

	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const char* , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      char* , const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(const char*&, const int)>));
	BOOST_MPL_ASSERT((is_functor_callable<e1, void(      char*&, const int)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const some_struct&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      some_struct&, const int, test_state)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const int , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      int , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const int&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      int&, const int, test_state)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const char* , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      char* , const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(const char*&, const int, test_state)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e1, void(      char*&, const int, test_state)>));

	//

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const some_struct )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      some_struct )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const some_struct&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      some_struct&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const int )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      int )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const int&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      int&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const char* )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      char* )>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const char*&)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      char*&)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const some_struct , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      some_struct , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const some_struct&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      some_struct&, const int)>));
	
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const int , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      int , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const int&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      int&, const int)>));

	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const char* , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      char* , const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(const char*&, const int)>));
	BOOST_MPL_ASSERT_NOT((is_functor_callable<e2, void(      char*&, const int)>));


	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      some_struct , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const some_struct&, const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      some_struct&, const int, test_state)>));
	
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const int , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      int , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const int&, const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      int&, const int, test_state)>));

	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const char* , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      char* , const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(const char*&, const int, test_state)>));
	BOOST_MPL_ASSERT((is_functor_callable<e2, void(      char*&, const int, test_state)>));

	//
	// TODO: Make these tests better. Make them comprehensive.
	//
}
