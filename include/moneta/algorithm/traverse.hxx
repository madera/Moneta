#pragma once
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/detail/has_member_trait.hxx"
#include <boost/mpl/copy_if.hpp>

	//////////////////////////////////////////////////////////////////////////
	//
	// XXX: Free functions as well, dammit...
	//
	//////////////////////////////////////////////////////////////////////////

namespace moneta { namespace algorithm {

	namespace detail {

		template <class Path, class Member, class Enable = void>
		struct add_path : boost::mpl::identity<void> {};

		template <class Path, class Member>
		struct add_path<
			Path, Member,
			typename boost::enable_if<typename boost::mpl::is_sequence<Path>::type>::type
		> : boost::mpl::push_back<Path, Member> {};

		//

		template <class Actions, class Base>
		struct actions_of : boost::mpl::copy_if<
			Actions,
			boost::is_base_of<Base, boost::mpl::_1>,
			boost::mpl::back_inserter<boost::mpl::vector0<> >
		> {};

		//

		template <class Entity, class Path, class State>
		class enter_or_leave_action {
			Entity& _entity;
			State& _state;
		public:
			enter_or_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				typename Action().operator()<Path>(_entity, _state);
			}
		};

		//

		template <class Entity, class Path, class State, class Member>
		class member_action {
			Entity& _entity;
			State& _state;
		public:
			member_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				typename Action().operator()<Path, Member>(_entity, _state);
			}
		};

		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::enable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(Entity& entity, State& state) {
			traverse<Actions, add_path<Path, Member>::type>(Member()(entity), state);
		}

		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::disable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(Entity& entity, State& state) {
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_member>::type
			>(detail::member_action<Entity, Path, State, Member>(entity, state));
		}

		template <class Actions, class Entity, class Path, class State>
		class member_operator {
			Entity& _entity;
			State& _state;
		public:
			member_operator(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Member>
			void operator()(Member&) const {
				call_member_or_recurse<Actions, Path, Member>(_entity, _state);
			}
		};

	}

	struct traverse_enter {};
	struct traverse_member {};
	struct traverse_leave {};

	template <class Actions, class Path = boost::mpl::vector0<>, class Entity = void, class State = void>
	void traverse(Entity& entity, State& state = State()) {
		// Enter actions
		//
		boost::mpl::for_each<
			typename detail::actions_of<Actions, traverse_enter>::type
		>(detail::enter_or_leave_action<Entity, Path, State>(entity, state));

		// Members
		//
		boost::mpl::for_each<typename traits::members<Entity>::type>(
			detail::member_operator<Actions, Entity, Path, State>(entity, state)
		);

		// Leave actions
		//
		boost::mpl::for_each<
			typename detail::actions_of<Actions, traverse_leave>::type
		>(detail::enter_or_leave_action<Entity, Path, State>(entity, state));
	}

}}
