#pragma once
#include "rawbin_io.hxx"
#include "../for_each_member.hxx"
#include "../../traits/member_names.hxx"
#include "../../traits/detail/memptr_hacker.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class IteratorType>
		struct encoder {
			IteratorType& _begin;
			IteratorType& _end;

			encoder(IteratorType& begin, IteratorType& end)
			 : _begin(begin), _end(end) {}

			template <class EntityType, class Member>
			void operator()(EntityType& entity, Member& member) const {
				_begin = rawbin::write(member(entity), _begin, _end);
			}
		};

	}

	template <class EntityType, class IteratorType>
	IteratorType encode(EntityType& entity, IteratorType begin, IteratorType end) {
		moneta::serialization::for_each_member(entity, detail::encoder<IteratorType>(begin, end));
		return begin;
	}

}}}
