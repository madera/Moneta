#pragma once
#include "change_trackers/hash_change_tracker.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "../make_entity.hxx"
#include "../traits/to_entity.hxx"
#include "../traits/detail/sub_tie_vector.hxx"
#include "../traits/pk_rtuple.hxx"
#include "../traits/rtuple.hxx"
#include "../traits/to_rtuple.hxx"
#include "../traits/to_pk_rtuple.hxx"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/tuple/tuple_io.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/format.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>
#include <iomanip>

namespace moneta { namespace container {

	namespace mi = boost::multi_index;

	template <
		class Entity,
		class LoadTracker = /*null_load_tracker<Entity>,*/ bitset_load_tracker<Entity>,
		class ChangeTracker = /*null_change_tracker<Entity>*/ hash_change_tracker<Entity>
	>
	class rtuple_set { // : boost::noncopyable {

		// We don't support entities with no PKs (yet).
		BOOST_MPL_ASSERT((moneta::traits::has_pk<Entity>));

		// XXX: Temporary... just for now.
		BOOST_MPL_ASSERT_NOT((boost::is_const<Entity>));

		// TODO: Make this intelligent.
		enum {
			NEWCOMER_FLAG = 128,
			DIRTY_FLAG    = 64,
			REMOVED_FLAG  = 32
		};

		typedef typename traits::pk_rtuple<
			Entity
		>::type pk_rtuple_type;
		
		typedef typename boost::call_traits<
			pk_rtuple_type
		>::param_type pk_rtuple_param_type;

		typedef typename traits::rtuple<
			Entity
		>::type rtuple_type;

		typedef traits::detail::sub_tie_vector<
			rtuple_type,
			boost::mpl::vector_c<int, 0>
		> sub_tie_vectorizer_type;

		typedef typename sub_tie_vectorizer_type::type sub_tie_vector_type;

		struct entry : LoadTracker, ChangeTracker {

			int flags;
			pk_rtuple_type pk;
			rtuple_type data;

			explicit entry(
				const rtuple_type& rtuple,
				const bool newcomer = false,
				const bool all_loaded = true
			)
			 : LoadTracker(all_loaded),
			   ChangeTracker(rtuple),
			   flags(0),
			   pk(traits::to_pk_rtuple<Entity>(rtuple)),
			   data(rtuple) {
				if (newcomer) {
					flags |= NEWCOMER_FLAG;
				}
			};

			bool operator==(const pk_rtuple_type& rhs) const {
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
					mi::member<entry, pk_rtuple_type, &entry::pk>
				>,
				mi::ordered_non_unique<
					mi::tag<by_flags>,
					mi::member<entry, int, &entry::flags>
				>
			>
		> container_type;

	private:
		boost::optional<entry> get_entry(const pk_rtuple_param_type pk) const {
			typedef typename container_type::template index<by_hash>::type index;
			typename index::const_iterator begin = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().begin();
			typename index::const_iterator   end = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().end();
			typename index::const_iterator   itr = std::find(begin, end, pk);
			return (itr == end)? boost::optional<entry>() : *itr;
		}

		void replace_entry(const pk_rtuple_param_type pk, const entry& entry) {
			typedef typename container_type::template index<by_hash>::type index;
			typename index::const_iterator begin = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().begin();
			typename index::const_iterator   end = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().end();
			typename index::const_iterator   itr = std::find(begin, end, pk);
			assert(itr != end);
			_container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().replace(itr, entry);
		}

		void set_entry_flag(const pk_rtuple_param_type pk, const size_t flag, const bool value = true) {
			boost::optional<entry> entry = get_entry(pk);
			if (entry) {
				if (value) {
					entry->flags |=  flag;
				} else {
					entry->flags &= ~flag;
				}

				replace_entry(pk, *entry);
			}
		}
	private:
		container_type _container;

	public:
		size_t size() {
			return _container.MONETA_INTRA_TEMPLATE_KEYWORD get<0>().size();
		}

		void insert(const rtuple_type& rtuple, const bool newcomer = false) {
			_container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().insert(entry(rtuple, newcomer));
		}

		void replace(const rtuple_type& rtuple) {
			auto itr = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().find(traits::to_pk_rtuple<Entity>(rtuple));
			if (itr != _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().end()) {
				_container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_hash>().replace(itr, entry(rtuple));
			}
		}

		void erase(const rtuple_type& rtuple) {
			erase(traits::to_pk_rtuple<Entity>(rtuple));
		}

		void erase(const pk_rtuple_type /*pk*/) {
			std::cerr << "erasing..." << std::endl;
		}

		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------

		bool is_bound(const pk_rtuple_param_type pk) const {
			return get_entry(pk).is_initialized();
		}

		// --------------------------------------------------------------------------------

		bool is_dirty(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->dirty(entry->data) : false;
		}

		// --------------------------------------------------------------------------------

		bool is_newcomer(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? ((entry->flags & NEWCOMER_FLAG) != 0) : false;
		}

		// --------------------------------------------------------------------------------

		bool is_removed(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? ((entry->flags & REMOVED_FLAG) != 0) : false;
		}

		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		
		void remove(pk_rtuple_param_type pk) {
			set_entry_flag(pk, REMOVED_FLAG);
		}

		// --------------------------------------------------------------------------------

	public:
		void debug_dump() {
			auto& index = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<by_sequence>();
			auto begin = index.begin();
			auto end = index.end();

			// Calc max size of this display column.
			size_t data_max_size = 0;
			for (auto itr = begin; itr != end; ++itr) {
				std::ostringstream oss;
				oss << boost::fusion::tuple_open("")
				    << boost::fusion::tuple_close("")
				    << boost::fusion::tuple_delimiter(", ")
				    << itr->data;

				const size_t size = oss.str().size();
				if (size > data_max_size) {
					data_max_size = size;
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

					<< std::dec
					<< itr->pk << " | "

					<< std::setw(data_max_size)
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
