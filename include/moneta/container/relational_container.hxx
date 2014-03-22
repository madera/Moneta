#pragma once
#include "change_trackers/hash_change_tracker.hxx"
#include "change_trackers/null_change_tracker.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "load_trackers/null_load_tracker.hxx"
#include "../make_entity.hxx"
#include "../traits/to_entity.hxx"
#include "../sql/traits/db_pk_tuple.hxx"
#include "../sql/traits/db_tuple.hxx"
#include "../sql/traits/to_db_tuple.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <boost/mpl/print.hpp>

namespace moneta { namespace container {

	namespace mi = boost::multi_index;



	template <
		class EntityType,
		class LoadTracker = null_load_tracker<EntityType>, //bitset_load_tracker<EntityType>,
		class ChangeTracker = null_change_tracker<EntityType> //hash_change_tracker<EntityType>
	>
	class relational_container : boost::noncopyable {

		// We don't support entities with no PKs (yet).
		BOOST_MPL_ASSERT((moneta::traits::has_pk<EntityType>));

		typedef typename sql::traits::db_pk_tuple<
			EntityType
		>::type db_pk_tuple_type;
		
		typedef typename boost::call_traits<
			db_pk_tuple_type
		>::param_type db_pk_tuple_param_type;

		typedef typename sql::traits::db_tuple<
			typename boost::remove_const<EntityType>::type
		>::type db_tuple_type;



		struct entry : LoadTracker, ChangeTracker {

			int flags;
			db_pk_tuple_type pk;
			db_tuple_type tuple;

			explicit entry(const EntityType& entity)
			 : LoadTracker(entity), ChangeTracker(entity),
			   flags(0),
			   pk(traits::pk_tie<const EntityType>()(entity)),
			   tuple(sql::traits::to_db_tuple(entity)) {};

			const bool operator==(const db_pk_tuple_type& rhs) const {
				return pk == rhs;
			}

		};

	private:
		struct by_sequence;
		struct by_hash;
		struct by_flags;

		typedef mi::multi_index_container<
			entry,
			mi::indexed_by<
				mi::sequenced<
					mi::tag<by_sequence>
				>,
				mi::hashed_unique<
					mi::tag<by_hash>,
					mi::member<entry, db_pk_tuple_type, &entry::pk>
				>,
				mi::ordered_non_unique<
					mi::tag<by_flags>,
					mi::member<entry, int, &entry::flags>
				>
			>
		> container_type;

	private:
		boost::optional<entry> get_entry(db_pk_tuple_param_type entity) const {
			typedef typename container_type::template index<by_hash>::type index;
			index::const_iterator begin = _container.get<by_hash>().begin();
			index::const_iterator   end = _container.get<by_hash>().end();
			index::const_iterator   itr = std::find(begin, end, entity);

			if (itr == end) {
				return boost::optional<entry>();
			}

			return *itr;
		}

	private:
		container_type _container;

	public:
		const size_t size() {
			return _container.get<0>().size();
		}

		void insert(const EntityType& entity) {
			_container.get<by_hash>().insert(entry(entity));
		}

		//boost::optional<EntityType> retrieve(db_pk_tuple_param_type pk) {
		//	boost::optional<entry> entry = get_entry(pk);
		//	if (!entry.is_initialized()) {
		//		return boost::optional<EntityType>();
		//	}
		//
		//	return moneta::traits::to_entity<EntityType>(entry->tuple);
		//}

		// --------------------------------------------------------------------------------

		const bool bound(const EntityType& entity) const {
			return bound(traits::pk_tie<const EntityType>()(entity));
		}

		const bool bound(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return entry.is_initialized();
		}

		// --------------------------------------------------------------------------------

		const bool dirty(const EntityType& entity) const {
			return dirty(traits::pk_tie<const EntityType>()(entity));
		}

		const bool dirty(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->dirty() : false;
		}

		// --------------------------------------------------------------------------------

		const bool is_new(const EntityType& entity) const {
			return is_new(traits::pk_tie<const EntityType>()(entity));
		}

		const bool is_new(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->flags & 1 : false;
		}
	};

}}
