#pragma once
#include "../traits/member_names.hxx"
#include "../traits/detail/memptr_hacker.hxx"
#include "../traits/is_entity.hxx"
#include "../make_entity.hxx"

// TODO: Use more sensible template argument names.

namespace moneta { namespace serialization {

	namespace detail {

		template <class MaybeEntityType, class Enable = void>
		struct apply_operation;

		template <class MemberEntityType>
		struct apply_operation<
			MemberEntityType,
			typename boost::enable_if<
				traits::is_entity<MemberEntityType>
			>::type
		> {
			template <class Operation, class EntityType, class MemberPointer>
			void operator()(Operation operation, EntityType& entity, MemberPointer memptr, MemberEntityType& value) {
				for_each_member(value, operation);
			}

			template <class Operation, class EntityType, class MemberPointer>
			void operator()(Operation operation, const EntityType& entity, MemberPointer memptr, const MemberEntityType& value) {
				for_each_member(value, operation);
			}
		};

		template <class NonEntityMemberType>
		struct apply_operation<
			NonEntityMemberType,
			typename boost::enable_if<
				boost::mpl::not_<traits::is_entity<NonEntityMemberType> >
			>::type
		> {
			template <class Operation, class EntityType, class MemberPointer>
			void operator()(Operation operation, EntityType& entity, MemberPointer memptr, NonEntityMemberType& value) {
				operation(entity, memptr, value);
			}

			template <class Operation, class EntityType, class MemberPointer>
			void operator()(Operation operation, const EntityType& entity, MemberPointer memptr, const NonEntityMemberType& value) {
				operation(entity, memptr, value);
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
			static void decompose(T K::* memptr, K& entity, Operation operation) {
				apply_operation<T>()(operation, entity, memptr, entity.*memptr);
			}

			template <typename T, class K, class Operation>
			static void decompose(T K::* memptr, const K& entity, Operation operation) {
				apply_operation<T>()(operation, entity, memptr, entity.*memptr);
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
