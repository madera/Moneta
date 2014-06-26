#pragma once
#include "io.hxx"
#include "../../traits/member_names.hxx"
#include "../../traits/detail/memptr_hacker.hxx"
#include "../../traits/is_entity.hxx"
#include "../../make_entity.hxx"
#include <boost/fusion/include/at_c.hpp>

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <class EntityType, class IteratorType>
		class member_encoder {
			const EntityType& _entity;
			IteratorType& _begin;
			IteratorType& _end;
		public:
			member_encoder(const EntityType& entity, IteratorType& begin, IteratorType& end)
			 : _entity(entity), _begin(begin), _end(end) {}

			template <typename MemberPointer>
			void operator()(MemberPointer& memptr) const {
				_begin = encode_member(memptr, _entity, _begin, _end);
			}
		};

		template <typename T, class K, class IteratorType>
		IteratorType encode_member(T K::* memptr, const K& x, IteratorType begin, IteratorType end) {
			return encoder<T>()(x.*memptr, begin, end);
		}

		template <class EntityType, class Enable = void>
		struct encoder;

		template <class NonEntityType>
		struct encoder<
			NonEntityType,
			typename boost::enable_if<
				boost::mpl::not_<traits::is_entity<NonEntityType> >
			>::type
		> {
			template <class IteratorType>
			IteratorType operator()(const NonEntityType& value, IteratorType begin, IteratorType end) {
				return write_raw(value, begin, end);
			}
		};

		template<class EntityType>
		struct encoder<
			EntityType,
			typename boost::enable_if<
				traits::is_entity<EntityType>
			>::type
		> {
			template <class IteratorType>
			IteratorType operator()(const EntityType& entity, IteratorType begin, IteratorType end) {
				boost::fusion::for_each(
					traits::member_pointers<EntityType>::get(),
					member_encoder<EntityType, IteratorType>(entity, begin, end)
				);

				return begin;
			}
		};

	}

	template <class EntityType, class IteratorType>
	IteratorType encode(EntityType& entity, IteratorType begin, IteratorType end) {
		return detail::encoder<EntityType>()(entity, begin, end);
	}

}}}
