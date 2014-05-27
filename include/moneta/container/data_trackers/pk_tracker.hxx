#pragma once
#include "../meta_set.hxx"
#include "../../traits/pk.hxx"
#include "../../traits/extract_pk.hxx"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/lexical_cast.hpp>

namespace moneta { namespace container {

	namespace detail {

		template <class Master, class EntityType>
		struct pk_tracker_impl {
			typedef pk_tracker_impl this_type;

			typedef typename boost::call_traits<
				typename moneta::traits::pk<EntityType>::type
			>::param_type param_type;

			struct entry {
				typedef typename moneta::traits::pk<EntityType>::type state_type;
				state_type pk;

				entry() {}

				// XXX: Add non-const too.
				entry(const EntityType& entity) {
					pk = moneta::traits::extract_pk(entity);
				}

				const bool operator==(param_type rhs) const {
					return pk == rhs;
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

			template <class LazyHack = Master>
			boost::optional<typename LazyHack::entry>
			find(param_type pk) const {
				const Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;
				index::const_iterator begin = _container.get<this_type>().begin();
				index::const_iterator end   = _container.get<this_type>().end();
				index::const_iterator itr   = std::find(begin, end, pk);
				return (itr == end)? boost::optional<typename Master::entry>() : *itr;
			}

			void erase(param_type pk) {
				Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;
				index::const_iterator begin = _container.get<this_type>().begin();
				index::const_iterator end   = _container.get<this_type>().end();
				index::const_iterator itr   = std::find(begin, end, pk);

				if (itr != end) {
					_container.get<this_type>().erase(itr);
				}
			}

			// DEPRECATED UNTIL REFACTORING
			template <class LazyHack = Master>
			void replace(param_type pk, typename LazyHack::entry& entry_) {
				Master::container_type& _container = Master::container(this);
				typedef typename Master::container_type::template index<this_type>::type index;
				index::const_iterator begin = _container.get<this_type>().begin();
				index::const_iterator end   = _container.get<this_type>().end();
				index::const_iterator itr   = std::find(begin, end, pk);
				assert(itr != end);
				_container.get<this_type>().replace(itr, entry_);
			}
		};
	
	} // namespace detail

	template <class EntityType>
	struct pk_tracker : boost::mpl::lambda<
		detail::pk_tracker_impl<boost::mpl::_1, EntityType>
	>::type {};

}}
