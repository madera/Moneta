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
#include <moneta/container/rcontext.hxx>
#include <boost/mpl/equal.hpp>
#include <boost/foreach.hpp>
#include "../model/Cat.hxx"
#include "../model/CatDataset.hxx"
#include "../model/Dog.hxx"
#include "../model/Composite.hxx"
#include "../model/tree/A.hxx"

inline void static_test() {
	// TODO: Move this to a separate test file.
	// TODO: Move seek_entity_types to a more general purpose traits file.
	//
	// details::seek_entity_types<>
	//
	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::detail::seek_entity_types<Person>::type,
			boost::mpl::vector<Person>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::detail::seek_entity_types<Cat>::type,
			boost::mpl::vector<Cat, Address>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::detail::seek_entity_types<A>::type,
			boost::mpl::vector5<A, B, C, D, E>
		>
	));

	// ------------------------------------------------------------------------------------------------------------

	//
	// rcontext tests...
	//

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::rcontext<Person>::optional_rset<Person>::type,
			moneta::container::default_meta_set<Person>::type
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::container::rcontext<Person>::optional_rset<Cat>::type,
			moneta::container::default_meta_set<Cat>::type
		>
	));

	//BOOST_MPL_ASSERT((
	//	boost::is_same<
	//		moneta::container::rcontext<Person>::make_optional_set::apply<Person>::type,
	//		boost::optional<
	//			moneta::container::detail::meta_set_impl<
	//				boost::mpl::vector1<
	//					moneta::container::pk_tracker<Person>
	//				>
	//			>
	//		>
	//	>
	//));

	//BOOST_MPL_ASSERT((
	//	boost::is_same<
	//		moneta::container::rcontext<Cat>::optional_rset<Cat>::type,
	//		boost::optional<
	//			moneta::container::detail::meta_set_impl<
	//				boost::mpl::vector1<
	//					moneta::container::pk_tracker<Cat>
	//				>
	//			>
	//		>
	//	>
	//));

	// ---

	//typedef moneta::container::rcontext<Cat> context_type;

	//BOOST_MPL_ASSERT((
	//	boost::is_same<
	//		context_type::container_for<Person>::type,
	//		boost::optional<
	//			moneta::container::detail::meta_set_impl<
	//				boost::mpl::vector1<moneta::container::pk_tracker<Person> >
	//			>
	//		>
	//	>
	//));

	//BOOST_MPL_ASSERT((
	//	boost::is_same<
	//		context_type::container_for<Cat>::type,
	//		boost::optional<
	//			moneta::container::detail::meta_set_impl<
	//				boost::mpl::vector1<moneta::container::pk_tracker<Cat> >
	//			>
	//		>
	//	>
	//));
}

BOOST_AUTO_TEST_CASE(rcontext_containers_test) {

	typedef moneta::container::rcontext<Cat> context_type;

	typedef context_type::container_for<Cat    >::type  first_type;
	typedef context_type::container_for<Address>::type second_type;

	BOOST_MPL_ASSERT((boost::is_same<
		context_type::containers_type,
		boost::mpl::inherit2<first_type, second_type>
	>));

	std::cerr << "############################################" << std::endl;
	std::cerr << "############################################" << std::endl;

	context_type::containers_type containers;

	 first_type&  first = containers;
	second_type& second = containers;

	BOOST_CHECK(! first.is_initialized());
	BOOST_CHECK(!second.is_initialized());

	 first.reset( first_type::value_type());
	second.reset(second_type::value_type());

	BOOST_CHECK_EQUAL( first->size(), 0);
	BOOST_CHECK_EQUAL(second->size(), 0);

	Cat garfield = moneta::make_entity<Cat>();
	first->insert(garfield);

	BOOST_CHECK_EQUAL( first->size(), 1);
	BOOST_CHECK_EQUAL(second->size(), 0);

	Address address = moneta::make_entity<Address>();
	second->insert(address);

	BOOST_CHECK_EQUAL( first->size(), 1);
	BOOST_CHECK_EQUAL(second->size(), 1);

	std::cerr << "############################################" << std::endl;
	std::cerr << "############################################" << std::endl;
}

BOOST_AUTO_TEST_CASE(rcontext_test) {

	moneta::container::rcontext<Cat> context;

	BOOST_CHECK_EQUAL(context.size<Cat>(), 0);
	BOOST_CHECK_EQUAL(context.size<Address>(), 0);

	Address address = moneta::make_entity<Address>();
	context.insert(address);
	BOOST_CHECK_EQUAL(context.size<Cat>(), 0);
	BOOST_CHECK_EQUAL(context.size<Address>(), 1);

	Cat cat = moneta::make_entity<Cat>();
	cat.Address.ID = 9000;
	context.insert(cat);
	BOOST_CHECK_EQUAL(context.size<Cat>(), 1);
	BOOST_CHECK_EQUAL(context.size<Address>(), 1 + 1);

	std::vector<Cat> dataset = test_dataset<Cat>()();
	BOOST_FOREACH(Cat cat, dataset) {
		context.insert(cat);
	}

	BOOST_CHECK_EQUAL(context.size<Cat>(), 1 + dataset.size());
	BOOST_CHECK_EQUAL(context.size<Address>(), 1 + 1 + dataset.size());

	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Cat>()->debug_dump(std::cerr);
	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Address>()->debug_dump(std::cerr);
	std::cerr << "---------------------------------" << std::endl;
}

BOOST_AUTO_TEST_CASE(rcontext_test_2) {

	moneta::container::rcontext<Cat> context;

	BOOST_CHECK_EQUAL(context.size<Cat    >(), 0);
	BOOST_CHECK_EQUAL(context.size<Address>(), 0);

	std::vector<Cat> dataset = test_dataset<Cat>()();
	BOOST_FOREACH(Cat cat, dataset) {
		context.insert(cat);
	}

	BOOST_CHECK_EQUAL(context.size<Cat    >(), dataset.size());
	BOOST_CHECK_EQUAL(context.size<Address>(), dataset.size());

	Cat conando = dataset[0];
	conando.ID = 4;
	conando.Name = "Conando";
	conando.Address.ID = 10;
	conando.Address.Number = 55;
	conando.Address.Street = "Conan Av.";
	
	context.replace(conando);

	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Cat>()->debug_dump(std::cerr);
	std::cerr << "---------------------------------" << std::endl;
	context.get_container<Address>()->debug_dump(std::cerr);
	std::cerr << "---------------------------------" << std::endl;
}

BOOST_AUTO_TEST_CASE(rcontext_test_3) {
	typedef moneta::container::rcontext<Person> context_type;
	context_type context;

	BOOST_CHECK_EQUAL(context.size<Person>(), 0);
}
