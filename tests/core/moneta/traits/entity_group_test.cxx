#include "stdafx.h"
#include <moneta/traits/entity_group.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Person.hxx"
#include "../model/Address.hxx"
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"
#include "../model/tree/A.hxx"
#include "../model/Composite.hxx"

inline void static_test() {
	using moneta::traits::entity_group;

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<>::top, boost::mpl::vector0<> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<>::all, boost::mpl::vector0<> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		entity_group<
			entity_group<
				entity_group<
					entity_group<
						entity_group<
						>
					>
				>
			>
		>::top,
		boost::mpl::vector0<>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<A>::top, boost::mpl::vector<A> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<A>::all, boost::mpl::vector<A, B, C, D, E> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<A, A>::top, boost::mpl::vector<A> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<A, A>::all, boost::mpl::vector<A, B, C, D, E> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Person>::top, boost::mpl::vector<Person> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Person>::all, boost::mpl::vector<Person> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Person, Person>::top, boost::mpl::vector<Person> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Person, Person>::all, boost::mpl::vector<Person> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Cat>::top, boost::mpl::vector<Cat> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Cat>::all, boost::mpl::vector<Cat, Address> >));

	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Cat, Cat>::top, boost::mpl::vector<Cat> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<entity_group<Cat, Cat>::all, boost::mpl::vector<Cat, Address> >));

	typedef entity_group<
		entity_group<Person>,
		entity_group<Cat>
	> person_cat_group;

	BOOST_MPL_ASSERT((boost::mpl::equal<person_cat_group::top, boost::mpl::vector<Person, Cat> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<person_cat_group::all, boost::mpl::vector<Person, Cat, Address> >));

	typedef entity_group<
		A,
		entity_group<
			Person,
			Cat
		>
	> a_person_address_group;

	BOOST_MPL_ASSERT((boost::mpl::equal<a_person_address_group::top, boost::mpl::vector<A, Person, Cat> >));
	BOOST_MPL_ASSERT((boost::mpl::equal<
		a_person_address_group::all,
		boost::mpl::vector<A, B, C, D, E, Person, Cat, Address>
	>));
	
	typedef entity_group<
		A,
		entity_group<
			Person,
			Address,
			Cat,
			entity_group<
				Cat,
				Cat,
				Person,
				Address
			>
		>
	> a_person_address_cat_group;
	
	BOOST_MPL_ASSERT((boost::mpl::equal<
		a_person_address_cat_group::top,
		boost::mpl::vector<A, Person, Address, Cat>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		a_person_address_cat_group::all,
		boost::mpl::vector<A, B, C, D, E, Person, Address, Cat>
	>));

	typedef entity_group<
		A,
		entity_group<
			Person,
			Address,
			entity_group<
				Cat,
				entity_group<
					Person,
					Address,
					entity_group<
						entity_group<
							entity_group<E>,
							entity_group<Composite>
						>
					>
				>
			>
		>
	> over9000;

	BOOST_MPL_ASSERT((boost::mpl::equal<
		over9000::top,
		boost::mpl::vector<A, Person, Address, Cat, E, Composite>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		over9000::all,
		boost::mpl::vector<A, B, C, D, E, Person, Address, Cat, Composite, Dog>
	>));
}
