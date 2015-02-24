#pragma once
#include "detail/path.hxx"
#include "../_aux/mplx_flatten.hxx"
#include "../traits/detail/is_functor_callable.hxx"
#include "../traits/is_entity.hxx"
#include "../traits/is_container.hxx"
#include "../traits/is_optional.hxx"

#include <boost/core/enable_if.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/spirit/home/support/container.hpp>

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

		// TODO: Export this useful function.
		template <class Path, class Member, class Enable = void>
		struct add_path : boost::mpl::identity<void> {};

		template <class Path, class Member>
		struct add_path<
			Path, Member,
			typename boost::enable_if<
				typename boost::mpl::is_sequence<Path>::type
			>::type
		> : boost::mpl::push_back<Path, Member> {};

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

		// XXX: TAG: Needs cleaning after done.
		template <class Entity, class State, class Member, class Path>
		class present_member_action { // XXX: Marked for review... it may not be needed.

			typedef typename boost::mpl::if_<
				boost::is_const<Entity>,
				typename boost::add_const<
					typename traits::optional_value_type<
						typename Member::result_type
					>::type
				>::type,
				typename traits::optional_value_type<
					typename Member::result_type
				>::type
			>::type value_type;

			struct value_getter {
				template <class E, class M>
				typename boost::enable_if<
					traits::is_optional<typename M::result_type>,
					value_type&
				>::type
				operator()(E& e, M) const {
					return *(M()(e));
				}

				template <class E, class M>
				typename boost::disable_if<
					traits::is_optional<typename M::result_type>,
					value_type&
				>::type
				operator()(E& e, M) const {
					return M()(e);
				}
			};

			struct is_present_member {
				template <class E, class M>
				typename boost::enable_if<
					traits::is_optional<typename M::result_type>,
					bool
				>::type
				operator()(E& e, M) const {
					return M()(e).is_initialized();
				}

				template <class E, class M>
				typename boost::disable_if<
					traits::is_optional<typename M::result_type>,
					bool
				>::type
				operator()(E&, M) const {
					return true;
				}
			};

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
				if (is_present_member()(_entity, Member())) {
					Action()(_entity, value_getter()(_entity, Member()), Member());
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
				if (is_present_member()(_entity, Member())) {
					Action()(_entity, value_getter()(_entity, Member()), Member(), Path());
				}
			}

			template <typename Action>
			typename boost::enable_if<
				moneta::traits::detail::is_functor_callable<
					Action,
					void (Entity&, State&, value_type&, Member, Path)
				>
			>::type
			process() const {
				if (is_present_member()(_entity, Member())) {
					Action()(_entity, _state, value_getter()(_entity, Member()), Member(), Path());
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

		template <class E, class C, class Enable = void>
		struct get_iterator : boost::mpl::if_<
			boost::is_const<E>,
			typename C::const_iterator,
			typename C::iterator
		> {};

		template <class E, class C>
		struct get_iterator<
			E,
			C,
			typename boost::enable_if<
				traits::is_optional<C>
			>::type
		> : boost::mpl::if_<
			boost::is_const<E>,
			typename C::value_type::const_iterator,
			typename C::value_type::iterator
		> {};

		template <class Traverser_, class Entity, class Path_, class State>
		class member_action_dispatcher {

			template <class Traverser, class Member, class Path>
			void iterate_members() const {
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

			template <class Member, class Path, class Operation>
			void iterate_container_member() const {
				typedef typename add_path<
					Path, Member
				>::type new_path;

				typedef detail::member_or_container_enter_leave_action<
					Entity, State, Member, new_path
				> action;

				using boost::mpl::for_each;
				for_each<typename Traverser_::enter_container_actions>(action(_entity, _state));

				//
				// TODO: Replace this code using Spirit's container_iterator or something.
				//
				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<
						typename Member::result_type
					>::type,
					typename Member::result_type
				>::type container_type;

				typedef typename boost::mpl::if_<
					boost::is_const<Entity>,
					typename boost::add_const<
						typename container_type::value_type
					>::type,
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
//					Operation()(_entity, _state, *itr, Member(), new_path());
				}

				typename boost::spirit::result_of::begin<container_type>::type itr = boost::spirit::traits::begin(*tags);
				typename boost::spirit::result_of::end  <v_t>::type end = boost::spirit::traits::end  (*tags);
				for ( ; itr != end; ++itr) {
					std::cout << "\t * " << *itr << std::endl;
					Operation()(_entity, _state, *itr, Member(), new_path());
				}


				for_each<typename Traverser_::leave_container_actions>(action(_entity, _state));
			}

			Entity& _entity;
			State& _state;

			//
			// Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<
						traits::is_optional<typename Member::result_type>
					>,
					boost::mpl::not_<
						traits::is_container<typename Member::result_type>
					>,
					traits::is_entity<typename Member::result_type>
				>
			>::type
			process() const {
				Traverser().template _traverse<
					typename add_path<Path, Member>::type
				>(Member()(_entity), _state);
			}

			//
			// Optional Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					boost::mpl::not_<
						traits::is_container<typename Member::result_type::value_type>
					>,
					traits::is_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				if (Member()(_entity).is_initialized()) {
					Traverser().template _traverse<
						typename add_path<Path, Member>::type
					>(Member()(_entity).get(), _state);
				}
			}

			//
			// Non-Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<
						traits::is_optional<typename Member::result_type>
					>,
					boost::mpl::not_<
						traits::is_container<typename Member::result_type>
					>,
					boost::mpl::not_<
						traits::is_entity<typename Member::result_type>
					>
				>
			>::type
			process() const {
				iterate_members<Traverser, Member, Path>();
			}

			//
			// Optional Non-Entity result_type
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					boost::mpl::not_<
						traits::is_container<typename Member::result_type::value_type>
					>,
					boost::mpl::not_<
						traits::is_entity<typename Member::result_type::value_type>
					>
				>
			>::type
			process() const {
				iterate_members<Traverser, Member, Path>();
			}

			// --------------------------------------------------------------------------------------------

			struct traverse_container_items {
				template <class E, class S, class V, class M, class P>
				typename boost::enable_if<
					traits::is_optional<V>
				>::type
				operator()(E&, S& state, V& value, M, P) const {
					Traverser_().template _traverse<P>(value.get(), state);
				}

				template <class E, class S, class V, class M, class P>
				typename boost::disable_if<
					traits::is_optional<V>
				>::type
				operator()(E&, S& state, V& value, M, P) const {
					Traverser_().template _traverse<P>(value, state);
				}
			};

			//
			// Container of Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<
						traits::is_optional<typename Member::result_type>
					>,
					traits::is_container<typename Member::result_type>,
					traits::is_entity<typename Member::result_type::value_type>
				>
			>::type
			process() const {
				iterate_container_member<Member, Path, traverse_container_items>();
			}

			//
			// Optional Container of Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type::value_type>,
					traits::is_entity<typename Member::result_type::value_type::value_type>
				>
			>::type
			process() const {
				iterate_container_member<Member, Path, traverse_container_items>();
			}

			// --------------------------------------------------------------------------------------------

			struct iterate_container_items {
				template <class E, class S, class V, class M, class P>
				void operator()(E& entity, S& state, V& value, M, P) const {
					boost::mpl::for_each<typename Traverser_::container_item_actions>(
						detail::container_item_action<
							E, V, S, M, P // XXX: Wrong order.
						>(value, entity, state)
					);
				}
			};

			//
			// Container of Non-Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					boost::mpl::not_<
						traits::is_optional<typename Member::result_type>
					>,
					traits::is_container<typename Member::result_type>,
					boost::mpl::not_<
						typename traits::is_entity<typename Member::result_type::value_type>
					>
				>
			>::type
			process() const {
				iterate_container_member<Member, Path, iterate_container_items>();
			}

			//
			// Optional Container of Non-Entity values
			//
			template <class Traverser, class Path, class Member>
			typename boost::enable_if<
				boost::mpl::and_<
					traits::is_optional<typename Member::result_type>,
					traits::is_container<typename Member::result_type::value_type>,
					boost::mpl::not_<
						typename traits::is_entity<
							typename Member::result_type::value_type::value_type
						>
					>
				>
			>::type
			process() const {
				iterate_container_member<Member, Path, iterate_container_items>();
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
			typedef detail::member_action_dispatcher<this_type, Entity, Path, State> member_action;

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
