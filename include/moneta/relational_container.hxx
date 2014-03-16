#pragma once
#include "make_entity.hxx"
#include "sql/traits/db_tuple.hxx"
#include "sql/traits/to_db_tuple.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

namespace moneta {

	enum entity_states {
		MONETA_ENTITY_STATE_NEW = 1
	};

	namespace detail {
	}

	namespace mi = boost::multi_index;

	template <
		class EntityType,
		class ComparatorType = std::less<EntityType>,
		class ChangeDetector = void
	>
	class relational_container : boost::noncopyable {

		typedef typename sql::traits::db_tuple<EntityType>::type tuple_type;
		typedef typename sql::traits::db_tuple<EntityType>::type const_tuple_type;

		template <class EntityType>
		struct entry {
			int state;
			tuple_type tuple;

			entry()
			 : state(0), tuple(make_tuple<EntityType>()) {};

			entry(EntityType& entity)
			 : state(0), tuple(sql::traits::to_db_tuple<EntityType>(entity)) {};

			const bool operator==(const entry& rhs) const {
				return tuple == rhs.tuple;
			}

			//entry(const EntityType& entity)
			//	: state(0) {
			//	const_tuple_type ct =
			//		sql::traits::to_db_tuple<EntityType>(entity);
			//};
		};

		typedef entry<EntityType> entry_type;

	private:
		struct by_sequence;

		typedef mi::multi_index_container<
			entry_type,
			mi::indexed_by<
				mi::sequenced<mi::tag<by_sequence> >/*,
				mi::ordered_non_unique<
					mi::member<entry, int, &entry::state>
				>*/
			>
		> container_type;

	private:
		boost::optional<entry_type>
		get_entry(/*const */EntityType& entity) const {
			typedef typename container_type::template index<by_sequence>::type by_sequence_index;
			by_sequence_index::const_iterator begin = _container.get<by_sequence>().begin();
			by_sequence_index::const_iterator   end = _container.get<by_sequence>().end();
			by_sequence_index::const_iterator   itr = std::find(begin, end, sql::traits::to_db_tuple(entity));

			if (itr == end) {
				return boost::optional<entry_type>();
			}

			return *itr;
		}

	private:
		container_type _container;

	public:
		const size_t size() {
			return _container.get<0>().size();
		}

		void insert(/*const */EntityType& entity) {
			entry_type newcomer(entity);
			newcomer.state = MONETA_ENTITY_STATE_NEW;
			_container.get<0>().push_back(newcomer);
		}

		const bool is_new(/*const */EntityType& entity) {
			boost::optional<entry_type> entry =  get_entry(entity);
			return entry? entry->state & MONETA_ENTITY_STATE_NEW : false;
		}

		const bool is_bound(/*const */EntityType& entity) {
			return get_entry(entity).is_initialized();
		}
	};

}
