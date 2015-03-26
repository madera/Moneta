#include "stdafx.h"
#include <moneta/algorithm/hash.hxx>
#include <moneta/make_entity.hxx>
#include "../model/Person.hxx"
#include "../model/CatDataset.hxx"

BOOST_AUTO_TEST_CASE(moneta_algorithm_hash_entity) {
	BOOST_CHECK_EQUAL(
		moneta::algorithm::hash(moneta::make_entity<Person>()),
		moneta::algorithm::hash(moneta::make_entity<Person>())
	);

	const std::vector<Cat> dataset = test_dataset<Cat>()();

	const size_t h0 = moneta::algorithm::hash(dataset[0]);
	const size_t h1 = moneta::algorithm::hash(dataset[1]);
	const size_t h2 = moneta::algorithm::hash(dataset[2]);
	const size_t h3 = moneta::algorithm::hash(dataset[3]);

	const size_t _h0 = moneta::algorithm::hash(dataset[0]);
	const size_t _h1 = moneta::algorithm::hash(dataset[1]);
	const size_t _h2 = moneta::algorithm::hash(dataset[2]);
	const size_t _h3 = moneta::algorithm::hash(dataset[3]);

	BOOST_CHECK_EQUAL(h0, _h0);
	BOOST_CHECK_EQUAL(h1, _h1);
	BOOST_CHECK_EQUAL(h2, _h2);
	BOOST_CHECK_EQUAL(h3, _h3);

	Cat c0 = dataset[0];
	Cat c1 = dataset[1];
	Cat c2 = dataset[2];
	Cat c3 = dataset[3];

	BOOST_CHECK_EQUAL(h0, moneta::algorithm::hash(c0));
	BOOST_CHECK_EQUAL(h1, moneta::algorithm::hash(c1));
	BOOST_CHECK_EQUAL(h2, moneta::algorithm::hash(c2));
	BOOST_CHECK_EQUAL(h3, moneta::algorithm::hash(c3));

	c0.Name += 'X';
	BOOST_CHECK_NE(h0, moneta::algorithm::hash(c0));

	c1.Address.ID++;
	BOOST_CHECK_NE(h1, moneta::algorithm::hash(c1));

	c2.ID++;
	BOOST_CHECK_NE(h2, moneta::algorithm::hash(c2));
}

BOOST_AUTO_TEST_CASE(moneta_algorithm_hash_non_entity) {
	BOOST_CHECK_EQUAL(   1, moneta::algorithm::hash(   1));
	BOOST_CHECK_EQUAL(   2, moneta::algorithm::hash(   2));
	BOOST_CHECK_EQUAL(   4, moneta::algorithm::hash(   4));
	BOOST_CHECK_EQUAL(   8, moneta::algorithm::hash(   8));
	BOOST_CHECK_EQUAL(  10, moneta::algorithm::hash(  10));
	BOOST_CHECK_EQUAL( 100, moneta::algorithm::hash( 100));
	BOOST_CHECK_EQUAL(1000, moneta::algorithm::hash(1000));
}
