#pragma once

namespace moneta { namespace codec {

	template <class GroupDecoder, class Iterator, class Visitor>
	int decode_many(Iterator begin, Iterator end, Visitor& visitor) {
		Iterator begin_ = begin;
		Iterator end_ = end;

		int total_consumed = 0;

		int result;
		do {
			result = GroupDecoder()(begin_, end_, visitor);
			if (result > 0) {
				begin_ += result;
				total_consumed += result;
			}
		} while (result > 0);

		return total_consumed;
	}

}}
