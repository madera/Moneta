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
#include <moneta/codec/xml/stateless_xml_encoder.hxx>
#include "../../../model/tree/A.hxx"
#include "../../../model/Customer.hxx"
#include "../../../model/Person.hxx"

MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, f))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, g))
MONETA_XML_ATTIBUTE(A, MONETA_MEMBER(A, int, h))

MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(D, int, l))

MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, m))
MONETA_XML_ATTIBUTE(E, MONETA_MEMBER(E, int, n))

inline void static_test() {
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(A, B  , b)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(A, int, h)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(B, C  , c)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(B, int, i)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(B, D  , d)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(C, int, j)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(C, int, k)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(D, int, l)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_attribute<MONETA_MEMBER(D, E  , e)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(E, int, m)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_attribute<MONETA_MEMBER(E, int, n)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(A, B  , b)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(A, int, h)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(B, C  , c)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(B, int, i)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(B, D  , d)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(C, int, j)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(C, int, k)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(D, int, l)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_xml_element<MONETA_MEMBER(D, E  , e)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(E, int, m)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_xml_element<MONETA_MEMBER(E, int, n)>));

	BOOST_MPL_ASSERT    ((moneta::traits::has_xml_attributes<A>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::has_xml_attributes<B>));
	BOOST_MPL_ASSERT    ((moneta::traits::has_xml_attributes<E>));

	BOOST_MPL_ASSERT    ((moneta::traits::has_xml_elements<A>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::has_xml_elements<E>));

	BOOST_MPL_ASSERT    ((moneta::traits::is_first_xml_attribute_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_xml_attribute_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_xml_attribute_member<MONETA_MEMBER(A, B  , b)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_xml_attribute_member<MONETA_MEMBER(A, int, h)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_xml_attribute_member<MONETA_MEMBER(A, int, f)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_xml_attribute_member<MONETA_MEMBER(A, int, g)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_xml_attribute_member<MONETA_MEMBER(A, B  , b)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_last_xml_attribute_member<MONETA_MEMBER(A, int, h)>));

	BOOST_MPL_ASSERT    ((moneta::traits::is_first_xml_element_member<MONETA_MEMBER(C, int, j)>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_first_xml_element_member<MONETA_MEMBER(C, int, k)>));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_last_xml_element_member<MONETA_MEMBER(C, int, j)>));
	BOOST_MPL_ASSERT    ((moneta::traits::is_last_xml_element_member<MONETA_MEMBER(C, int, k)>));
}

MONETA_DEFINE_ENTITY(
	posix_named_entity,
	((int,                                an_int            ))
	((std::vector<int>,                   int_container       ))
	((boost::optional<std::vector<int> >, optional_container))
)

BOOST_AUTO_TEST_CASE(test_moneta_traits_detail_xml_container_item_name) {
	using moneta::traits::xml_container_item_name;

	BOOST_CHECK_EQUAL(
		(
			xml_container_item_name<
				MONETA_MEMBER(Customer, boost::optional<std::vector<std::string> >, Tags)
			>::get()
		),
		"TagsItem"
	);

	BOOST_CHECK_EQUAL(
		(
			xml_container_item_name<
				MONETA_MEMBER(posix_named_entity, std::vector<int>, int_container)
			>::get()
		),
		"int_container_item"
	);

	BOOST_CHECK_EQUAL(
		(
			xml_container_item_name<
				MONETA_MEMBER(posix_named_entity, boost::optional<std::vector<int> >, optional_container)
			>::get()
		),
		"optional_container_item"
	);
}

// ---

MONETA_DEFINE_ENTITY(
	Foo,
	((int,    ID  ))
	((Person, Dude))
)

BOOST_AUTO_TEST_CASE(test_moneta_traits_detail_xml_item_name_entity) {
	BOOST_CHECK_EQUAL((moneta::traits::get_xml_item_name<Person, boost::mpl::vector0<> >()), "Person");

	BOOST_CHECK_EQUAL((moneta::traits::get_xml_item_name<
		Person,
		boost::mpl::vector1<
			MONETA_MEMBER(Foo, Person, Dude)
		>
	>()), "Dude");
}
