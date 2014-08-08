#pragma once
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/detail/has_member_trait.hxx"

namespace moneta { namespace algorithm {

	namespace detail {

		struct no_state {};

		DEFINE_HAS_MEMBER_TRAIT(enter)

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::enable_if<has_member_enter<Traits> >::type
		call_enter_if_defined(Entity& entity, State& state = State()) {
			// XXX: Free functions as well, dammit...
			typename Traits::enter().operator()<
				Entity, Path
			>(entity);
		}

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::disable_if<has_member_enter<Traits> >::type
		call_enter_if_defined(Entity& entity, State& state = State()) {
		}

		// --------------------------------------------------------------------------------------------------

		DEFINE_HAS_MEMBER_TRAIT(leave)

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::enable_if<has_member_leave<Traits> >::type
		call_leave_if_defined(Entity& entity, State& state = State()) {
			// XXX: Free functions as well, dammit...
			typename Traits::leave().operator()<
				Entity, Path
			>(entity);
		}

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::disable_if<has_member_leave<Traits> >::type
		call_leave_if_defined(Entity& entity, State& state = State()) {
		}

		// --------------------------------------------------------------------------------------------------

		DEFINE_HAS_MEMBER_TRAIT(member)

		template <class Traits, class Path, class Member, class Entity, class State = no_state>
		typename boost::enable_if<has_member_member<Traits> >::type
		call_member_if_defined(Entity& entity, State& state = State()) {
			// XXX: Free functions as well, dammit...
			typename Traits::member().operator()<
				Entity,
				Member,
				Path
			>(entity);
		}

		template <class Traits, class Path, class Member, class Entity, class State = no_state>
		typename boost::disable_if<has_member_member<Traits> >::type
		call_member_if_defined(Entity& entity, State& state = State()) {
		}

		// --------------------------------------------------------------------------------------------------

		DEFINE_HAS_MEMBER_TRAIT(entity)

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::enable_if<has_member_entity<Traits> >::type
		call_entity_if_defined_or_iterate_members(Entity& entity, State& state = State()) {
			typename Traits::entity()(entity);
		}

		template <class Traits, class Path, class Entity, class State = no_state>
		typename boost::disable_if<has_member_entity<Traits> >::type
		call_entity_if_defined_or_iterate_members(Entity& entity, State& state = State()) {
			boost::mpl::for_each<typename traits::members<Entity>::type>(
				detail::member_operator<Traits, Entity, Path>(entity)
			);
		}

		// --------------------------------------------------------------------------------------------------

		template <class Path, class Member, class Enable = void>
		struct add_path : boost::mpl::identity<void> {};

		template <class Path, class Member>
		struct add_path<
			Path, Member,
			typename boost::enable_if<typename boost::mpl::is_sequence<Path>::type>::type
		> : boost::mpl::push_back<Path, Member> {};

		template <class Traits, class Path, class Member, class Entity, class State = no_state>
		typename boost::enable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(Entity& entity, State& state = State()) {
			traverse<Traits, add_path<Path, Member>::type>(Member()(entity));
		}

		template <class Traits, class Path, class Member, class Entity, class State = no_state>
		typename boost::disable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(Entity& entity, State& state = State()) {
			call_member_if_defined<Traits, Path, Member>(entity);
		}

		// --------------------------------------------------------------------------------------------------

		template <class Traits, class Entity, class Path>
		class member_operator {
			Entity& _entity;
		public:
			member_operator(Entity& entity)
			 : _entity(entity) {}

			template <typename Member>
			void operator()(Member&) const {
				call_member_or_recurse<Traits, Path, Member>(_entity);
			}
		};

	}

	template <class Traits, class Path = boost::mpl::vector0<>, class Entity = void>
	void traverse(Entity& entity) {
		detail::call_enter_if_defined<Traits, Path>(entity);
		detail::call_entity_if_defined_or_iterate_members<Traits, Path>(entity);
		detail::call_leave_if_defined<Traits, Path>(entity);
	}

	template <class Traits, class Path = boost::mpl::vector0<>, class Entity = void, class State>
	void stateful_traverse(Entity& entity, State& state) {
		detail::call_enter_if_defined<Traits, Path>(entity, state);
		detail::call_entity_if_defined_or_iterate_members<Traits, Path>(entity, state);
		detail::call_leave_if_defined<Traits, Path>(entity, state);
	}

}}
