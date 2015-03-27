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

#pragma once

// TODO: Substitute with Boost's recent solution: Boost TTI.
#define DEFINE_HAS_MEMBER_TRAIT(name) \
template<class T> \
struct has_member_##name { \
	struct A { int name; }; \
	struct B : T, A {}; \
	template<typename C, C> struct ChT; \
	template<typename C> static char (&f(ChT<int A::*, &C::name>*))[1]; \
	template<typename C> static char (&f(...))[2]; \
	static bool const value = sizeof(f<B>(0)) == 2; \
	typedef boost::mpl::bool_<value> type; \
};

#define DEFINE_HAS_NESTED_TYPE(name) \
template <typename T> \
struct has_nested_type_##name { \
	typedef char yes[1]; \
	typedef char no[2]; \
	template <typename C> static yes& test(typename C::name*); \
	template <typename> static no& test(...); \
	static const bool value = sizeof(test<T>(0)) == sizeof(yes); \
	typedef boost::mpl::bool_<value> type; \
};
