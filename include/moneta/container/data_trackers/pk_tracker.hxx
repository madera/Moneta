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

// TODO: Use BOOST_DEDUCED_TYPENAME

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
				typename moneta::traits::pk<Entity>::type
			>::param_type param_type;

			struct entry {
				typedef typename moneta::traits::pk<Entity>::type state_type;
				state_type pk;

				entry() {}

				// XXX: Add non-const too.
				entry(const Entity& entity) {
					pk = moneta::traits::extract_pk(entity);
				}

				std::string to_string() const {
					return boost::lexical_cast<std::string>(pk);
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

			struct is_entry_equal {
				param_type& _needle;
				
				is_entry_equal(const param_type& needle)
				 : _needle(needle) {}

				bool operator()(const entry& e) {
					return e.pk == _needle;
				}
			};

			template <class LazyHack = Master>
			boost::optional<typename LazyHack::entry>
			find(param_type pk) const {
				const typename Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;

				// TODO: Use the fucking index, motherfucker!!

				typename index::const_iterator itr = std::find_if(
					_container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().begin(),
					_container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().end(),
					is_entry_equal(pk)
				);

				return (itr == _container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().end())?
					boost::optional<typename Master::entry>() : *itr;
			}

			void erase(param_type pk) {
				typename Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;
				typename index::const_iterator begin = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().begin();
				typename index::const_iterator end   = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().end();
				typename index::const_iterator itr   = std::find_if(begin, end, is_entry_equal(pk));

				if (itr != end) {
					_container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().erase(itr);
				}
			}

			// DEPRECATED UNTIL REFACTORING
			template <class LazyHack = Master>
			void replace(param_type pk, typename LazyHack::entry& entry_) {
				typename Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;
				typename index::const_iterator begin = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().begin();
				typename index::const_iterator end   = _container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().end();
				typename index::const_iterator itr   = std::find_if(begin, end, is_entry_equal(pk));
				assert(itr != end);
				_container.MONETA_INTRA_TEMPLATE_KEYWORD get<this_type>().replace(itr, entry_);
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
