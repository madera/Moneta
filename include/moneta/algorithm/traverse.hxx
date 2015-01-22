#pragma once
#include "../detail/keywords.hxx"
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/is_container.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/detail/has_member_trait.hxx"
#include <boost/mpl/copy_if.hpp>

namespace moneta { namespace algorithm {

	namespace detail {
		struct no_state {};
	}

	template <
		class Actions,
		class Path = boost::mpl::vector0<>,
		class Entity = void,
		class State = detail::no_state
	>
	void traverse(Entity& entity, State& state = State());

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
				Action().MONETA_INTRA_TEMPLATE_KEYWORD operator()<Path>(_entity, _state);
			}
		};

		template <class Entity, class Path, class Member, class State>
		class container_enter_or_leave_action {
			Entity& _entity;
			State& _state;
		public:
			container_enter_or_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				Action().MONETA_INTRA_TEMPLATE_KEYWORD operator()<Member, Path>(_entity, _state);
			}
		};

		template <class Entity, class Path, class State, class Member>
		class member_action {
			Entity& _entity;
			State& _state;
		public:
			member_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				Action().MONETA_INTRA_TEMPLATE_KEYWORD operator()<Path, Member>(_entity, _state);
			}
		};

		template <class Actions, class Path, class Member, class Entity, class State>
		void process(Entity& entity, State& state);

		template <class Actions, class Entity, class Path, class State>
		class member_operator {
			Entity& _entity;
			State& _state;
		public:
			member_operator(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Member>
			void operator()(Member&) const {
				process<Actions, Path, Member>(_entity, _state);
			}
		};

		template <class Entity, class Path, class Member, class State>
		class container_member_operator {
			Entity& _entity;
			State& _state;
		public:
			container_member_operator(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				Action().MONETA_INTRA_TEMPLATE_KEYWORD operator()<Member, Path>(_entity, _state);
			}
		};

		struct traverse_enter {};
		struct traverse_member {};
		struct traverse_leave {};

		struct traverse_enter_container {};
		struct traverse_container_member {};
		struct traverse_leave_container {};

		//
		// Entity result_type
		//
		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::enable_if<
			boost::mpl::and_<
				boost::mpl::not_<traits::is_container<typename Member::result_type> >,
				traits::is_entity<typename Member::result_type>
			>
		>::type
		process(Entity& entity, State& state) {
			traverse<Actions, add_path<Path, Member>::type>(Member()(entity), state);
		}

		//
		// Non-Entity result_type
		//
		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::enable_if<
			boost::mpl::and_<
				boost::mpl::not_<traits::is_container<typename Member::result_type> >,
				boost::mpl::not_<traits::is_entity<typename Member::result_type> >
			>
		>::type
		process(Entity& entity, State& state) {
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_member>::type
			>(detail::member_action<Entity, Path, State, Member>(entity, state));
		}

		//
		// Container of Entity values
		//
		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::enable_if<
			boost::mpl::and_<
				traits::is_container<typename Member::result_type>,
				traits::is_entity<typename Member::result_type::value_type>
			>
		>::type
		process(Entity& entity, State& state) {
			typedef typename Member::result_type container_type;
			typedef typename container_type::value_type value_type;

			typedef typename add_path<Path, Member>::type new_path;

			// Enter actions
			//
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_enter_container>::type
			>(detail::container_enter_or_leave_action<Entity, new_path, Member, State>(entity, state));

			// Members
			//
			for (auto& entity_value : Member()(entity)) { // XXX: FIXME: Must be C++03!!
				traverse<Actions, new_path>(entity_value, state);
			}

			// Leave actions
			//
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_leave_container>::type
			>(detail::container_enter_or_leave_action<Entity, new_path, Member, State>(entity, state));
		}

		//
		// Container of Non-Entity values
		//
		template <class Actions, class Path, class Member, class Entity, class State>
		typename boost::enable_if<
			boost::mpl::and_<
				traits::is_container<typename Member::result_type>,
				boost::mpl::not_<typename traits::is_entity<typename Member::result_type::value_type> >
			>
		>::type
		process(Entity& entity, State& state) {
			typedef typename Member::result_type container_type;
			typedef typename container_type::value_type value_type;

			typedef typename add_path<Path, Member>::type new_path;

			// Enter actions
			//
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_enter_container>::type
			>(detail::container_enter_or_leave_action<Entity, new_path, Member, State>(entity, state));

			// Members
			//
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_container_member>::type
			>(detail::container_member_operator<Entity, new_path, Member, State>(entity, state));

			// Leave actions
			//
			boost::mpl::for_each<
				typename detail::actions_of<Actions, traverse_leave_container>::type
			>(detail::container_enter_or_leave_action<Entity, new_path, Member, State>(entity, state));
		}
	}

	template <class Actions, class Path, class Entity, class State>
	void traverse(Entity& entity, State& state) {
		// Enter actions
		//
		boost::mpl::for_each<
			typename detail::actions_of<Actions, detail::traverse_enter>::type
		>(detail::enter_or_leave_action<Entity, Path, State>(entity, state));

		// Members
		//
		boost::mpl::for_each<typename traits::members<Entity>::type>(
			detail::member_operator<Actions, Entity, Path, State>(entity, state)
		);

		// Leave actions
		//
		boost::mpl::for_each<
			typename detail::actions_of<Actions, detail::traverse_leave>::type
		>(detail::enter_or_leave_action<Entity, Path, State>(entity, state));
	}

}}
