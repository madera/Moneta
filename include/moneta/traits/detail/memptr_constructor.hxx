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
#include <boost/mpl/at.hpp>

// XXX: Boost.PPize this.

namespace moneta { namespace traits { namespace detail {

	template <class R, class MemPtrs, class ClassType, int Size>
	struct memptr_constructor;

	template <class R, class MemPtrs, class ClassType>
	struct memptr_constructor<R, MemPtrs, ClassType, 1> {
		R operator()(ClassType& instance) {
			return R(
				typename boost::mpl::at_c<MemPtrs, 0>::type()(instance)
			);
		}

		R operator()(const ClassType& instance) {
			return R(
				typename boost::mpl::at_c<MemPtrs, 0>::type()(instance)
			);
		}
	};

	template <class R, class MemPtrs, class ClassType>
	struct memptr_constructor<R, MemPtrs, ClassType, 2> {
		R operator()(ClassType& instance) {
			return R(
				typename boost::mpl::at_c<MemPtrs, 0>::type()(instance),
				typename boost::mpl::at_c<MemPtrs, 1>::type()(instance)
			);
		}

		R operator()(const ClassType& instance) {
			return R(
				typename boost::mpl::at_c<MemPtrs, 0>::type()(instance),
				typename boost::mpl::at_c<MemPtrs, 1>::type()(instance)
			);
		}
	};

}}} 
