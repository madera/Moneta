#pragma once
#include "../traits/member_names.hxx"
#include "../traits/detail/memptr_hacker.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace codec {

	namespace detail {

		template <class Operation, class Entity, class Member, class Path, class Enable = void>
		struct apply_operation_impl;

		template <class Operation, class Entity, class Member, class Path>
		struct apply_operation_impl<
			Operation, Entity, Member, Path,
			typename boost::enable_if<
				traits::detail::is_functor_callable<Operation, void(Entity&, Member, Path)>
			>::type
		> {
			void operator()(Operation operation, Entity& entity, Member member) const {
				operation(entity, member, Path());
			}
		};

		template <class Operation, class Entity, class Member, class Path>
		struct apply_operation_impl<
			Operation, Entity, Member, Path,
			typename boost::enable_if<
				traits::detail::is_functor_callable<Operation, void(Entity&, Member)>
			>::type
		> {
			void operator()(Operation operation, Entity& entity, Member member) const {
				operation(entity, member);
			}
		};

		//
		// --------------------------------------------------------------------------------------------------
		//

		template <class MaybeEntity, class Path, class Enable = void>
		struct recurse_or_call_operation;

		// Specialization for: Recurse
		//
		template <class MemberEntity, class Path>
		struct recurse_or_call_operation<
			MemberEntity,
			Path,
			typename boost::enable_if<traits::is_entity<typename MemberEntity::result_type> >::type
		> {
			template <class Operation, class Entity>
			void operator()(Operation operation, Entity& entity) {
				for_each_member<
					typename MemberEntity::result_type&,
					Operation,
					boost::mpl::push_back<Path, MemberEntity>::type
				>(MemberEntity()(entity), operation);
			}

			template <class Operation, class Entity>
			void operator()(Operation operation, const Entity& entity) {
				for_each_member<
					const typename MemberEntity::result_type&,
					Operation,
					boost::mpl::push_back<Path, MemberEntity>::type
				>(MemberEntity()(entity), operation);
			}
		};

		// Specialization for: Operate
		//
		template <class NonEntityMemberType, class Path>
		struct recurse_or_call_operation<
			NonEntityMemberType, Path,
			typename boost::disable_if<traits::is_entity<typename NonEntityMemberType::result_type> >::type
		> {
			template <class Operation, class Entity>
			void operator()(Operation operation, Entity& entity) {
				// TODO: Write a clever comment to alert users in case of compile error here.
				apply_operation_impl<
					Operation, Entity&, NonEntityMemberType, Path
				>()(operation, entity, NonEntityMemberType());
			}

			template <class Operation, class Entity>
			void operator()(Operation operation, const Entity& entity) {
				// TODO: Write a clever comment to alert users in case of compile error here.
				apply_operation_impl<
					Operation, const Entity&, NonEntityMemberType, Path
				>()(operation, entity, NonEntityMemberType());
			}
		};

		template <class Entity, class Operation, class Path>
		class member_operator {
			Entity& _entity;
			Operation _operation;
		public:
			member_operator(Entity& entity, Operation operation)
			 : _entity(entity), _operation(operation) {}

			template <typename Member>
			void operator()(Member member) const {
				recurse_or_call_operation<Member, Path>()(_operation, _entity);
			}
		};

	}

	template <class Members, class Entity, class Operation, class Path = boost::mpl::vector0<> >
	void for_some_members(Entity& entity, Operation operation) {
		boost::mpl::for_each<Members>(detail::member_operator<Entity, Operation, Path>(entity, operation));
	}

	template <class Entity, class Operation, class Path = boost::mpl::vector0<> >
	void for_each_member(Entity& entity, Operation operation) {
		for_some_members<typename traits::members<Entity>::type, Entity, Operation, Path>(entity, operation);
	}

}}
