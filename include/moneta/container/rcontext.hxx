#pragma once
#include "detail/seek_entity_types.hxx"
#include "rtuple_set.hxx"
#include "../traits/tuple.hxx"
#include "../traits/rtuple.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/entity_name.hxx"
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
			operator()(const PairType& pair) const {
				boost::fusion::get<1>(pair) = _context.MONETA_INTRA_TEMPLATE_KEYWORD insert<
					typename traits::pure_type<typename mpl::at_c<PairType, 0>::type>::type
				>(boost::fusion::get<0>(pair));
			}

			template <typename PairType>
			typename boost::enable_if<
				mpl::not_<traits::is_entity<typename mpl::at_c<PairType, 0>::type> >,
				void
			>::type
			operator()(const PairType& pair) const {
				boost::fusion::get<1>(pair) = boost::fusion::get<0>(pair);
			}

			template <class Entity>
			typename traits::pk<Entity>::type
			insert(Entity& entity) {
				using namespace moneta::traits;

				typename tie<Entity>::type entity_tuple = to_tie<Entity>(entity);
				typename traits::rtuple<Entity>::type rtuple;

				typedef boost::fusion::vector<
					typename traits::   tie<Entity>::type& ,
					typename traits::rtuple<Entity>::type&
				> zip_vector_type;

				fusion::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, rtuple));
				fusion::for_each(zip, rcontext_inserter(_context));

				std::cout << "Inserting entity: "
					  << traits::get_entity_name<Entity>()
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
			operator()(const PairType& pair) const {
				boost::fusion::get<1>(pair) = _context.MONETA_INTRA_TEMPLATE_KEYWORD replace<
					typename traits::pure_type<typename mpl::at_c<PairType, 0>::type>::type
				>(boost::fusion::get<0>(pair));
			}

			template <typename PairType>
			typename boost::enable_if<
				mpl::not_<traits::is_entity<typename mpl::at_c<PairType, 0>::type> >,
				void
			>::type
			operator()(const PairType& pair) const {
				boost::fusion::get<1>(pair) = boost::fusion::get<0>(pair);
			}

			template <class Entity>
			typename traits::pk<Entity>::type
			replace(Entity& entity) {
				using namespace moneta::traits;

				typename tie<Entity>::type entity_tuple = to_tie<Entity>(entity);
				typename traits::rtuple<Entity>::type rtuple;

				typedef boost::fusion::vector<
					typename traits::   tie<Entity>::type& ,
					typename traits::rtuple<Entity>::type&
				> zip_vector_type;

				fusion::zip_view<zip_vector_type> zip(zip_vector_type(entity_tuple, rtuple));
				fusion::for_each(zip, rcontext_replacer(_context));

				std::cout << "Updating entity: "
					  << traits::get_entity_name<Entity>()
					  << std::endl;

				return extract_pk(entity);
			}
		};
	}

	template <class RootEntity>
	class rcontext {
		typedef rcontext this_type;

		template <class Entity>
		struct optional_rset : boost::mpl::identity<
			boost::optional<
				rtuple_set<Entity>
			>
		> {};

		struct make_container {
			template <class Entity>
			struct apply : optional_rset<Entity>
			{};
		};

		typedef typename detail::rcontext_containers<
			RootEntity,
			make_container
		>::type containers_type;

		// --------------------------------------------------------------------------------

		containers_type _containers;

	public: // XXX: Exposed for debugging purposes :XXX
		template <class Entity>
		typename optional_rset<Entity>::type&
		get_container() {
			return _containers;
		}

	public:
		template <class Entity>
		const size_t size() {
			typename optional_rset<Entity>::type& optional_rset = get_container<Entity>();
			return optional_rset? optional_rset->size() : 0;
		}

		template <class Entity>
		typename traits::pk<Entity>::type
		insert(Entity& entity) {
			detail::rcontext_inserter<this_type> inserter(*this);
			inserter.insert(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			typename optional_rset<Entity>::type& optional_rset = get_container<Entity>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(rtuple_set<Entity>());
			}

			optional_rset->insert(moneta::traits::to_rtuple(entity));
			return traits::extract_pk(entity);
		}

		template <class Entity>
		typename traits::pk<Entity>::type
		replace(Entity& entity) {
			detail::rcontext_replacer<this_type> replacer(*this);
			replacer.replace(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			typename optional_rset<Entity>::type& optional_rset = get_container<Entity>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(rtuple_set<Entity>());
			}

			optional_rset->replace(moneta::traits::to_rtuple(entity));
			return traits::extract_pk(entity);
		}
	};

}}
