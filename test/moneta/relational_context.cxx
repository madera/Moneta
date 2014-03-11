#include "stdafx.h"
#include <moneta/relational_context.hxx>
#include <boost/mpl/equal.hpp>
#include "model/Cat.hxx"
#include "model/Dog.hxx"
#include "model/Composite.hxx"

#include <boost/mpl/print.hpp>

static void static_tests() {
	BOOST_MPL_ASSERT((
		boost::is_same<
			moneta::relational_context<Cat>::type,
			moneta::relational_context<Cat>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::relational_context<Cat>::types,
			boost::mpl::vector<Cat, Address>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			moneta::relational_context<Composite>::types,
			boost::mpl::vector<Composite, Person, Dog>
		>
	));
}

BOOST_AUTO_TEST_CASE(relational_context_test) {

	moneta::relational_context<Cat> context;


}
