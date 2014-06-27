#pragma once
#include "../traits/member_names.hxx"
#include "../traits/detail/memptr_hacker.hxx"
#include "../traits/is_entity.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace serialization {

	namespace detail {

		template <class MaybeEntityType, class Enable = void>
		struct apply_operation;

		template<class EntityType>
		struct apply_operation<
			EntityType,
			typename boost::enable_if<
				traits::is_entity<EntityType>
			>::type
		> {
			template <class Operation>
			void operator()(EntityType& entity, Operation operation) {
				for_each_member(entity, operation);
			}

			template <class Operation>
			void operator()(const EntityType& entity, Operation operation) {
				for_each_member(entity, operation);
			}
		};

		template <class NonEntityType>
		struct apply_operation<
			NonEntityType,
			typename boost::enable_if<
				boost::mpl::not_<traits::is_entity<NonEntityType> >
			>::type
		> {
			template <class Operation>
			void operator()(NonEntityType& value, Operation operation) {
				operation(value);
			}

			template <class Operation>
			void operator()(const NonEntityType& value, Operation operation) {
				operation(value);
			}
		};

		template <class EntityType, class Operation>
		class member_operator {
			EntityType& _entity;
			Operation _operation;
		public:
			member_operator(EntityType& entity, Operation operation)
			 : _entity(entity), _operation(operation) {}

			template <typename MemberPointer>
			void operator()(MemberPointer& memptr) const {
				decompose(memptr, _entity, _operation);
			}

			template <typename T, class K, class Operation>
			static void decompose(T K::* memptr, K& x, Operation operation) {
				apply_operation<T>()(x.*memptr, operation);
			}

			template <typename T, class K, class Operation>
			static void decompose(T K::* memptr, const K& x, Operation operation) {
				apply_operation<T>()(x.*memptr, operation);
			}
		};

	}

	template <class EntityType, class Operation>
	void for_each_member(EntityType& entity, Operation operation) {
		boost::fusion::for_each(
			traits::member_pointers<EntityType>::get(),
			detail::member_operator<EntityType, Operation>(entity, operation)
		);
	}

}}
