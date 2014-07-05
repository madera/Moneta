#pragma once
#include "../traits/member_names.hxx"
#include "../traits/detail/memptr_hacker.hxx"
#include "../traits/is_entity.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace codec {

	namespace detail {

		template <class MaybeEntityType, class Enable = void>
		struct apply_operation;

		template <class MemberEntityType>
		struct apply_operation<
			MemberEntityType,
			typename boost::enable_if<
				traits::is_entity<
					typename MemberEntityType::result_type
				>
			>::type
		> {
			template <class Operation, class EntityType>
			void operator()(Operation operation, EntityType& entity) {
				for_each_member(MemberEntityType()(entity), operation);
			}

			template <class Operation, class EntityType>
			void operator()(Operation operation, const EntityType& entity) {
				for_each_member(MemberEntityType()(entity), operation);
			}
		};

		template <class NonEntityMemberType>
		struct apply_operation<
			NonEntityMemberType,
			typename boost::disable_if<
				traits::is_entity<
					typename NonEntityMemberType::result_type
				>
			>::type
		> {
			template <class Operation, class EntityType>
			void operator()(Operation operation, EntityType& entity) {
				operation(entity, NonEntityMemberType());
			}

			template <class Operation, class EntityType>
			void operator()(Operation operation, const EntityType& entity) {
				operation(entity, NonEntityMemberType());
			}
		};

		template <class EntityType, class Operation>
		class member_operator {
			EntityType& _entity;
			Operation _operation;
		public:
			member_operator(EntityType& entity, Operation operation)
			 : _entity(entity), _operation(operation) {}

			template <typename Member>
			void operator()(Member& member) const {
				apply_operation<Member>()(_operation, _entity);
			}
		};

	}

	template <class EntityType, class Operation>
	void for_each_member(EntityType& entity, Operation operation) {
		boost::mpl::for_each<typename traits::members<EntityType>::type>(
			detail::member_operator<EntityType, Operation>(entity, operation)
		);
	}

}}
