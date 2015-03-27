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

namespace moneta { namespace codec { namespace io {

	struct is_whitespace {		
		bool operator()(const char x) const {
			return std::isspace(x) != 0;
		}
	};

	template <class Iterator>
	int consume_whitespaces(Iterator begin, Iterator end) {
		return consume_while(begin, end, is_whitespace());
	}

}}}
