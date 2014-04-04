#pragma once
#include "../../traits/tuple.hxx"
#include "../../traits/is_entity.hxx"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container { namespace detail {

	template <class EntityType, class State = boost::mpl::vector<> >
	struct seek_entity_types : boost::mpl::fold<
		typename traits::tuple<EntityType>::type,
		typename boost::mpl::push_back<State, EntityType>::type,
		boost::mpl::if_<
			traits::is_entity<boost::mpl::_2>,
			typename seek_entity_types<boost::mpl::_2, boost::mpl::_1>,
			boost::mpl::_1
		>
	> {};

	template <class RootEntityType, class NodeFx>
	struct rcontext_containers : boost::mpl::inherit_linearly<
		typename seek_entity_types<RootEntityType>::type,
		boost::mpl::inherit<
			boost::mpl::_1,
			typename boost::mpl::apply<NodeFx, boost::mpl::_2>::type
		>
	> {};

}}}
