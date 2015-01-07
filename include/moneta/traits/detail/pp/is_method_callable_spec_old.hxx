
// TODO: Delete this file. It's old.

// WARNING: No guardians!
//
// ==========================================================================
// === WARNING: C++03 Limitation: Doesn't work for zero argument methods! ===
// ==========================================================================
//
// Input macros:
//
//	IS_METHOD_CALLABLE_NAME
//	IS_METHOD_CALLABLE_FUNCTION
//	IS_METHOD_CALLABLE_ARITY (default 10)
//
// Based on http://www.rsdn.ru/forum/cpp/2759773.1
//

#ifndef IS_METHOD_CALLABLE_ARITY
#define IS_METHOD_CALLABLE_ARITY 10
#endif

#include "../../../_aux/mplx_nullref.hxx"
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/bool.hpp>

namespace {
	template<class T>
	struct BOOST_PP_CAT(has_member_, IS_METHOD_CALLABLE_NAME) { // XXX: Use pre-existing.

#ifdef BOOST_MSVC
		struct A { int IS_METHOD_CALLABLE_FUNCTION; };
#else
		struct A { int IS_METHOD_CALLABLE_FUNCTION(); };
#endif

		struct B : T, A {};
		template<typename C, C> struct ChT;
		template<typename C> static char(&f(ChT<int A::*, &C::IS_METHOD_CALLABLE_FUNCTION>*))[1];
		template<typename C> static char(&f(...))[2];
		static const bool value = sizeof(f<B>(0)) == 2;
		typedef ::boost::mpl::bool_<value> type;
	};

	template <typename T> class dummy {};
	template <typename T, typename U> const U& operator,(const U&, dummy<T>);
	template <typename T, typename U>       U& operator,(U&, dummy<T>);
}

template <typename T, typename Signature>
struct IS_METHOD_CALLABLE_NAME {
	class yes {};
	class no { yes m[2]; };

	template <typename U, typename Result>
	struct deducer {
		static yes fx(Result);
		static no fx(dummy<T>);
		static no fx(...);
		static no fx(no);
	};

	template <typename U>
	struct deducer<U, void> {
		static yes fx(...);
		static no fx(no);
	};

	struct derived : public T {
		using T::IS_METHOD_CALLABLE_FUNCTION;
		no IS_METHOD_CALLABLE_FUNCTION(...) const;
	};

	typedef typename ::boost::mpl::if_<
		::boost::is_const<T>,
		::boost::add_const<derived>,
		derived
	>::type derived_type;

	template <bool Good, typename F>
	struct impl : ::boost::mpl::false_ {};

	typedef impl<BOOST_PP_CAT(has_member_, IS_METHOD_CALLABLE_NAME) < T > ::value, Signature> type;
	static const bool value = type::value;
//

#define ENUM_Ts_SPEC(z, n, size) BOOST_PP_CAT(mplx::nullref<T,n)>()BOOST_PP_COMMA_IF(BOOST_PP_NOT_EQUAL(n,BOOST_PP_DEC(size)))
#define ENUM_Ts(n) BOOST_PP_REPEAT(n, ENUM_Ts_SPEC, n)

#define DEFINE_IMPL_WITH_ARGS(n) \
	template <typename Result, BOOST_PP_ENUM_PARAMS(n, typename T)> \
	struct impl<true, Result(BOOST_PP_ENUM_PARAMS(n, T))> : ::boost::mpl::bool_< \
		sizeof(deducer<T, Result>::fx(( \
			mplx::nullref<derived_type>().IS_METHOD_CALLABLE_FUNCTION(ENUM_Ts(n)), mplx::nullref<dummy<T> >())) \
		) == sizeof(yes) \
	> {};													

#define IMPL_SPEC(z, n, data) DEFINE_IMPL_WITH_ARGS(BOOST_PP_INC(n))
BOOST_PP_REPEAT(IS_METHOD_CALLABLE_ARITY, IMPL_SPEC, _)
#undef IMPL_SPEC

#undef DEFINE_IMPL_WITH_ARGS

#undef ENUM_Ts
#undef ENUM_Ts_SPEC
};

#undef IS_METHOD_CALLABLE_FUNCTION
#undef IS_METHOD_CALLABLE_NAME
#undef IS_METHOD_CALLABLE_ARITY
