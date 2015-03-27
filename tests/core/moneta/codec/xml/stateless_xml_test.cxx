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

#include "pch.hxx"
#include <moneta/codec/xml/stateless_xml.hxx>

inline void static_test() {
	BOOST_MPL_ASSERT((boost::is_same<
		moneta::codec::xml::encoder,
		moneta::codec::stateless_xml_encoder_implementation::stateless_xml_encoder
	>));

	// XXX
	//BOOST_MPL_ASSERT((boost::is_same<
	//	moneta::codec::xml::decoder,
	//	moneta::codec::stateless_xml_encoder_implementation::stateless_xml_decoder
	//>));

	BOOST_MPL_ASSERT((boost::is_same<
		moneta::codec::xml::typecode_type,
		std::string
	>));
}
