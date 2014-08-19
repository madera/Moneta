#include "stdafx.h"
#include <moneta/codec/xml/xml_encoder.hxx>
#include "../../model/LettersTree.hxx"

// Test tree:
//
//     A________
//    / \ \     \
//   f  g  B__   h
//        / \ \
//       C  i  D
//     / |    / \
//    j  k   l   E
//              / \
//             m   n
//

MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, f))

static void static_test() {
	BOOST_MPL_ASSERT((moneta::traits::detail::xml_attribute<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT((moneta::codec::detail::has_xml_attribute<A>));
	BOOST_MPL_ASSERT_NOT((moneta::codec::detail::has_xml_attribute<E>));

	BOOST_MPL_ASSERT((moneta::codec::detail::is_first_xml_attribute<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::codec::detail::is_first_xml_attribute<MONETA_MEMBER(A, int, g)>));

}

BOOST_AUTO_TEST_CASE(xml_traits_test_TEMPORARY) {
	moneta::codec::encode<moneta::codec::xml>(A(), 0, 0);
}
