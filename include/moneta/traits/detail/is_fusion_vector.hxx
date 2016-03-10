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
#include <boost/fusion/container/vector.hpp>

namespace moneta { namespace traits { namespace detail {

	template <typename T0>
	struct is_fusion_vector
	 : boost::false_type {};

//	template <typename T0>
//	struct is_fusion_vector<
//		boost::fusion::vector<T0>
//	> : boost::true_type {};

	template <typename T0>
	struct is_fusion_vector<
		boost::fusion::vector1<T0>
	> : boost::true_type {};

//	template <typename T0, typename T1>
//	struct is_fusion_vector<
//		boost::fusion::vector<T0, T1>
//	> : boost::true_type {};

	template <typename T0, typename T1>
	struct is_fusion_vector<
		boost::fusion::vector2<T0, T1>
	> : boost::true_type {};

//	template <typename T0, typename T1, typename T2>
//	struct is_fusion_vector<boost::fusion::vector<T0, T1, T2> > : boost::true_type {};
	template <typename T0, typename T1, typename T2>
	struct is_fusion_vector<boost::fusion::vector3<T0, T1, T2> > : boost::true_type {};


//	template <typename T0, typename T1, typename T2, typename T3>
//	struct is_fusion_vector<boost::fusion::vector<T0, T1, T2, T3> > : boost::true_type {};
	template <typename T0, typename T1, typename T2, typename T3>
	struct is_fusion_vector<boost::fusion::vector4<T0, T1, T2, T3> > : boost::true_type {};


//	template <typename T0, typename T1, typename T2, typename T3, typename T4>
//	struct is_fusion_vector<boost::fusion::vector<T0, T1, T2, T3, T4> > : boost::true_type {};
	template <typename T0, typename T1, typename T2, typename T3, typename T4>
	struct is_fusion_vector<boost::fusion::vector5<T0, T1, T2, T3, T4> > : boost::true_type {};

}}}
