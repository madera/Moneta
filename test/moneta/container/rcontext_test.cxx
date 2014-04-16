#include "stdafx.h"
#include <moneta/container/rcontext.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"

#include <boost/mpl/print.hpp>

static void static_tests() {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::detail::seek_entity_types<Cat>::type,
			boost::mpl::vector<Cat, Address>
		>
	));

	//BOOST_MPL_ASSERT((
	//	boost::mpl::equal<
	//		moneta::container::rcontext<Cat>::containers_type,
	//		boost::mpl::vector<Cat, Address>
	//	>
	//));

	//BOOST_MPL_ASSERT((
	//	boost::mpl::equal<
	//		moneta::rcontext<Composite>::containers_type,
	//		boost::mpl::vector<Composite, Person, Dog>
	//	>
	//));

}

BOOST_AUTO_TEST_CASE(rcontext_test) {
	moneta::container::rcontext<Cat> context;

	BOOST_CHECK_EQUAL(context.size<Cat>(), 0);
	BOOST_CHECK_EQUAL(context.size<Address>(), 0);

	{
		Cat cat;
		cat.ID = 1;
		cat.Name = "Somecat";
		cat.Address.ID = 10;
		cat.Address.Number = 10;
		cat.Address.Street = "Ten St.";

		context.insert(cat);
	}

	{
		Cat cat;
		cat.ID = 2;
		cat.Name = "Garfield";
		cat.Address.ID = 20;
		cat.Address.Number = 20;
		cat.Address.Street = "Twenty St.";

		context.insert(cat);
	}

	{
		Cat cat;
		cat.ID = 3;
		cat.Name = "Felix";
		cat.Address.ID = 30;
		cat.Address.Number = 30;
		cat.Address.Street = "Thirty St.";

		context.insert(cat);
	}

	{
		Cat cat;
		cat.ID = 4;
		cat.Name = "Bob";
		cat.Address.ID = 40;
		cat.Address.Number = 40;
		cat.Address.Street = "Forty St.";

		context.insert(cat);
	}

	BOOST_CHECK_EQUAL(context.size<Cat>(), 4);
	BOOST_CHECK_EQUAL(context.size<Address>(), 4);

	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Cat>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Address>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;

}
