//
// TODO: Rename this to traverser. Rename file to remove ng. Remove previous impl.
//

#pragma once
#include "detail/path.hxx"
#include "../_aux/mplx_flatten.hxx"
#include "../_aux/mplx_nullref.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/is_container.hxx"

#include <boost/core/enable_if.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>

#define MONETA_TRAVERSE_MAX_MEMBERS 10

#define MONETA_TRAVERSE_PARAMS \
	BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(MONETA_TRAVERSE_MAX_MEMBERS), T)

#define MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS \
	BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_DEC(MONETA_TRAVERSE_MAX_MEMBERS), class T, boost::mpl::na)

namespace moneta { namespace algorithm {

	namespace detail {
		struct traverse_enter {};
		struct traverse_member {};
		struct traverse_leave {};

		struct traverse_enter_container {};
		struct traverse_container_item {};
		struct traverse_leave_container {};

		struct no_state {};

		template <class T>
		struct get_mpl_vector : boost::mpl::identity<
			typename T::mpl_vector
		> {};

		template <class Actions, class Base>
		struct actions_of : mplx::flatten<
			typename boost::mpl::copy_if<
				Actions,
				boost::is_base_of<Base, boost::mpl::_1>,
				boost::mpl::inserter<
					boost::mpl::vector0<>,
					boost::mpl::push_back<
						boost::mpl::_1,
						get_mpl_vector<boost::mpl::_2>
					>
				>
			>::type
		> {};

		template <class Path, class Member, class Enable = void>
		struct add_path : boost::mpl::identity<void> {};

		template <class Path, class Member>
		struct add_path<
			Path, Member,
			typename boost::enable_if<typename boost::mpl::is_sequence<Path>::type>::type
		> : boost::mpl::push_back<Path, Member> {};

		template <class Entity, class Path, class State>
		class entity_enter_or_leave_action {
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Entity&)>
			>::type
			process() const {
				Action()(_entity);
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Entity&, Path&)>
			>::type
			process() const {
				Action()(_entity, mplx::nullref<Path>());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Entity&, Path&, State&)>
			>::type
			process() const {
				Action()(_entity, mplx::nullref<Path>(), _state);
			}
			
