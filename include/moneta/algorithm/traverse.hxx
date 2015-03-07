#pragma once
#include "detail/path.hxx"
#include "../_aux/mplx_flatten.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/is_container.hxx"
#include "../traits/optional.hxx"
#include "../traits/container.hxx"
#include <boost/core/enable_if.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/mpl/for_each.hpp>

#define MONETA_TRAVERSE_MAX_MEMBERS 10

#define MONETA_TRAVERSE_PARAMS \
	BOOST_PP_ENUM_PARAMS(BOOST_PP_DEC(MONETA_TRAVERSE_MAX_MEMBERS), T)

#define MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS \
	BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(BOOST_PP_DEC(MONETA_TRAVERSE_MAX_MEMBERS), class T, boost::mpl::na)

namespace moneta { namespace algorithm {

	namespace detail {

		struct traverse_start {};

		struct traverse_enter {};
		struct traverse_member {};
		struct traverse_present_member {};
		struct traverse_leave {};

		struct traverse_enter_container {};
		struct traverse_container_item {};
		struct traverse_leave_container {};

		struct traverse_finish {};

		struct no_state {};

		//

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

		template <class Entity, class State, class Path>
		class start_finish_enter_leave_action {
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&)
				>
			>::type
			process() const {
				Action()(_entity);
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Path)
				>
			>::type
			process() const {
				Action()(_entity, Path());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, State&, Path)
				>
			>::type
			process() const {
				Action()(_entity, _state, Path());
			}
			
		public:
			start_finish_enter_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Entity, class State, class Member, class Path>
		class member_or_container_enter_leave_action {
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Member)
				>
			>::type
			process() const {
				Action()(_entity, Member());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Member, Path)
				>
			>::type
			process() const {
				Action()(_entity, Member(), Path());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, State&, Member, Path)
				>
			>::type
			process() const {
				Action()(_entity, _state, Member(), Path());
			}

		public:
			member_or_container_enter_leave_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Entity, class State, class Member, class Path>
		class present_member_action {

			typedef typename traits::detail::const_if_const<
				Entity,
				typename traits::optional_value_type<
					typename Member::result_type
				>::type
			>::type value_type;

			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, value_type&, Member)
				>
			>::type
			process() const {
				if (traits::is_optional_present(Member()(_entity))) {
					Action()(_entity, traits::get_optional_value(Member()(_entity)), Member());
				}
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, value_type&, Member, Path)
				>
			>::type
			process() const {
				if (traits::is_optional_present(Member()(_entity))) {
					Action()(_entity, traits::get_optional_value(Member()(_entity)), Member(), Path());
				}
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, value_type&, State&, Member, Path)
				>
			>::type
			process() const {
				if (traits::is_optional_present(Member()(_entity))) {
					Action()(_entity, traits::get_optional_value(Member()(_entity)), _state, Member(), Path());
				}
			}

		public:
			present_member_action(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Entity, typename Value, class State, class Member, class Path>
		class container_item_action {
			Value& _value;
			Entity& _entity;
			State& _state;

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Value&, Member)
				>
			>::type
			process() const {
				Action()(_entity, _value, Member());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Value&, Member, Path)
				>
			>::type
			process() const {
				Action()(_entity, _value, Member(), Path());
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, Value&, State&, Member, Path)
				>
			>::type
			process() const {
				Action()(_entity, _value, _state, Member(), Path());
			}

		public:
			container_item_action(Value& value, Entity& entity, State& state)
			 : _value(value), _entity(entity), _state(state) {}

			template <typename Action>
			void operator()(Action&) const {
				process<Action>();
			}
		};

		template <class Traverser_, class Entity, class State, class Path_>
		class member_action_dispatcher {

			template <class Path>
			struct traverse_item {
				State& _state;

				traverse_item(State& state)
				 : _state(state) {}

				template <class Value>
				void operator()(Value& value) const {
					Traverser_().template _traverse<Path>(value, _state);
				}
			};

			template <class Member, class Path>
			struct iterate_value_items {
				Entity& _entity;
				State& _state;

				iterate_value_items(Entity& entity, State& state)
				 : _entity(entity), _state(state) {}

				template <class Value>
				void operator()(Value& value) const {
					boost::mpl::for_each<typename Traverser_::container_item_actions>(
						detail::container_item_action<
							Entity, Value, State, Member, Path // XXX: Wrong order.
						>(value, _entity, _state)
					);
				}
			};

			template <class Traverser, class Member, class Path>
			void entity_member() const {
				if (!traits::is_optional_present(Member()(_entity))) {
					return;
				}

				Traverser().template _traverse<
					typename detail::add_path<Path, Member>::type
				>(traits::get_optional_value(Member()(_entity)), _state);
			}

			template <class Traverser, class Member, class Path>
			void nonentity_member() const {
				boost::mpl::for_each<typename Traverser::member_actions>(
					detail::member_or_container_enter_leave_action<
						Entity, State, Member, Path	
					>(_entity, _state)
				);

				boost::mpl::for_each<typename Traverser::present_member_actions>(
					detail::present_member_action<
						Entity, State, Member, Path
					>(_entity, _state)
				);
			}

			template <class Traverser, class Member, class Path>
			void container_of_entities() const {
				if (!traits::is_optional_present(Member()(_entity))) {
					return;
				}

				typedef typename detail::add_path<Path, Member>::type path;
				detail::member_or_container_enter_leave_action<
					Entity, State, Member, path
				> action(_entity, _state);

				boost::mpl::for_each<typename Traverser::enter_container_actions>(action);
				traits::container_for_each(Member()(_entity), traverse_item<path>(_state));
				boost::mpl::for_each<typename Traverser::leave_container_actions>(action);
			}

