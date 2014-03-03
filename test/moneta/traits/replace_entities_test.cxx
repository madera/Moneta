#include "stdafx.h"
#include <moneta/traits/replace_entities_with.hxx>
#include <moneta/traits/pk_or_type_filter.hxx>
#include <boost/mpl/equal.hpp>
#include "../model/Cat.hxx"
#include "../model/Address.hxx"
#include "../model/Composite.hxx"

#include <boost/mpl/print.hpp>
#include <boost/mpl/quote.hpp>

BOOST_AUTO_TEST_CASE(replace_entities_test) {

	BOOST_MPL_ASSERT((
		boost::mpl::equal<
			boost::mpl::transform<
				typename moneta::traits::detail::mpl::vector<Cat>::type,
				moneta::traits::detail::pk_or_type<boost::mpl::_1>
			>::type,
			boost::mpl::vector3<int, std::string, int>
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
