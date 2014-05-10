#pragma once
#include "detail/seek_entity_types.hxx"
#include "rtuple_set.hxx"
#include "../traits/tuple.hxx"
#include "../traits/rtuple.hxx"
#include "../traits/is_entity.hxx"
#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/optional.hpp>
#include <boost/fusion/view/zip_view.hpp>
#include <boost/fusion/tuple.hpp>

#include "../sql/generators/insert.hxx"

namespace moneta { namespace container {

	namespace detail {

		// XXX: MOVE THIS!!
		namespace mpl = boost::mpl;
		namespace fusion = boost::fusion;

		template <class ContextType>
		struct rcontext_inserter {
			ContextType& _context;

			rcontext_inserter(ContextType& context)
			 : _context(context) {}

			template <typename PairType>
			typename boost::enable_if<
				traits::is_entity<typename mpl::at_c<PairType, 0>::type>,
				void
			>::type
			operator()(PairType& pair) const {
				boost::fusion::get<1>(pair) = _context.insert<
					typename traits::pure_type<typename mpl::at_c<PairType, 0>::type>::type
				>(boost::fusion::get<0>(pair));
			}

			template <typename PairType>
			typename boost::enable_if<
				mpl::not_<traits::is_entity<typename mpl::at_c<PairType, 0>::type> >,
				void
			>::type
			operator()(PairType& pair) const {
				boost::fusion::get<1>(pair) = boost::fusion::get<0>(pair);
			}

			template <class EntityType>
			typename traits::pk<EntityType>::type
			insert(EntityType& entity) {
				using namespace moneta::traits;

				tie<EntityType>::type entity_tuple = to_tie<EntityType>(entity);
				traits::rtuple<EntityType>::type rtuple;

				typedef boost::fusion::vector<
					typename traits::   tie<EntityType>::type& ,
					typename traits::rtuple<EntityType>::type&
				> zip_vector_type;

				fusion::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, rtuple));
				fusion::for_each(zip, rcontext_inserter(_context));

				std::cout << "Inserting entity: "
					  << traits::get_entity_name<EntityType>()
					  << std::endl;

				return extract_pk(entity);
			}
		};


		template <class ContextType>
		struct rcontext_replacer {
			ContextType& _context;

			rcontext_replacer(ContextType& context)
			 : _context(context) {}

			template <typename PairType>
			typename boost::enable_if<
				traits::is_entity<typename mpl::at_c<PairType, 0>::type>,
				void
			>::type
			operator()(PairType& pair) const {
				boost::fusion::get<1>(pair) = _context.replace<
					typename traits::pure_type<typename mpl::at_c<PairType, 0>::type>::type
				>(boost::fusion::get<0>(pair));
			}

			template <typename PairType>
			typename boost::enable_if<
				mpl::not_<traits::is_entity<typename mpl::at_c<PairType, 0>::type> >,
				void
			>::type
			operator()(PairType& pair) const {
				boost::fusion::get<1>(pair) = boost::fusion::get<0>(pair);
			}

			template <class EntityType>
			typename traits::pk<EntityType>::type
			replace(EntityType& entity) {
				using namespace moneta::traits;

				tie<EntityType>::type entity_tuple = to_tie<EntityType>(entity);
				traits::rtuple<EntityType>::type rtuple;

				typedef boost::fusion::vector<
					typename traits::   tie<EntityType>::type& ,
					typename traits::rtuple<EntityType>::type&
				> zip_vector_type;

				fusion::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, rtuple));
				fusion::for_each(zip, rcontext_replacer(_context));

				std::cout << "Updating entity: "
					  << traits::get_entity_name<EntityType>()
					  << std::endl;

				return extract_pk(entity);
			}
		};
	}

	template <class RootEntityType>
	class rcontext {
		typedef rcontext this_type;

		template <class EntityType>
		struct optional_rset : boost::mpl::identity<
			boost::optional<
				rtuple_set<EntityType>
			>
		> {};

		struct make_container {
			template <class EntityType>
			struct apply : optional_rset<EntityType>
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
		typename optional_rset<EntityType>::type&
		get_container() {
			return _containers;
		}

	public:
		template <class EntityType>
		const size_t size() {
			typename optional_rset<EntityType>::type& optional_rset = get_container<EntityType>();
			return optional_rset? optional_rset->size() : 0;
		}

		template <class EntityType>
		typename traits::pk<EntityType>::type
		insert(EntityType& entity) {
			detail::rcontext_inserter<this_type> inserter(*this);
			inserter.insert(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			typename optional_rset<EntityType>::type& optional_rset = get_container<EntityType>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(rtuple_set<EntityType>());
			}

			optional_rset->insert(moneta::traits::to_rtuple(entity));
			return traits::extract_pk(entity);
		}

		template <class EntityType>
		typename traits::pk<EntityType>::type
		replace(EntityType& entity) {
			detail::rcontext_replacer<this_type> replacer(*this);
			replacer.replace(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			typename optional_rset<EntityType>::type& optional_rset = get_container<EntityType>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(rtuple_set<EntityType>());
			}

			optional_rset->replace(moneta::traits::to_rtuple(entity));
			return traits::extract_pk(entity);
		}
	};

}}
