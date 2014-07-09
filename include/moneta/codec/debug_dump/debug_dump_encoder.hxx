#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec {

 	struct debug_dump;
 
	template <class T>
	struct encoder<debug_dump, T> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			std::ostringstream oss;
			return 0;
		}
	};

}}
