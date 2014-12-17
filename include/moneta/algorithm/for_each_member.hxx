#pragma once
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../make_entity.hxx"

// TODO: Document that this function should be called for_each_non_entity_member. Or find a better name for it.

namespace moneta { namespace algorithm {

	namespace detail {

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(enter)

		template <class Path, class Operation, class Entity>
		typename boost::enable_if<has_member_enter<Operation> >::type
		call_enter_if_defined(const Operation& operation, Entity& entity) {
			// NOTE: A compile time error here could indicate an unexpected signature
			//       for the user defined enter() method.
			return operation.template enter<Entity, Path>(entity);
		}

		template <class Path, class Operation, class Entity>
		typename boost::disable_if<has_member_enter<Operation> >::type
		call_enter_if_defined(const Operation& operation, Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(leave)

		template <class Path, class Operation, class Entity>
		typename boost::enable_if<has_member_leave<Operation> >::type
		call_leave_if_defined(const Operation& operation, Entity& entity) {
			// NOTE: A compile time error here could indicate an unexpected signature
			//       for the user defined leave() method.
			return operation.template leave<Entity, Path>(entity);
		}

		template <class Path, class Operation, class Entity>
		typename boost::disable_if<has_member_leave<Operation> >::type
		call_leave_if_defined(const Operation& operation, Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//
		// Specialization for: Operate
		//
		// TODO: Could this be a simple function?

		template <class MaybeEntity, class Path, class Enable = void>
		struct recurse_or_call_operation;

		template <class Entity, class Operation, class Path>
		class do_member_operator {
			Entity& _entity;
			const Operation& _operation;
		public:
			do_member_operator(Entity& entity, const Operation& operation)
			 : _entity(entity), _operation(operation) {}

			template <typename Member>
			void operator()(Member&) const {
				recurse_or_call_operation<Member, Path>()(_entity, _operation);
			}
		};

		// ---

		template <class Entity, class Operation, class Members, class Path = boost::mpl::vector0<> >
		struct for_some_members_impl {
			void operator()(Entity& entity, const Operation& operation) const {
				call_enter_if_defined<Path>(operation, entity);

				boost::mpl::for_each<
					Members
				>(detail::do_member_operator<Entity, Operation, Path>(entity, operation));

				call_leave_if_defined<Path>(operation, entity);
			}
		};

		template <class Members, class Path, class Entity, class Operation>
		void call_for_some_members_impl(Entity& entity, Operation& operation) {
			for_some_members_impl<Entity, Operation, Members, Path>()(entity, operation);
		}

		// ---

		template <class Entity, class Operation, class Path = boost::mpl::vector0<> >
		struct for_each_member_impl {
			void operator()(Entity& entity, const Operation& operation) const {
				typedef typename traits::members<Entity>::type members;
				call_for_some_members_impl<members, Path>(entity, operation);
			}
		};

		template <class Path, class Entity, class Operation>
		void call_for_each_member_impl(Entity& entity, Operation& operation) {
			for_each_member_impl<Entity, Operation, Path>()(entity, operation);
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		// Specialization for: Recurse
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
			void operator()(Entity& entity, const Operation& operation) {
				// TODO: Write a clever comment to alert users in case of compile error here.

				// FIXME: Encapsulate this call so it works with free functions et al.
#ifdef BOOST_MSVC
				operation.operator()<Entity, Member, Path>(entity);
#else
				operation.template operator()<Entity, Member, Path>(entity);
#endif
			}
		};

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
			void operator()(Entity& entity, const Operation& operation) {
				typedef typename boost::mpl::push_back<Path, Member>::type new_path;
				call_for_each_member_impl<new_path>(Member()(entity), operation);
			}
		};
	}

	template <class Members, class Entity, class Operation>
	void for_some_members(Entity& entity, const Operation& operation) {
		detail::for_some_members_impl<Entity, Operation, Members>()(entity, operation);
	}

	template <class Entity, class Operation>
	void for_each_member(Entity& entity, const Operation& operation) {
		detail::for_each_member_impl<Entity, Operation>()(entity, operation);
	}

}}
