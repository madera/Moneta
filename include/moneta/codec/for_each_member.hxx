#pragma once
#include "../traits/member_names.hxx"
#include "../traits/detail/memptr_hacker.hxx"
#include "../traits/is_entity.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace codec {

	namespace detail {

		template <class MaybeEntity, class Path, class Enable = void>
		struct apply_operation;

		template <class MemberEntity, class Path>
		struct apply_operation<
			MemberEntity,
			Path,
			typename boost::enable_if<
				traits::is_entity<
					typename MemberEntity::result_type
				>
			>::type
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

		template <class NonEntityMemberType, class Path>
		struct apply_operation<
			NonEntityMemberType,
			Path,
			typename boost::disable_if<
				traits::is_entity<
					typename NonEntityMemberType::result_type
				>
			>::type
		> {
			template <class Operation, class Entity>
			void operator()(Operation operation, Entity& entity) {
				operation.operator()< // XXX
					Entity&,
					NonEntityMemberType,
					Path
				>(entity, NonEntityMemberType());
			}

			template <class Operation, class Entity>
			void operator()(Operation operation, const Entity& entity) {
				operation.operator()< // XXX
					const Entity&,
					NonEntityMemberType,
					Path
				>(entity, NonEntityMemberType());
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
			void operator()(Member& member) const {
				apply_operation<Member, Path>()(_operation, _entity);
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
