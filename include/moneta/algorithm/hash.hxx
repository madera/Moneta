#pragma once
#include "traverse.hxx"
#include <boost/functional/hash.hpp>

namespace moneta { namespace algorithm {

	namespace detail {

		struct hash_present_member {
			template <class Entity, class Value, class State, class Member, class Path>
			void operator()(Entity&, Value& value, State& state, Member, Path) const {
				const size_t hash = boost::hash<Value>()(value);
				state ^= hash << 1;
			}
		};

		struct hash_container_item {
			template <class Entity, typename Value, class State, class Member, class Path>
			void operator()(Entity&, Value& value, State& state, Member, Path) const {
				const size_t hash = boost::hash<Value>()(value);
				state ^= hash << 1;
			}
		};

		template <class Entity, class Enable = void>
		struct hash_impl {
			size_t operator()(const Entity& entity) const {
				typedef moneta::algorithm::traverse<
					present_member_actions<hash_present_member>,
					container_item_actions<hash_container_item>
				> hasher;

				size_t result = 0;
				hasher()(entity, result);
		
				return result;
			}
		};

		template <class NonEntity>
		struct hash_impl<
			NonEntity,
			typename boost::enable_if<
				typename traits::isnt_entity<NonEntity>::type
			>::type
		> {
			size_t operator()(const NonEntity& non_entity) const {
				return boost::hash<NonEntity>()(non_entity);
			}
		};
	}

	template <class T>
	size_t hash(const T& x) {
		return detail::hash_impl<T>()(x);
	}

}}
