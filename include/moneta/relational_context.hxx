#pragma once
#include "traits/tuple.hxx"
#include "traits/is_entity.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
namespace moneta {

	namespace detail {

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

	}

	template <class RootEntityType>
	struct relational_context {
		typedef relational_context type;
		typedef typename detail::seek_entity_types<RootEntityType>::type types;
	};

}