			template <class Traverser, class Member, class Path>
			void container_of_nonentities() const {
				if (!traits::is_optional_present(Member()(_entity))) {
					return;
				}

				typedef typename detail::add_path<Path, Member>::type path;
				detail::member_or_container_enter_leave_action<
					Entity, State, Member, path
				> action(_entity, _state);

				boost::mpl::for_each<typename Traverser::enter_container_actions>(action);
				traits::container_for_each(
					Member()(_entity), iterate_value_items<Member, path>(_entity, _state)
				);
				boost::mpl::for_each<typename Traverser::leave_container_actions>(action);
			}

			Entity& _entity;
			State& _state;

			//
	// 01 //	// Entity result_type
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::isnt_optional<typename Member::result_type>,
					traits::isnt_container<typename Member::result_type>,
					traits::is_entity<typename Member::result_type>
				>
			>::type
			process() const {
				entity_member<Traverser, Member, Path>();
			}

			//
	// 02 //	// Optional Entity result_type
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::isnt_container<typename Member::result_type::value_type>,
					traits::is_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				entity_member<Traverser, Member, Path>();
			}

			//
	// 03 //	// Non-Entity result_type
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::isnt_optional<typename Member::result_type>,
					traits::isnt_container<typename Member::result_type>,
					traits::isnt_entity<typename Member::result_type>
				>
			>::type
			process() const {
				nonentity_member<Traverser, Member, Path>();
			}

			//
	// 04 //	// Optional Non-Entity result_type
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::isnt_container<typename Member::result_type::value_type>,
					traits::isnt_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				nonentity_member<Traverser, Member, Path>();
			}

			//
	// 05 //	// Container of Entity values
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::isnt_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type>,
					traits::is_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				container_of_entities<Traverser, Member, Path>();
			}

			//
	// 06 //	// Optional Container of Entity values
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type::value_type>,
					traits::is_entity<
						typename boost::mpl::identity<
							typename Member::result_type::value_type
						>::type::value_type
					>
				>
			>::type
			process() const {
				container_of_entities<Traverser, Member, Path>();
			}

			//
	// 07 //	// Container of Non-Entity values
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::isnt_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type>,
					traits::isnt_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				container_of_nonentities<Traverser, Member, Path>();
			}

			//
	// 08 //	// Optional Container of Non-Entity values
			//
			template <class Traverser, class Member, class Path>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type::value_type>,
					traits::isnt_entity<
						typename boost::mpl::identity<
							typename Member::result_type::value_type
						>::type::value_type
					>
				>
			>::type
			process() const {
				container_of_nonentities<Traverser, Member, Path>();
			}

		public:
			member_action_dispatcher(Entity& entity, State& state)
			 : _entity(entity), _state(state) {}

			template <typename Member>
			void operator()(Member&) const {
				process<Traverser_, Member, Path_>();
			}
		};
	}

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct start_actions : detail::traverse_start {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct enter_actions : detail::traverse_enter {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct member_actions : detail::traverse_member {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct present_member_actions : detail::traverse_present_member {
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
	struct finish_actions : detail::traverse_finish {
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
	};

	template <class T, MONETA_TRAVERSE_PARAMS_WITH_DEFAULTS>
	struct traverse {
		typedef traverse this_type;
		
		typedef boost::mpl::vector<T, MONETA_TRAVERSE_PARAMS> mpl_vector;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_start >::type start_actions;

		typedef typename detail::actions_of<mpl_vector, detail::traverse_enter >::type enter_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_member>::type member_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_present_member>::type present_member_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_leave >::type leave_actions;

		typedef typename detail::actions_of<mpl_vector, detail::traverse_enter_container>::type enter_container_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_container_item >::type container_item_actions;
		typedef typename detail::actions_of<mpl_vector, detail::traverse_leave_container>::type leave_container_actions;

		typedef typename detail::actions_of<mpl_vector, detail::traverse_finish>::type finish_actions;

		template <class Path, class Entity, class State>
		void _traverse(Entity& entity, State& state) const {
			typedef detail::start_finish_enter_leave_action<Entity, State, Path> enter_leave_action;
			typedef detail::member_action_dispatcher<this_type, Entity, State, Path> member_action;

			using boost::mpl::for_each;
			for_each<enter_actions>(enter_leave_action(entity, state));
			for_each<typename traits::members<Entity>::type>(member_action(entity, state));
			for_each<leave_actions>(enter_leave_action(entity, state));
		}

		template <class Path, class Entity, class State>
		void begin_traverse(Entity& entity, State& state) const {
			typedef detail::start_finish_enter_leave_action<Entity, State, Path> enter_leave_action;
			boost::mpl::for_each<start_actions>(enter_leave_action(entity, state));
			_traverse<Path>(entity, state);
			boost::mpl::for_each<finish_actions>(enter_leave_action(entity, state));
		}

	public:
		template <class Entity>
		void operator()(Entity& entity) const {
			begin_traverse<boost::mpl::vector0<> >(entity, detail::no_state());
		}

		template <class Entity, class State>
		void operator()(Entity& entity, State& state) const {
			begin_traverse<boost::mpl::vector0<> >(entity, state);
		}
	};
}}
