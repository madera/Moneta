#pragma once
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../make_entity.hxx"

namespace moneta { namespace algorithm {

	namespace detail {

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(enter)

		template <class Path, class Operation, class Entity>
		typename boost::enable_if<has_member_enter<Operation> >::type
		call_enter_if_defined(Operation& operation, Entity& entity) {
			// NOTE: A compile time error here could indicate an unexpected signature
			//       for the user defined enter() method.
			return operation.enter<Entity, Path>(entity);
		}

		template <class Path, class Operation, class Entity>
		typename boost::disable_if<has_member_enter<Operation> >::type
		call_enter_if_defined(Operation& operation, Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(leave)

		template <class Path, class Operation, class Entity>
		typename boost::enable_if<has_member_leave<Operation> >::type
		call_leave_if_defined(Operation& operation, Entity& entity) {
			// NOTE: A compile time error here could indicate an unexpected signature
			//       for the user defined leave() method.
			return operation.leave<Entity, Path>(entity);
		}

		template <class Path, class Operation, class Entity>
		typename boost::disable_if<has_member_leave<Operation> >::type
		call_leave_if_defined(Operation& operation, Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		// TODO: Could this be a simple function?
		template <class MaybeEntity, class Path, class Enable = void>
		struct recurse_or_call_operation;

		// Specialization for: Recurse
		//
		template <class Member, class Path>
		struct recurse_or_call_operation<
			Member,
			Path,
			typename boost::enable_if<
				traits::is_entity<
					typename Member::result_type
				>
			>::type
		> {
			template <class Operation, class Entity>
			void operator()(Operation& operation, Entity& entity) {
				for_each_member_impl(
					Member()(entity),
					operation,
					boost::mpl::push_back<Path, Member>::type()
				);
			}
		};

		// Specialization for: Operate
		//
		template <class Member, class Path>
		struct recurse_or_call_operation<
			Member, Path,
			typename boost::disable_if<
				traits::is_entity<
					typename Member::result_type
				>
			>::type
		> {
			template <class Operation, class Entity>
			void operator()(Operation& operation, Entity& entity) {
				// TODO: Write a clever comment to alert users in case of compile error here.

				// FIXME: Encapsulate this call so it works with free functions et al.
				operation.operator()<Entity, Member, Path>(entity);
			}
		};

		template <class Entity, class Operation, class Path>
		class member_operator {
			Entity& _entity;
			Operation& _operation;
		public:
			member_operator(Entity& entity, Operation& operation)
			 : _entity(entity), _operation(operation) {}

			template <typename Member>
			void operator()(Member&) const {
				recurse_or_call_operation<Member, Path>()(_operation, _entity);
			}
		};

		template <class Members, class Entity, class Operation, class Path = boost::mpl::vector0<> >
		void for_some_members_impl(Entity& entity, Operation& operation, Path& path = Path()) {
			call_enter_if_defined<Path>(operation, entity);

			boost::mpl::for_each<
				Members
			>(detail::member_operator<Entity, Operation, Path>(entity, operation));

			call_leave_if_defined<Path>(operation, entity);
		}

		template <class Entity, class Operation, class Path = boost::mpl::vector0<> >
		void for_each_member_impl(Entity& entity, Operation& operation, Path& path = Path()) {
			for_some_members_impl<
				typename traits::members<Entity>::type, Entity, Operation, Path
			>(entity, operation);
		}
	}

	template <class Members, class Entity, class Operation>
	void for_some_members(Entity& entity, Operation& operation) {
		detail::for_some_members_impl<Members>(entity, operation);
	}

	template <class Entity, class Operation>
	void for_each_member(Entity& entity, Operation& operation) {
		detail::for_each_member_impl(entity, operation);
	}

}}
