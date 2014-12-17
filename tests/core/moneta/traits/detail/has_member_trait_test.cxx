#include "stdafx.h"
#include <moneta/traits/detail/has_member_trait.hxx>

struct a {
	typedef double floating_type;

	int x;
	void f() {}
};

DEFINE_HAS_MEMBER_TRAIT(foo)
DEFINE_HAS_MEMBER_TRAIT(f)
DEFINE_HAS_MEMBER_TRAIT(x)
DEFINE_HAS_MEMBER_TRAIT(floating_type)

DEFINE_HAS_NESTED_TYPE(foo)
DEFINE_HAS_NESTED_TYPE(f)
DEFINE_HAS_NESTED_TYPE(x)
DEFINE_HAS_NESTED_TYPE(floating_type)

static void static_test() {
	BOOST_MPL_ASSERT((has_member_x<a>));
	BOOST_MPL_ASSERT((has_member_f<a>));
	BOOST_MPL_ASSERT((has_member_floating_type<a>));

	BOOST_MPL_ASSERT_NOT((has_nested_type_x<a>));
	BOOST_MPL_ASSERT_NOT((has_nested_type_f<a>));
	BOOST_MPL_ASSERT((has_nested_type_floating_type<a>));
}
