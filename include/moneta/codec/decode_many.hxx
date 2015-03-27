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
