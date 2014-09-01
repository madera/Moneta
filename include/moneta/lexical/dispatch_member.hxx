#pragma once
#include "../limits/member.hxx"
#include "../traits/member_names.hxx"
#include <string>

#include "../algorithm/for_each_member.hxx"

namespace moneta { namespace lexical {

	namespace detail {

		// Offers fast dispatch when compared to for_each_member() loops seeking for a member name.
		//
		template <class Entity, int Size = boost::mpl::size<traits::members<Entity>::type>::value>
		struct dispatch_member_impl;

		template <class Entity>
		struct dispatch_member_impl<Entity, 0> {
			template <class Operation>
			void operator()(const int index, Operation& operation) const {
			}
		};

		template <class Entity>
		struct dispatch_member_impl<Entity, 1> {
			template <class Operation>
			void operator()(const int index, Operation& operation) const {
				using boost::mpl::at_c;
				if (false) {}
				else if (index == 0)
					operation.operator()<typename at_c<typename traits::members<Entity>::type, 0>::type>(); // XXX
			}
		};

		template <class Entity>
		struct dispatch_member_impl<Entity, 4> {
			template <class Operation>
			void operator()(const int index, Operation& operation) const {
				using boost::mpl::at_c;
				if (false) {}
				else if (index == 0)
					operation.operator()<typename at_c<typename traits::members<Entity>::type, 0>::type>(); // XXX
				else if (index == 1)
					operation.operator()<typename at_c<typename traits::members<Entity>::type, 1>::type>(); // XXX
				else if (index == 2)
					operation.operator()<typename at_c<typename traits::members<Entity>::type, 2>::type>(); // XXX
				else if (index == 3)
					operation.operator()<typename at_c<typename traits::members<Entity>::type, 3>::type>(); // XXX
			}
		};
	}

	template <class Entity, class Operation>
	void dispatch_member(const std::string& name, Operation& operation) {
		detail::dispatch_member_impl<Entity>()(moneta::traits::get_member_name_index<Entity>(name), operation);
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Slow version.
	//
	// TODO: Experiment to see if compiler finds a way to use this. It's cleaner, but (theoretically) slower.
	//
	namespace detail {

		template <class Operation>
		struct dispatch_member2_op {
			const std::string _target;
			Operation& _operation;

			dispatch_member2_op(const std::string target, Operation& operation)
			 : _target(target), _operation(operation) {}

			template <class Entity, class Member, class Path>
			void operator()(Entity& entity) const {
				if (traits::detail::member_name<Member>::get() == _target) {
					_operation.operator()<Member>();
				}
			}
		};

	}

	template <class Entity, class Operation>
	void dispatch_member2(const std::string& name, Operation& operation) {
		algorithm::for_each_member(Entity(), detail::dispatch_member2_op<Operation>(name, operation));
	}

}}
