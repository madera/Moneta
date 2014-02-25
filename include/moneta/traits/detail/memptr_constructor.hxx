#pragma once
#include <boost/mpl/at.hpp>

namespace moneta { namespace traits { namespace detail {

	template <class R, class MemPtrs, class ClassType, int Size>
	struct memptr_constructor;

	template <class R, class MemPtrs, class ClassType>
	struct memptr_constructor<R, MemPtrs, ClassType, 1> {
		R operator()(ClassType& instance) {
			return R(
				boost::mpl::at_c<MemPtrs, 0>::type()(instance)
			);
		}

		R operator()(const ClassType& instance) {
			return R(
				boost::mpl::at_c<MemPtrs, 0>::type()(instance)
			);
		}
	};

	template <class R, class MemPtrs, class ClassType>
	struct memptr_constructor<R, MemPtrs, ClassType, 2> {
		R operator()(ClassType& instance) {
			return R(
				boost::mpl::at_c<MemPtrs, 0>::type()(instance),
				boost::mpl::at_c<MemPtrs, 1>::type()(instance)
			);
		}

		R operator()(const ClassType& instance) {
			return R(
				boost::mpl::at_c<MemPtrs, 0>::type()(instance),
				boost::mpl::at_c<MemPtrs, 1>::type()(instance)
			);
		}
	};

}}} 
