// TODO: Determine best name for the file.

#pragma once
#include <iterator>

namespace moneta { namespace codec {

	namespace io {

		template <typename InputIterator, typename OutputIterator>
		int copy_cstr(InputIterator& next, InputIterator end, OutputIterator result, const bool do_suffix = true) {
			int consumed = 0;

			while (next != end && *next != 0) {
				*result++ = *next++;
				++consumed;
			}

			if (do_suffix) {
				*result++ = 0;
			}

			return consumed;
		}

	}

}}
