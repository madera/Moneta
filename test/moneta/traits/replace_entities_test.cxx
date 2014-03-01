#include "stdafx.h"
#include <moneta/traits/replace_entities_with.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Cat.hxx"
#include "../model/Composite.hxx"

#include <boost/mpl/print.hpp>

BOOST_AUTO_TEST_CASE(replace_entities_test) {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			typename moneta::traits::replace_entities<
				typename moneta::traits::detail::mpl::vector<Cat>::type,
				moneta::traits::detail::replace_entity_with_pk_tie
			>::type,
			boost::mpl::vector3<int&, std::string&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			typename moneta::traits::replace_entities_with_pks<
				typename moneta::traits::detail::mpl::vector<Cat>::type
			>::type,
			boost::mpl::vector3<int, std::string, int>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			typename moneta::traits::replace_entities_with_pk_ties<
				typename moneta::traits::detail::mpl::vector<Cat>::type
			>::type,
			boost::mpl::vector3<int&, std::string&, int&>
		>
	));

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			typename moneta::traits::replace_entities_with_pk_ties<
				typename moneta::traits::detail::mpl::vector<Composite>::type
			>::type,
			boost::mpl::vector3<int&, int&, boost::fusion::vector2<std::string&, int&> >
		>
	));
}
