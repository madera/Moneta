// XXX: Move this file to another namespace.
#pragma once
#include "../../traits/tuple.hxx"
#include "../../traits/is_entity.hxx"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container { namespace detail {

	template <class Entity, class State = boost::mpl::vector<> >
	struct seek_entity_types : boost::mpl::fold<
		typename traits::tuple<Entity>::type,
		typename boost::mpl::push_back<State, Entity>::type,
		boost::mpl::if_<
			traits::is_entity<boost::mpl::_2>,
			moneta::container::detail::seek_entity_types<boost::mpl::_2, boost::mpl::_1>,
			boost::mpl::_1
		>
	> {};

}}}
