#include "stdafx.h"
#include <moneta/codec/multi_decoder.hxx>
#include "../model/simple/ThreeInts.hxx"
#include "../model/simple/FourInts.hxx"

void foo() {
	moneta::codec::multi_decoder<FourInts> decoder;

	moneta::codec::multi_decoder<ThreeInts>::variant_type entity;

	int result = 1;// decoder(begin, end, entity);
	if (result > 0) {
		//std::cout << "Read " << result << " bytes and got: " << entity << std::endl;
	} else if (result == 0) {
		std::cout << "None of the decoders know how to decode" << std::endl;
	} else if (result < 0) {
		std::cout << "Need at least " << result << " more bytes to decode" << std::endl;
	}
}
