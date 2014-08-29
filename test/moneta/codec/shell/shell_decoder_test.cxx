#include "stdafx.h"
#include <moneta/codec/shell/shell_decoder.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include "../../model/simple/ThreeInts.hxx"
#include "../../model/simple/FourInts.hxx"

//template <class Path, class Entity>
//struct enter_entity_encoder<entity_encoder_test_codec, Path, Entity> {
//	template <class Iterator>
//	int operator()(const Entity& entity, Iterator begin, Iterator end) const {
//		if (begin == end) {
//			return -1;
//		}
//
//		*begin = 'E';
//		return 1;
//	}
//};

BOOST_AUTO_TEST_CASE(shell_decoder_test) {
}
