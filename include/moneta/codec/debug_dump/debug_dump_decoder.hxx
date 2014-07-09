#pragma once
#include "../decoder.hxx"

namespace moneta { namespace codec {

	struct debug_dump;

	template <class T>
	struct decoder<debug_dump, T> {
		template <class Iterator>
		int operator()(T& target, Iterator begin, Iterator end) const {
			int length = std::distance(begin, end);
			if (length < sizeof(T)) {
				return length - sizeof(T);
			}

			std::copy(begin, begin + sizeof(T), reinterpret_cast<char*>(&target));
			return sizeof(T);
		}
	};

}}
