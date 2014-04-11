#include "stdafx.h"
#include <moneta/traits/rtuple.hxx>
#include <moneta/traits/to_rtuple.hxx>
#include <moneta/traits/detail/sub_tie_vector.hxx>
#include <moneta/traits/detail/deref_if_unary.hxx>
#include <moneta/traits/detail/indices_of.hxx>
#include <boost/mpl/vector_c.hpp>
#include "../model/Cat.hxx"
#include "../model/Dog.hxx"

template <class EntityType>
struct to_pk_rtuple_impl {
	typedef typename moneta::traits::rtuple<EntityType>::type rtuple_type;

	typedef typename moneta::traits::detail::indices_of<
		typename moneta::traits::members<EntityType>::type,
		moneta::traits::detail::is_pk<boost::mpl::_1>
	>::type indices_type;

	typedef moneta::traits::detail::sub_tie_vector<
		rtuple_type,
		indices_type
	> sub_tie_type;

	typedef typename moneta::traits::detail::deref_if_unary<
		typename sub_tie_type::type
	>::type type;

	type operator()(rtuple_type& rtuple) const {
		return moneta::traits::detail::unary_deref(sub_tie_type()(rtuple));
	}
};

template <class EntityType>
typename to_pk_rtuple_impl<EntityType>::type
to_pk_rtuple(typename moneta::traits::rtuple<EntityType>::type& rtuple) {
	return to_pk_rtuple_impl<EntityType>()(rtuple);
}

BOOST_AUTO_TEST_CASE(to_pk_rtuple_test) {
	Cat felix;
	felix.ID = 1;
	felix.Name = "Felix the cat";
	felix.Address.ID = 555;

	typedef moneta::traits::rtuple<Cat>::type rtuple_t;
	rtuple_t rtuple = moneta::traits::to_rtuple<Cat>(felix);

	int pk = to_pk_rtuple<Cat>(rtuple);
}

BOOST_AUTO_TEST_CASE(to_pk_rtuple_test_2) {
	Dog snoopy;
	snoopy.Owner = "Charlie";
	snoopy.ID = 1;
	snoopy.Name = "Snoopy";

	typedef moneta::traits::rtuple<Dog>::type rtuple_t;
	rtuple_t rtuple = moneta::traits::to_rtuple(snoopy);

	auto pk = to_pk_rtuple<Dog>(rtuple);
}
