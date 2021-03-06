// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "detail/seek_entity_types.hxx"

#include "meta_set.hxx"
#include "data_trackers/pk_tracker.hxx"
#include "data_trackers/rtuple_data.hxx"
#include "load_trackers/bitset_load_tracker.hxx"
#include "change_trackers/hash_change_tracker.hxx"

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

/////////// XXX: ORGANIZE THIS!!
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
///////////
#include <boost/mpl/print.hpp>

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

				//std::cout << "Inserting entity: "
				//	  << traits::get_entity_name<Entity>()
				//	  << std::endl;

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
					typename traits::   tie<Entity>::type&,
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

	// XXX: Think about this...
	template <class Entity>
	struct default_meta_set : boost::mpl::identity<
		detail::meta_set_impl<
			boost::mpl::vector4<
				moneta::container::pk_tracker<Entity>,
				moneta::container::bitset_load_tracker2<Entity>,
				moneta::container::hash_change_tracker2<Entity>,
				moneta::container::rtuple_data<Entity>
			>
		>
	> {};

	template <
		class RootEntity,
		
		template <class Entity>
		class Set = default_meta_set
	>
	/*class*/
	struct rcontext {
		typedef rcontext this_type;

		template <class Entity>
		struct set_type : boost::mpl::identity<
			typename Set<Entity>::type
		> {};

		template <class Entity>
		struct optional_rset : boost::mpl::identity<
			boost::optional<
				typename set_type<Entity>::type
			>
		> {};

		struct make_optional_set {
			template <class Entity>
			struct apply {
				typedef typename optional_rset<Entity>::type type;
			};
		};

		template <class Entity>
		struct container_for {
			typedef typename boost::mpl::apply<
				make_optional_set,
				Entity
			>::type type;
		};

		// ---
		typedef typename detail::seek_entity_types<RootEntity>::type all_entities;

		typedef typename boost::mpl::transform<
			all_entities,
			container_for<boost::mpl::_1>
		>::type container_bases;

		typedef typename boost::mpl::inherit_linearly<
			container_bases,
			boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
		>::type containers_type;

		// --------------------------------------------------------------------------------

		containers_type _containers;

	public: // XXX: Exposed for debugging purposes :XXX
		template <class Entity>
		typename container_for<Entity>::type&
		//typename optional_rset<Entity>::type&
		get_container() {
			return /*(typename optional_rset<Entity>::type&)*/_containers;
		}

	public:
		template <class Entity>
		size_t size() {
			typename container_for<Entity>::type& set = get_container<Entity>();
			return set? set->size() : 0;
		}

		template <class Entity>
		typename traits::pk<Entity>::type
		insert(Entity& entity) {
			detail::rcontext_inserter<this_type> inserter(*this);
			inserter.insert(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			auto& optional_rset = get_container<Entity>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(typename set_type<Entity>::type());
			}

			optional_rset->insert(entity);
			return traits::extract_pk(entity);
		}

		template <class Entity>
		typename traits::pk<Entity>::type
		replace(Entity& entity) {
			detail::rcontext_replacer<this_type> replacer(*this);
			replacer.replace(entity);

			// Get optional_rset entry, and "allocate" a new instance if not initialized.
			auto& optional_rset = get_container<Entity>();
			if (!optional_rset.is_initialized()) {
				optional_rset.reset(typename set_type<Entity>::type());
			}

			optional_rset->replace(entity);
			return traits::extract_pk(entity);
		}
	};

}}
