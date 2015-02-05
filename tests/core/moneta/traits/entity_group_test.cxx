#include "stdafx.h"
#include <moneta/traits/entity_group.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Address.hxx"
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"
#include "../model/tree/A.hxx"

using moneta::traits::entity_group;

inline void static_test() {
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<>::top,  boost::mpl::set0<> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<>::type, boost::mpl::set0<> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		entity_group<
			entity_group<
				entity_group<
				//	entity_group<
				//		entity_group<
				//		>
				//	>
				>
			>
		>::top,
		boost::mpl::set0<>
	>));

	//
	// No matter what, the group for entity A must be always { A, B, C, D, E }.
	//

	typedef entity_group<A> a;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<a::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::top, A>::type, A>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<a::top, B>::type, B>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<a::top, C>::type, C>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<a::top, D>::type, D>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<a::top, E>::type, E>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<a::type>::value, ==, 5);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::type, A>::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::type, B>::type, B>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::type, C>::type, C>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::type, D>::type, D>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<a::type, E>::type, E>));

	typedef entity_group<A, A> aa;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<aa::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::top, A>::type, A>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<aa::top, B>::type, B>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<aa::top, C>::type, C>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<aa::top, D>::type, D>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<aa::top, E>::type, E>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<aa::type>::value, ==, 5);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::type, A>::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::type, B>::type, B>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::type, C>::type, C>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::type, D>::type, D>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<aa::type, E>::type, E>));

	//
	// Single entity without entity members should be itself.
	//

	typedef entity_group<Person> p;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<p::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<p::top, Person>::type, Person>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<p::type>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<p::type, Person>::type, Person>));

	typedef entity_group<Person, Person> pp;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<pp::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<pp::top, Person>::type, Person>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<pp::type>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<pp::type, Person>::type, Person>));

	//
	// Single entity with entity members should catch all the child entities.
	//

	typedef entity_group<Cat> c;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<c::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<c::top, Cat>::type, Cat>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<c::type>::value, ==, 2);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<c::type, Cat    >::type, Cat    >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<c::type, Address>::type, Address>));

	typedef entity_group<Cat, Cat> cc;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<cc::top>::value, ==, 1);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<cc::top, Cat>::type, Cat>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<cc::type>::value, ==, 2);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<cc::type, Cat    >::type, Cat    >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<cc::type, Address>::type, Address>));

	//
	// Mixing entity_group containers
	//

	typedef entity_group<p, c> person_cat_group;
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<person_cat_group::top>::value, ==, 2);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<person_cat_group::top, Cat   >::type, Cat   >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<person_cat_group::top, Person>::type, Person>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<person_cat_group::type>::value, ==, 3); // Person, Cat and Address.
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<person_cat_group::type, Person >::type, Person >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<person_cat_group::type, Cat    >::type, Cat    >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<person_cat_group::type, Address>::type, Address>));

	//
	// More mixes
	//
	typedef entity_group<
		A,
		entity_group<
			Person,
			Address
		>
	> mix;
	
	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<mix::top>::value, ==, 3);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::top, A>::type, A>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<mix::top, B>::type, B>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<mix::top, C>::type, C>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<mix::top, D>::type, D>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<mix::top, E>::type, E>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::top, Person >::type, Person >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::top, Address>::type, Address>));

	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<mix::type>::value, ==, 7);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, A>::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, B>::type, B>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, C>::type, C>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, D>::type, D>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, E>::type, E>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, Person >::type, Person >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<mix::type, Address>::type, Address>));

	//
	// Even more mixes
	//

	typedef entity_group<
		A,
		entity_group<
			Person,
			Address/*,
			entity_group<
				Cat
			>*/
		>
	> level2;
	
//	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<level2::top>::value, ==, 4);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::top, A>::type, A>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<level2::top, B>::type, B>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<level2::top, C>::type, C>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<level2::top, D>::type, D>));
	BOOST_MPL_ASSERT_NOT((boost::is_same<boost::mpl::at<level2::top, E>::type, E>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::top, Person >::type, Person >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::top, Address>::type, Address>));
//	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::top, Cat    >::type, Cat    >));

//	BOOST_MPL_ASSERT_RELATION(boost::mpl::size<level2::type>::value, ==, 8);
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, A>::type, A>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, B>::type, B>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, C>::type, C>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, D>::type, D>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, E>::type, E>));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, Person >::type, Person >));
	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, Address>::type, Address>));
//	BOOST_MPL_ASSERT((boost::is_same<boost::mpl::at<level2::type, Cat    >::type, Cat    >));
}
