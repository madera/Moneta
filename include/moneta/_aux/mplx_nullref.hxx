#pragma once
#include <boost/mpl/clear.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/vector.hpp>

namespace mplx {

	template <class T>
	T& nullref() {
		return *((T*) 0);
	}

}
