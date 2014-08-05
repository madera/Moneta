#pragma once
#include "../traits/member_names.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/detail/has_member_trait.hxx"

namespace moneta { namespace algorithm {

	namespace detail {

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(enter)

		template <class Traits, class Path, class Entity>
		typename boost::enable_if<has_member_enter<Traits> >::type
		call_enter_if_defined(Entity& entity) {
			typename Traits::enter()(entity);
		}

		template <class Traits, class Path, class Entity>
		typename boost::disable_if<has_member_enter<Traits> >::type
		call_enter_if_defined(Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(leave)

		template <class Traits, class Path, class Entity>
		typename boost::enable_if<has_member_leave<Traits> >::type
		call_leave_if_defined(Entity& entity) {
			typename Traits::leave()(entity);
		}

		template <class Traits, class Path, class Entity>
		typename boost::disable_if<has_member_leave<Traits> >::type
		call_leave_if_defined(Entity& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(member)

		template <class Traits, class Path, class Member>
		typename boost::enable_if<has_member_member<Traits> >::type
		call_member_if_defined(typename Member::class_type& entity) {
			// XXX: Free functions as well, dammit...
			typename Traits::member().operator()<
				typename Member::class_type,
				Member,
				Path
			>(entity);
		}

		template <class Traits, class Path, class Member>
		typename boost::disable_if<has_member_member<Traits> >::type
		call_member_if_defined(typename Member::class_type& entity) {
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		DEFINE_HAS_MEMBER_TRAIT(entity)

		template <class Traits, class Path, class Entity>
		typename boost::enable_if<has_member_entity<Traits> >::type
		call_entity_if_defined_or_iterate_members(Entity& entity) {
			typename Traits::entity()(entity);
		}

		template <class Traits, class Path, class Entity>
		typename boost::disable_if<has_member_entity<Traits> >::type
		call_entity_if_defined_or_iterate_members(Entity& entity) {
			boost::mpl::for_each<typename traits::members<Entity>::type>(
				detail::member_operator<Traits, Entity, Path>(entity)
			);
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

		template <class Traits, class Path, class Member>
		typename boost::enable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(typename Member::class_type& entity) {
			traverse<Traits, typename Member::result_type, Path>(
				Member()(entity)
			);
		}

		template <class Traits, class Path, class Member>
		typename boost::disable_if<traits::is_entity<typename Member::result_type> >::type
		call_member_or_recurse(typename Member::class_type& entity) {
			call_member_if_defined<Traits, Path, Member>(entity);
		}

		//
		// --------------------------------------------------------------------------------------------------
		//

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

	template <class Traits, class Entity, class Path = void>
	void traverse(Entity& entity) {
		detail::call_enter_if_defined<Traits, Path>(entity);
		detail::call_entity_if_defined_or_iterate_members<Traits, Path>(entity);
		detail::call_leave_if_defined<Traits, Path>(entity);
	}

}}
