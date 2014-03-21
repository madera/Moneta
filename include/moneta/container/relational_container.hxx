#pragma once
#include "change_trackers/hash_change_tracker.hxx"
#include "change_trackers/null_change_tracker.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "load_trackers/null_load_tracker.hxx"
#include "../make_entity.hxx"
#include "../sql/traits/db_tuple.hxx"
#include "../sql/traits/to_db_tuple.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container {

	namespace mi = boost::multi_index;



	template <
		class EntityType,
		class LoadTracker = null_load_tracker<EntityType>, //bitset_load_tracker<EntityType>,
		class ChangeTracker = null_change_tracker<EntityType> //hash_change_tracker<EntityType>
	>
	class relational_container : boost::noncopyable {

		struct entry : LoadTracker, ChangeTracker {
			typedef typename sql::traits::db_tuple<
				typename boost::remove_const<EntityType>::type
			>::type tuple_type;

			int flags;
			tuple_type data;

			entry()
			 : LoadTracker(), ChangeTracker(),
			   flags(0), data(sql::traits::make_db_tuple<EntityType>()) {};

			explicit entry(const EntityType& entity)
			 : LoadTracker(entity), ChangeTracker(entity),
			   flags(0), data(sql::traits::to_db_tuple(entity)) {};

			// XXX: TBD
			const bool operator==(const EntityType& entity) const {
				auto tie = moneta::sql::traits::to_db_tie(entity);
				return data == tie;
			}

			// XXX: TBD
			const bool operator==(const entry& rhs) const {
				return data == rhs.data;
			}

			////
			//// Flags
			////
			//const flags_t flags() const {
			//	return flags;
			//}

			//const void flags(boost::call_traits<flags_t>::param_type value) const {
			//	flags = value;
			//}
		};

	private:
		struct by_sequence;
		struct by_flags;

		typedef mi::multi_index_container<
			entry,
			mi::indexed_by<
				mi::sequenced<mi::tag<by_sequence> >,
				mi::ordered_non_unique<
					mi::tag<by_flags>,
					mi::member<entry, int, &entry::flags>
				>
			>
		> container_type;

	private:
		boost::optional<entry>
		get_entry(const EntityType& entity) const {
			typedef typename container_type::template index<by_sequence>::type by_sequence_index;
			by_sequence_index::const_iterator begin = _container.get<by_sequence>().begin();
			by_sequence_index::const_iterator   end = _container.get<by_sequence>().end();
			by_sequence_index::const_iterator   itr = std::find(begin, end, entity);

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
			_container.get<0>().push_back(entry(entity));
		}

		const bool bound(const EntityType& entity) const {
			return get_entry(entity).is_initialized();
		}

		const bool dirty(const EntityType& entity) const {
			auto pk = traits::pk_tie(entity);
			return true;
		}

		//const bool is_new(/*const */EntityType& entity) {
		//	boost::optional<entry_type> entry =  get_entry(entity);
		//	return entry? entry->flags & MONETA_ENTITY_STATE_NEW : false;
		//}

		//const bool is_bound(/*const */EntityType& entity) {
		//	return get_entry(entity).is_initialized();
		//}
	};

}}
