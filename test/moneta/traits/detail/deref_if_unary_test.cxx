#include "stdafx.h"
#include <moneta/traits/detail/deref_if_unary.hxx>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/sequence/comparison/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/vector.hpp>

namespace fusion = boost::fusion;
namespace mpl    = boost::mpl;

using moneta::traits::detail::deref_if_unary;

static void static_test() {
	using boost::mpl::equal;

	// Using mpl vectors
	//
	BOOST_MPL_ASSERT((equal<deref_if_unary<mpl::vector<int>        >::type, int       >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<mpl::vector<int&>       >::type, int       >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<mpl::vector<const int>  >::type, const int >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<mpl::vector<const int&> >::type, const int&>));



	BOOST_MPL_ASSERT((equal<
		deref_if_unary<mpl::vector<int, char> >::type,
		mpl::vector<int, char>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<mpl::vector<int&, char&> >::type,
		mpl::vector<int&, char&>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<mpl::vector<int, char> >::type,
		mpl::vector<int, char>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<mpl::vector<const int&, const char&> >::type,
		mpl::vector<const int&, const char&>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<mpl::vector<int, char&, const double> >::type,
		mpl::vector<int, char&, const double>
	>));


	// Using fusion vectors
	//
	BOOST_MPL_ASSERT((equal<deref_if_unary<fusion::vector<int       > >::type, int       >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<fusion::vector<int&      > >::type, int       >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<fusion::vector<const int > >::type, const int >));
	BOOST_MPL_ASSERT((equal<deref_if_unary<fusion::vector<const int&> >::type, const int&>));



	BOOST_MPL_ASSERT((equal<
		deref_if_unary<fusion::vector<int, char> >::type,
		mpl::vector<int, char>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<fusion::vector<int&, char&> >::type,
		mpl::vector<int&, char&>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<fusion::vector<int, char> >::type,
		mpl::vector<int, char>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<fusion::vector<const int&, const char&> >::type,
		mpl::vector<const int&, const char&>
	>));

	BOOST_MPL_ASSERT((equal<
		deref_if_unary<fusion::vector<int, char&, const double> >::type,
		mpl::vector<int, char&, const double>
	>));
}

BOOST_AUTO_TEST_CASE(deref_if_unary_opfx_test_0) {
	using moneta::traits::detail::deref_if_unary;
	using fusion::vector;

	typedef vector<int> vi_t;
	vi_t vi(1);
//	BOOST_CHECK_EQUAL(fusion::at_c<0>(vi), deref_if_unary<vi_t>()(vi));
}

BOOST_AUTO_TEST_CASE(deref_if_unary_opfx_test_1) {
	typedef fusion::vector<int, char> vector_t;
	vector_t v(1, 'a');
//	BOOST_CHECK(fusion::equal_to(v, deref_if_unary<vector_t>()(v)));
}

BOOST_AUTO_TEST_CASE(deref_if_unary_opfx_test_2) {
	typedef fusion::vector<int, char> vector_t;
	typedef fusion::vector<int&, char&> vtie_t;
	
	vector_t v(1, 'a');
	//vtie_t vt(moneta::traits::detail::make_reference_transformer(v));
	//vector_t vt2(moneta::traits::detail::make_reference_transformer(v));

	// Alter using vtie.
//	fusion::at_c<0>(vt)++;

//	BOOST_CHECK_EQUAL(fusion::at_c<0>(v), fusion::at_c<0>(vt));

	int x = 0;
	//vector_t result = deref_if_unary<vector_t>()(v);
//	vtie_t tie = result;
}
