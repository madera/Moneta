#pragma once
#include <iterator>

namespace moneta { namespace codec {

	namespace io {

		template <typename InputIterator, typename OutputIterator>
		int read(
			InputIterator next, InputIterator end,
			OutputIterator result_begin, OutputIterator result_end,
			const bool zero_terminate = true
		) {
			int consumed = 0;

			while (
				result_begin != result_end &&
				next != end && *next != 0
			) {
				*result_begin++ = *next++;
				++consumed;
			}

			if (zero_terminate && result_begin != result_end) {
				*result_begin++ = 0;
			}

			return consumed;
		}

	}

}}
