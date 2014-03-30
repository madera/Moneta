#pragma once
#include "change_trackers/hash_change_tracker.hxx"
#include "change_trackers/null_change_tracker.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "load_trackers/null_load_tracker.hxx"
#include "../make_entity.hxx"
#include "../traits/to_entity.hxx"
#include "../sql/traits/db_pk_tuple.hxx"
#include "../sql/traits/rtuple.hxx"
#include "../sql/traits/to_db_tuple.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>

#include <iomanip>
#include <boost/tuple/tuple_io.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/format.hpp>
#include <boost/mpl/print.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>

namespace moneta { namespace container {

	namespace mi = boost::multi_index;



	template <
		class EntityType,
		class LoadTracker = /*null_load_tracker<EntityType>,*/ bitset_load_tracker<EntityType>,
		class ChangeTracker = /*null_change_tracker<EntityType>*/ hash_change_tracker<EntityType>
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

		typedef typename sql::traits::rtuple<
			typename boost::remove_const<EntityType>::type
		>::type db_tuple_type;



		struct entry : LoadTracker, ChangeTracker {

			int flags;
			db_pk_tuple_type pk;
			db_tuple_type data;

			explicit entry(
				const EntityType& entity,
				const bool newcomer = false,
				const bool all_loaded = true
			)
			 : LoadTracker(all_loaded),
			   ChangeTracker(sql::traits::to_db_tuple(entity)),
			   flags(0),
			   pk(traits::pk_tie<const EntityType>()(entity)),
			   data(sql::traits::to_db_tuple(entity)) {
				if (newcomer) {
					flags |= 1;
				}
			};

			const bool operator==(const db_pk_tuple_type& rhs) const {
				return pk == rhs;
			}

			// -------------------------------------------------

			const bool newcomer() const {
				return flags & 1;
			}

			void newcomer(const bool value) {
				flags |= value;
			}

			// -------------------------------------------------

			const bool removed() const {
				return flags & 2;
			}

			void remove(const bool value = true) {
				flags |= (value << 2);
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
			return (itr == end)? boost::optional<entry>() : *itr;
		}

	private:
		container_type _container;

	public:
		const size_t size() {
			return _container.get<0>().size();
		}

		void insert(const EntityType& entity, const bool newcomer = false) {
			_container.get<by_hash>().insert(entry(entity, newcomer));
		}

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
			return (entry.is_initialized())? entry->dirty(entry->data) : false;
		}

		// --------------------------------------------------------------------------------

		const bool newcomer(const EntityType& entity) const {
			return newcomer(traits::pk_tie<const EntityType>()(entity));
		}

		const bool newcomer(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->newcomer() : false;
		}

		// --------------------------------------------------------------------------------

		const bool removed(const EntityType& entity) const {
			return removed(traits::pk_tie<const EntityType>()(entity));
		}

		const bool removed(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->removed() : false;
		}

		// --------------------------------------------------------------------------------
		
		const void remove(const EntityType& entity) const {
			return remove(traits::pk_tie<const EntityType>()(entity));
		}

		const void remove(db_pk_tuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->remove() : false;
		}

		// --------------------------------------------------------------------------------

	public:
		void dbg() {
			auto& index = _container.get<by_sequence>();
			auto begin = index.begin();
			auto end = index.end();

			// Calc max size of this display column.
			size_t max_size = 0;
			for (auto itr = begin; itr != end; ++itr) {
				std::ostringstream oss;
				oss << boost::fusion::tuple_open("")
				    << boost::fusion::tuple_close("")
				    << boost::fusion::tuple_delimiter(", ")
				    << itr->data;

				const size_t size = oss.str().size();
				if (size > max_size) {
					max_size = size;
				}
			}

			for (auto itr = begin; itr != end; ++itr) {
				std::ostringstream data_oss;
				data_oss << boost::fusion::tuple_open("")
					 << boost::fusion::tuple_close("")
					 << boost::fusion::tuple_delimiter(", ")
					 << itr->data;

				std::cerr
					<< itr->flags << " | "

					<< itr->pk    << " | "

					<< std::dec
					<< std::setw(max_size)
					<< std::setfill('\0')
					<< std::left
					<< data_oss.str() << " | "
					
					<< itr->load_state << " | "

					<< std::hex
					<< std::setw(8)
					<< std::setfill('0')
					<< std::right
					<< itr->change_state

					<< std::endl;
			}
		}
	};

}}
