#pragma once
#include "../limits/member.hxx"
#include "../traits/member_names.hxx"
#include "../_aux/mplx_at_c.hxx"

namespace moneta { namespace lexical {

	namespace detail {
	
		template <class Operation>
		struct dispatch_entity_impl {

			struct state {
				bool done;
				std::string name;
				Operation& operation;

				state(const std::string& name_, Operation& operation_)
				 : name(name_), operation(operation_), done(false) {}
			};

			state& _state;

			dispatch_entity_impl(state& state_)
			 : _state(state_) {}

			template <class Entity>
			void operator()(const Entity&) const {
				if (!_state.done && _state.name == moneta::traits::detail::entity_name<Entity>::get()) {
					_state.operation.operator()<Entity>(); // XXX
				}
			}
		};

	}

	template <class Entities, class Operation>
	void dispatch_entity(const std::string& name, Operation& operation) {
		detail::dispatch_entity_impl<Operation>::state state(name, operation);
		boost::mpl::for_each<Entities>(detail::dispatch_entity_impl<Operation>(state));
	}

}}
