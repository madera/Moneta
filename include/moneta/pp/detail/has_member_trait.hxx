#pragma once

// TODO: Substitute with Boost's recent solution.
#define DEFINE_HAS_MEMBER_TRAIT(name) \
template<class T> \
struct has_member_##name { \
	struct A { int name; }; \
	struct B : T, A {}; \
	template<typename C, C> struct ChT; \
	template<typename C> static char (&f(ChT<int A::*, &C::name>*))[1]; \
	template<typename C> static char (&f(...))[2]; \
	static bool const value = sizeof(f<B>(0)) == 2; \
};
