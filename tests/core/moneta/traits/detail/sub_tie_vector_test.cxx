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
#include <moneta/traits/detail/sub_tie_vector.hxx>
#include <moneta/traits/rtuple.hxx>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/equal.hpp>
#include <string>
#include "../../model/Cat.hxx"

inline void static_test() {

	using moneta::traits::detail::sub_tie_vector;
	using boost::fusion::vector;
	using boost::mpl::vector_c;

	typedef boost::fusion::vector<char , short , int > tuple_t;
	typedef boost::fusion::vector<char&, short&, int&>   tie_t;

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0>
		>::type,
		vector<char&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0, 1>
		>::type,
		vector<char&, short&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 1, 2>
		>::type,
		vector<short&, int&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0, 1, 2>
		>::type,
		vector<char&, short&, int&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 2, 0>
		>::type,
		vector<int&, char&>
	>));

	// ===
	// ===> This test should cause an assertion fail inside sub_tie_vector
	// ===  --------------------------------------------------------------
	//
	//BOOST_MPL_ASSERT((boost::mpl::equal<
	//	sub_tie_vector<
	//		vector<char, short, int>,
	//		vector_c<int, 0, 1, 2, 3>
	//	>::type,
	//	vector<char&, short&, int&>
	//>));
}

BOOST_AUTO_TEST_CASE(tuple_sub_tie_test) {
	using boost::mpl::vector_c;
	using moneta::traits::detail::sub_tie_vector;

	typedef moneta::traits::tuple<Cat>::type tuple_type;
	tuple_type tuple;
	sub_tie_vector<tuple_type, vector_c<int, 0      > >()(tuple);
	sub_tie_vector<tuple_type, vector_c<int, 0, 1   > >()(tuple);
	sub_tie_vector<tuple_type, vector_c<int, 0, 1, 2> >()(tuple);

	typedef moneta::traits::tuple<const Cat>::type const_tuple_type;
	const_tuple_type const_tuple;
	sub_tie_vector<const_tuple_type, vector_c<int, 0      > >()(const_tuple);
	sub_tie_vector<const_tuple_type, vector_c<int, 0, 1   > >()(const_tuple);
	sub_tie_vector<const_tuple_type, vector_c<int, 0, 1, 2> >()(const_tuple);
}

BOOST_AUTO_TEST_CASE(rtuple_sub_tie_test) {
	using boost::mpl::vector_c;
	using moneta::traits::detail::sub_tie_vector;

	typedef moneta::traits::rtuple<Cat>::type rtuple_type;
	rtuple_type rtuple;
	sub_tie_vector<rtuple_type, vector_c<int, 0      > >()(rtuple);
	sub_tie_vector<rtuple_type, vector_c<int, 0, 1   > >()(rtuple);
	sub_tie_vector<rtuple_type, vector_c<int, 0, 1, 2> >()(rtuple);

	typedef moneta::traits::rtuple<const Cat>::type const_rtuple_type;
	const_rtuple_type const_rtuple;
	sub_tie_vector<const_rtuple_type, vector_c<int, 0      > >()(const_rtuple);
	sub_tie_vector<const_rtuple_type, vector_c<int, 0, 1   > >()(const_rtuple);
	sub_tie_vector<const_rtuple_type, vector_c<int, 0, 1, 2> >()(const_rtuple);
}
