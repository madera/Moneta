#pragma once
#include "traits/tuple.hxx"
#include "traits/is_entity.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/inherit_linearly.hpp>
#include <boost/optional.hpp>

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

		template <class RootEntityType, class NodeFx>
		struct relational_context_containers : boost::mpl::inherit_linearly<
			typename seek_entity_types<RootEntityType>::type,
			boost::mpl::inherit<
				boost::mpl::_1,
				typename boost::mpl::apply<NodeFx, boost::mpl::_2>::type
			>
		> {};

	}

	template <class RootEntityType>
	class relational_context {

		template <class EntityType>
		struct container {
			typedef boost::optional<
				std::vector<EntityType> //EntityContainerMap<EntityType>
			> type;
		};

		struct container_maker {
			template <class EntityType>
			struct apply : container<EntityType> {};
		};
		
		typedef typename detail::relational_context_containers<
			RootEntityType,
			container_maker
		>::type containers_type;

		template <class EntityType>
		typename container<EntityType>::type& get_container() {
			return _containers;
		}

	private:
		containers_type _containers;
	public:
		template <class EntityType>
		const size_t size() {
			container<EntityType>::type& container = get_container<EntityType>();
			return container? container->size() : 0;
		}
	};

}
