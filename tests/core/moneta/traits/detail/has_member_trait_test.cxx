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

inline void static_test() {
	BOOST_MPL_ASSERT((has_member_x<a>));
	BOOST_MPL_ASSERT((has_member_f<a>));
	BOOST_MPL_ASSERT((has_member_floating_type<a>));

	BOOST_MPL_ASSERT_NOT((has_nested_type_x<a>));
	BOOST_MPL_ASSERT_NOT((has_nested_type_f<a>));
	BOOST_MPL_ASSERT((has_nested_type_floating_type<a>));
}
