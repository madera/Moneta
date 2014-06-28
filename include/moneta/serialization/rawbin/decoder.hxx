#pragma once
#include "io.hxx"
#include "../for_each_member.hxx"
#include "../../traits/member_names.hxx"
#include "../../traits/detail/memptr_hacker.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class EntityType, class IteratorType>
		struct decoder {
			EntityType& _entity;
			IteratorType& _begin;
			IteratorType& _end;

			decoder(EntityType& entity, IteratorType& begin, IteratorType& end)
			 : _entity(entity), _begin(begin), _end(end) {}

			template <class T>
			void operator()(T x) const {
				_begin = write_raw(x, _begin, _end);
			}
		};

	}

// 	template <class EntityType, class IteratorType>
// 	IteratorType encode(EntityType& entity, IteratorType begin, IteratorType end) {
// 		moneta::serialization::for_each_member(entity, detail::encoder<IteratorType>(begin, end));
// 		return begin;
// 	}

	template <class EntityType, class IteratorType>
	int decode(IteratorType begin, IteratorType end, EntityType& entity) {
		return detail::decoder<EntityType>()(begin, end, entity);
	}

}}}
