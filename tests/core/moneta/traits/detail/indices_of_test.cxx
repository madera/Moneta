#include "stdafx.h"
#include <moneta/traits/detail/indices_of.hxx>
#include <boost/mpl/at.hpp>

inline void static_test() {
	using moneta::traits::detail::indices_of;
	namespace mpl = boost::mpl;

	{
		typedef indices_of<
			mpl::vector<long, float, short, double, float, long, long double>,
			boost::is_float<mpl::_1>
		>::type indices_t;

		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 0>::type, mpl::int_<1> >));
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 1>::type, mpl::int_<3> >));
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 2>::type, mpl::int_<4> >));
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 3>::type, mpl::int_<6> >));
	}

	{
		typedef indices_of<
			mpl::vector<long, float, short, double, float, long, long double>,
			boost::is_same<long, mpl::_1>
		>::type indices_t;

		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 0>::type, mpl::int_<0> >));
		BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at_c<indices_t, 1>::type, mpl::int_<5> >));
	}
}