		public:
			entity_enter_or_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Member, class Entity, class Path, class State>
		class container_enter_or_leave_action {
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity);
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&, const Path&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity, mplx::nullref<Path>());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&, const Path&, State&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity, mplx::nullref<Path>(), _state);
			}

		public:
			container_enter_or_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Value, class Member, class Entity, class Path, class State>
		class container_item_action {
			Value& _value;
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Value&, Member&, Entity&)>
			>::type
			process() const {
				Action()(_value, mplx::nullref<Member>(), _entity);
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Value&, Member&, Entity&, const Path&)>
			>::type
			process() const {
				Action()(_value, mplx::nullref<Member>(), _entity, mplx::nullref<Path>());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Value&, Member&, Entity&, const Path&, State&)>
			>::type
			process() const {
				Action()(_value, mplx::nullref<Member>(), _entity, mplx::nullref<Path>(), _state);
			}

		public:
			container_item_action(Value& value, Entity& entity, State& state)
			 : _value(value), _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Member, class Entity, class Path, class State>
		class member_action {
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity);
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&, const Path&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity, mplx::nullref<Path>());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<Action, void (Member&, Entity&, const Path&, State&)>
			>::type
			process() const {
				Action()(mplx::nullref<Member>(), _entity, mplx::nullref<Path>(), _state);
			}
			
		public:
			member_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Traverser_, class Entity, class Path_, class State>
		class member_action_dispatcher {
			Entity& _entity;
			State& _state;

			//
			// Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<traits::is_container<typename Member::result_type> >,
					traits::is_entity<typename Member::result_type>
				>
			>::type
			process() const {
				Traverser().template _traverse<
					typename add_path<Path, Member>::type
				>(mplx::nullref<Member>()(_entity), _state);
			}

			//
			// Non-Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<traits::is_container<typename Member::result_type> >,
					boost::mpl::not_<traits::is_entity<typename Member::result_type> >
				>
			>::type
			process() const {
				boost::mpl::for_each<typename Traverser::member_actions>(
					detail::member_action<Member, Entity, Path, State>(_entity, _state)
				);
			}

			//
			// Container of Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_container<typename Member::result_type>,
					traits::is_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				typedef typename add_path<Path, Member>::type path;
				typedef detail::container_enter_or_leave_action<Member, Entity, path, State> action;

				using boost::mpl::for_each;
				for_each<typename Traverser::enter_container_actions>(action(_entity, _state));

				//
				// TODO: Replace this code using Spirit's container_iterator or something.
				//
				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<typename Member::result_type>::type,
					typename Member::result_type
				>::type container_type;

				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename container_type::const_iterator,
					typename container_type::iterator
				>::type iterator_type;

				container_type& container = Member()(_entity);
				iterator_type itr = container.begin();
				for ( ; itr != container.end(); ++itr) {
					Traverser().template _traverse<path>(*itr, _state);
				}

				for_each<typename Traverser::leave_container_actions>(action(_entity, _state));
			}

			//
			// Container of Non-Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_container<typename Member::result_type>,
					boost::mpl::not_<typename traits::is_entity<typename Member::result_type::value_type> >
				>
			>::type
			process() const {
				typedef detail::container_enter_or_leave_action<
					Member,
					Entity,
					typename add_path<Path, Member>::type,
					State
				> action;

				using boost::mpl::for_each;
				for_each<typename Traverser::enter_container_actions>(action(_entity, _state));

				//
				// TODO: Replace this code using Spirit's container_iterator or something.
				//
				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<typename Member::result_type>::type,
					typename Member::result_type
				>::type container_type;

				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<typename container_type::value_type>::type,
					typename container_type::value_type
				>::type value_type;

				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename container_type::const_iterator,
					typename container_type::iterator
				>::type iterator_type;

				container_type& container = Member()(_entity);
				iterator_type itr = container.begin();
				for ( ; itr != container.end(); ++itr) {
					value_type& value = *itr;
					boost::mpl::for_each<typename Traverser::container_item_actions>(
						detail::container_item_action<
							value_type,
							Member,
							Entity,
							typename add_path<Path, Member>::type,
							State
						>(value, _entity, _state)
					);
				}
				
				for_each<typename Traverser::leave_container_actions>(action(_entity, _state));
			}

		public:
			member_action_dispatcher(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Member>
			void operator()(Member&) const {
				process<Traverser_, Path_, Member>();
			}
		};
	}

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct enter_actions : detail::traverse_enter {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct member_actions : detail::traverse_member {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct leave_actions : detail::traverse_leave {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct enter_container_actions : detail::traverse_enter_container {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct container_item_actions : detail::traverse_container_item {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct leave_container_actions : detail::traverse_leave_container {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct traverse {
		typedef traverse this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_enter >::type enter_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_member>::type member_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_leave >::type leave_actions;

		typedef typename detail::actions_of<mpl_vector, detail::traverse_enter_container >::type enter_container_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_container_item  >::type container_item_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_leave_container >::type leave_container_actions;

		template <class Path, class Entity, class State>
		void _traverse(Entity& entity, State& state) const {
			typedef detail::entity_enter_or_leave_action<Entity, Path, State> enter_leave_action;
			typedef detail::member_action_dispatcher<this_type, Entity, Path, State> member_action;

			using boost::mpl::for_each;
			for_each<enter_actions>(enter_leave_action(entity, state));
			for_each<typename traits::members<Entity>::type>(member_action(entity, state));
			for_each<leave_actions>(enter_leave_action(entity, state));
		}

	public:
		template <class Entity>
		void operator()(Entity& entity) const {
			_traverse<boost::mpl::vector0<> >(entity, detail::no_state());
		}

		template <class Entity, class State>
		void operator()(Entity& entity, State& state) const {
			_traverse<boost::mpl::vector0<> >(entity, state);
		}
	};
}}
