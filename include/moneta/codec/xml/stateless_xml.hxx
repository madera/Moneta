#pragma once
#include "stateless_xml_encoder.hxx"

namespace moneta { namespace codec {

	struct xml {
		typedef stateless_xml_encoder_implementation::stateless_xml_encoder encoder;
		//typedef detail::xml::xml_decoder decoder;
		typedef std::string typecode_type;
	};

}}
