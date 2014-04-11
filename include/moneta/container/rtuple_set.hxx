#pragma once
#include "change_trackers/hash_change_tracker.hxx"
#include "change_trackers/null_change_tracker.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "load_trackers/null_load_tracker.hxx"
#include "../make_entity.hxx"
#include "../traits/to_entity.hxx"
#include "../traits/detail/sub_tie_vector.hxx"
#include "../traits/pk_rtuple.hxx"
#include "../traits/rtuple.hxx"
#include "../traits/to_rtuple.hxx"
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/mpl/vector_c.hpp>

#include <iomanip>
#include <boost/tuple/tuple_io.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/format.hpp>
#include <boost/mpl/print.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/fusion/include/io.hpp>


// XXX: Move!
//template <class EntityType>
//struct rtuple_pk_extractor {
//	BOOST_MPL_ASSERT((moneta::traits::is_entity<EntityType>));
//
//	// XXX: Temporary... just for now.
//	BOOST_MPL_ASSERT_NOT((boost::is_const<EntityType>));
//
//	typedef typename moneta::traits::rtuple<EntityType>::type rtuple_type;	
//	typedef boost::mpl::vector_c<int, 0> pk_indeces;
//	typedef moneta::traits::detail::sub_tie<rtuple_type, pk_indeces> rtuple_pk_type;
//	typedef moneta::traits::detail::deref_if_unary<rtuple_pk_type> pk_derefer_type;
//	typedef typename pk_derefer_type::type type;
//
//	type operator()(rtuple_type& rtuple) const {
//		return pk_derefer_type()(rtuple_pk_type(rtuple));
//	}
//
//	type operator()(const rtuple_type& rtuple) const {
//		auto x = rtuple_pk_type(rtuple);
//		return pk_derefer_type()(x);
//	}
//};
//
//template <class EntityType>
//typename rtuple_pk_extractor<EntityType>::type
//extract_rtuple_pk(const typename rtuple_pk_extractor<EntityType>::rtuple_type& rtuple) {
//	return rtuple_pk_extractor<EntityType>()(rtuple);
//}
// !evoM :XXX



namespace moneta { namespace container {

	namespace mi = boost::multi_index;

	template <
		class EntityType,
		class LoadTracker = /*null_load_tracker<EntityType>,*/ bitset_load_tracker<EntityType>,
		class ChangeTracker = /*null_change_tracker<EntityType>*/ hash_change_tracker<EntityType>
	>
	class rtuple_set { // : boost::noncopyable {

		// We don't support entities with no PKs (yet).
		BOOST_MPL_ASSERT((moneta::traits::has_pk<EntityType>));

		// XXX: Temporary... just for now.
		BOOST_MPL_ASSERT_NOT((boost::is_const<EntityType>));

		// TODO: Make this intelligent.
		enum {
			NEWCOMER_FLAG = 128,
			DIRTY_FLAG    = 64,
			REMOVED_FLAG  = 32
		};

		typedef typename traits::pk_rtuple<
			EntityType
		>::type pk_rtuple_type;
		
		typedef typename boost::call_traits<
			pk_rtuple_type
		>::param_type pk_rtuple_param_type;

		typedef typename traits::rtuple<
			EntityType
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
			   pk(rand()),
			   //pk(traits::detail::to_pk_rtuple(rtuple)),
			   data(rtuple) {
				if (newcomer) {
					flags |= NEWCOMER_FLAG;
				}
			};

			const bool operator==(const pk_rtuple_type& rhs) const {
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
			index::const_iterator begin = _container.get<by_hash>().begin();
			index::const_iterator   end = _container.get<by_hash>().end();
			index::const_iterator   itr = std::find(begin, end, pk);
			return (itr == end)? boost::optional<entry>() : *itr;
		}

		void replace_entry(const pk_rtuple_param_type pk, const entry& entry) {
			typedef typename container_type::template index<by_hash>::type index;
			index::const_iterator begin = _container.get<by_hash>().begin();
			index::const_iterator   end = _container.get<by_hash>().end();
			index::const_iterator   itr = std::find(begin, end, pk);
			assert(itr != end);
			_container.get<by_hash>().replace(itr, entry);
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
		const size_t size() {
			return _container.get<0>().size();
		}

		void insert(const rtuple_type& rtuple, const bool newcomer = false) {
			_container.get<by_hash>().insert(entry(rtuple, newcomer));
		}

		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------

		const bool is_bound(const pk_rtuple_param_type pk) const {
			return get_entry(pk).is_initialized();
		}

		// --------------------------------------------------------------------------------

		const bool is_dirty(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? entry->dirty(entry->data) : false;
		}

		// --------------------------------------------------------------------------------

		const bool is_newcomer(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? ((entry->flags & NEWCOMER_FLAG) != 0) : false;
		}

		// --------------------------------------------------------------------------------

		const bool is_removed(const pk_rtuple_param_type pk) const {
			boost::optional<entry> entry = get_entry(pk);
			return (entry.is_initialized())? ((entry->flags & REMOVED_FLAG) != 0) : false;
		}

		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		// --------------------------------------------------------------------------------
		
		const void remove(pk_rtuple_param_type pk) {
			set_entry_flag(pk, REMOVED_FLAG);
		}

		// --------------------------------------------------------------------------------

	public:
		void debug_dump() {
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
