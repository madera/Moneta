#pragma once
#include "detail/seek_entity_types.hxx"
#include "rtuple_set.hxx"
#include "../traits/tuple.hxx"
#include "../traits/is_entity.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/optional.hpp>

namespace moneta { namespace container {

	namespace detail {

		// XXX: MOVE THIS!!
		namespace mpl = boost::mpl;
		namespace fuz = boost::fusion;
		using namespace moneta::traits;

		template <class ContextType>
		struct rcontext_inserter {
			ContextType& _context;

			rcontext_inserter(ContextType& context)
			 : _context(context) {}

			template <typename PairType>
			typename boost::enable_if<is_entity<typename mpl::at_c<PairType, 0>::type>, void>::type
			operator()(PairType& pair) const {
				typedef typename pure_type<typename mpl::at_c<PairType, 0>::type>::type type;
				fuz::get<1>(pair) = soci_create<type>(fuz::get<0>(pair));
			}

			template <typename PairType>
			typename boost::enable_if<mpl::not_<is_entity<typename mpl::at_c<PairType, 0>::type>>, void>::type
			operator()(PairType& pair) const {
				fuz::get<1>(pair) = fuz::get<0>(pair);
			}

			template <class EntityType>
			void insert(EntityType& entity) {
				tie<EntityType>::type entity_tuple = to_tie<EntityType>(entity);
				sql::rtuple<EntityType>::type rtuple;

				typedef fuz::vector<
					tie<EntityType>::type&,
					sql::rtuple<EntityType>::type&
				> zip_vector_type;

				fuz::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, rtuple));
				fuz::for_each(zip, recursive_soci_serializer(session));

				session << sql::generators::insert_into_table<EntityType>(), ::soci::use(rtuple);

				return extract_pk(entity);
			}
		};
	}

	template <class RootEntityType>
	class rcontext {
		template <class EntityType>
		struct container {
			typedef boost::optional<
				rtuple_set<EntityType>
			> type;
		};

		struct make_container {
			template <class EntityType>
			struct apply : container<EntityType>
			{};
		};

		typedef typename detail::rcontext_containers<
			RootEntityType,
			make_container
		>::type containers_type;

		// --------------------------------------------------------------------------------

		containers_type _containers;

	public: // XXX: Exposed for debugging purposes :XXX
		template <class EntityType>
		typename container<EntityType>::type&
		get_container() {
			return _containers;
		}

	public:
		template <class EntityType>
		const size_t size() {
			typename container<EntityType>::type& container = get_container<EntityType>();
			return container? container->size() : 0;
		}

		template <class EntityType>
		void insert(const EntityType& entity) {

			// Get container entry, and "allocate" a new instance if not initialized.
			//
			typename container<EntityType>::type& set = get_container<EntityType>();
			if (!set.is_initialized()) {
				set.reset(rtuple_set<EntityType>());
			}

			set->insert(moneta::traits::to_rtuple(entity));
		}

	};

}}
