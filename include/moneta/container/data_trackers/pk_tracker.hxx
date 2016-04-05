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
#include "../meta_set.hxx"
#include "../../traits/pk.hxx"
#include "../../traits/extract_pk.hxx"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/lexical_cast.hpp>

namespace moneta { namespace container {

	namespace detail {

		template <class Master, class Entity>
		struct pk_tracker_impl {
			typedef Master base_type;
			typedef Entity entity_type;

			typedef pk_tracker_impl this_type;

			typedef typename boost::call_traits<
				typename traits::pk<Entity>::type
			>::param_type param_type;

			struct entry {
				typedef typename traits::pk<
					Entity
				>::type state_type;

				state_type pk;

				entry() {}

				// XXX: Add non-const too.
				entry(const Entity& entity) {
					pk = moneta::traits::extract_pk(entity);
				}

				std::string to_string() const {
					const std::string pk_str =
					  boost::lexical_cast<std::string>(pk);
					return std::string("pk=") + pk_str;
				}
			};

			struct get_index {
				template <class MasterEntry>
				struct apply {
					typedef boost::multi_index::hashed_unique<
						boost::multi_index::tag<this_type>,
						boost::multi_index::member<
							entry,
							typename entry::state_type,
							&entry::pk
						>
					> type;
				};
			};

			template <class M>
			struct traits {
				typedef typename M::container_type container_type;
				typedef const container_type const_container_type;


				typedef typename container_type::
					template index<
						this_type
					>::type
				index_type;
				typedef const index_type const_index_type;


				typedef typename index_type::iterator
					iterator_type;

				typedef typename index_type::const_iterator
					const_iterator_type;
			};

			template <class LazyHack = Master>
			boost::optional<typename LazyHack::entry>
			find(param_type pk) const {
				typename traits<Master>::const_index_type&
				index = Master::container(this).
				  MONETA_DEDUCED_TYPENAME get<this_type>();

				typename traits<Master>::const_iterator_type
				itr = index.find(pk);

				return (itr != index.end())? *itr :
				       boost::optional<typename Master::entry>();
			}

			void erase(param_type pk) {
				typename traits<Master>::index_type&
				index = Master::container(this).
				  MONETA_DEDUCED_TYPENAME get<this_type>();

				typename traits<Master>::iterator_type
				itr = index.find(pk);

				if (itr != index.end()) {
					index.erase(itr);
				}
			}

			// TODO: Verify deprecation.
			template <class LazyHack = Master>
			void replace(param_type pk, typename LazyHack::entry& entry_) {
				typename traits<Master>::index_type& index = Master::container(this).MONETA_DEDUCED_TYPENAME get<this_type>();
				typename traits<Master>::iterator_type itr = index.find(pk);

				if (itr != index.end()) {
				//	index.replace(itr, entry_);
				}
			}
		};
	
	} // namespace detail

	template <class Entity>
	struct pk_tracker : boost::mpl::lambda<
		detail::pk_tracker_impl<boost::mpl::_1, Entity>
	>::type {
		typedef Entity entity_type;
	};

}}
