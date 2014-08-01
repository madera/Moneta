#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec {

 	struct rawbin;
 
	template <class T>
	struct value_encoder<rawbin, T, typename boost::enable_if<boost::is_pod<T> >::type> {
		template <class Iterator>
		int operator()(const T& value, Iterator begin, Iterator end) const {
			int length = std::distance(begin, end);
			if (length < sizeof(T)) {
				return length - sizeof(T);
			}

			const char* data = reinterpret_cast<const char*>(&value);
			std::copy(data, data + sizeof(T), begin);
			return sizeof(T);
		}
	};

}}
