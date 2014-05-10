#include "stdafx.h"
#include <moneta/container/rcontext.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/foreach.hpp>
#include "../model/Cat.hxx"
#include "../model/CatDataset.hxx"
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

	std::vector<Cat> dataset = test_dataset<Cat>()();
	BOOST_FOREACH(Cat cat, dataset) {
		context.insert(cat);
	}

	BOOST_CHECK_EQUAL(context.size<Cat>(), 4);
	BOOST_CHECK_EQUAL(context.size<Address>(), 4);

	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Cat>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Address>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;

	Cat conando = dataset[0];
	conando.ID = 4;
	conando.Name = "Conando";
	conando.Address.ID = 10;
	conando.Address.Number = 55;
	conando.Address.Street = "Conan Av.";
	
	context.replace(conando);

	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Cat>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Address>()->debug_dump();
	std::cerr << "---------------------------------" << std::endl;

}
