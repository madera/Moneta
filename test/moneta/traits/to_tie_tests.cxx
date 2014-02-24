#include "stdafx.h"
#include <moneta/traits/to_tie.hxx>
#include "../model/Person.hxx"

BOOST_AUTO_TEST_CASE(to_tie_test) {
	using boost::is_same;
	using boost::mpl::at_c;

	Person person;
	//moneta::traits::tie<Person>::type tie = moneta::traits::to_tie(person);
}
