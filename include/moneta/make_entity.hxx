#pragma once
#include "traits/tuple.hxx"
#include "traits/to_tie.hxx"
#include "traits/detail/blanker.hxx"
#include "traits/fixed_values.hxx"
#include "traits/members.hxx"
#include <boost/fusion/algorithm/transformation/transform.hpp>
#include "algorithm/for_each_member.hxx"

namespace moneta { namespace algorithm {
	template <class Members, class Entity, class Operation>
	void for_some_members(Entity& entity, Operation& operation);
}}

namespace moneta {

	namespace detail {
		struct fixed_value_setter {
			template <class Entity, class Member, class Path>
			void operator()(Entity& entity) const {
				Member()(entity) = traits::detail::fixed_value<Member>::get();
			}
		};
	}

	template <class Entity>
	typename traits::tuple<Entity>::type make_tuple() {
		typename traits::tuple<Entity>::type result;

		// TODO: Add fixed_value assignment for tuples here.

		return boost::fusion::transform(result, traits::detail::blanker());
	}

	template <class Entity>
	typename traits::pure_type<Entity>::type make_entity() {
		typedef typename traits::pure_type<Entity>::type result_type;

		result_type result;
		traits::to_tie<result_type>(result) = make_tuple<result_type>();

		moneta::algorithm::for_some_members<
			typename traits::fixed_value_members<result_type>::type
		>(result, detail::fixed_value_setter());
		
		return result;
	}
}
