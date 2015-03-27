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
#include <moneta/traits/pk.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Dog.hxx"
#include "../model/Cat.hxx"

struct Dummy {
};

inline void static_test() {
	using boost::is_same;
	using boost::mpl::equal;
	using moneta::traits::pk;
	using moneta::traits::has_pk;
	using moneta::traits::detail::is_pk;

	namespace fusion = boost::fusion;
	namespace mpl = boost::mpl;

	BOOST_MPL_ASSERT((equal<pk<Person>::type, int>));
	BOOST_MPL_ASSERT((equal<pk<const Person>::type, const int>));

	BOOST_MPL_ASSERT((equal<pk<Dog      >::type, fusion::vector<std::string,       int      > >));
	BOOST_MPL_ASSERT((equal<pk<const Dog>::type, fusion::vector<const std::string, const int> >));

	// Non Entities (and entities without a pk) must be their own pk.
	BOOST_MPL_ASSERT((is_same<pk<int        >::type, int        >));
	BOOST_MPL_ASSERT((is_same<pk<short      >::type, short      >));
	BOOST_MPL_ASSERT((is_same<pk<std::string>::type, std::string>));
	BOOST_MPL_ASSERT((is_same<pk<Dummy      >::type, Dummy      >));

	// Person
	BOOST_MPL_ASSERT((is_pk<MONETA_MEMBER(Person, int, ID)>));

	BOOST_MPL_ASSERT_NOT((is_pk<MONETA_MEMBER(Person, std::string, Name   )>));
	BOOST_MPL_ASSERT_NOT((is_pk<MONETA_MEMBER(Person, double,      Height )>));
	BOOST_MPL_ASSERT_NOT((is_pk<MONETA_MEMBER(Person, int,         Ratings)>));

	// Dog
	BOOST_MPL_ASSERT((is_pk<MONETA_MEMBER(Dog, std::string, Owner)>));
	BOOST_MPL_ASSERT((is_pk<MONETA_MEMBER(Dog, int,         ID   )>));

	BOOST_MPL_ASSERT_NOT((is_pk<MONETA_MEMBER(Person, std::string, Name)>));

	//
	// has_pk
	//
	BOOST_MPL_ASSERT((has_pk<Person>));
	BOOST_MPL_ASSERT((has_pk<Dog>));
	BOOST_MPL_ASSERT_NOT((has_pk<Dummy>));

	//
	// detail_mpl_pk
	//

	// Person
	BOOST_MPL_ASSERT((
		equal<moneta::traits::detail::mpl::pk<Person>::type, boost::mpl::vector<int> >
	));

	// Dog
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::mpl::pk<Dog>::type,
			boost::mpl::vector<std::string, int>
		>
	));

	//
	// detail_fusion_pk
	//

	// Person
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::fusion::pk<Person>::type,
			boost::fusion::vector<int>
		>
	));

	// Dog
	BOOST_MPL_ASSERT((
		equal<
			moneta::traits::detail::fusion::pk<Dog>::type,
			boost::fusion::vector<std::string, int>
		>
	));
}

// TODO: Check this comment below.
//
// Commented since extract_pk could be embedded into pk() as a simplifier.
// It's future is TBD.
//BOOST_AUTO_TEST_CASE(pk_extraction_test) {
//	BOOST_MPL_ASSERT((boost::is_same<moneta::traits::pk<Person>::type, int>));
//
//	Person person;
//	person.ID = 1;
//
//	moneta::traits::pk<Person> pk;
//	BOOST_CHECK_EQUAL(moneta::traits::extract_pk(person), 1);
//}
