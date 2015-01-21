#include "stdafx.h"
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
