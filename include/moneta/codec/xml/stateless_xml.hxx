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
#include "stateless_xml_encoder.hxx"

namespace moneta { namespace codec {

	struct xml {
		typedef stateless_xml_encoder_implementation::stateless_xml_encoder encoder;
		//typedef detail::xml::xml_decoder decoder;
		typedef std::string typecode_type;
	};

}}
