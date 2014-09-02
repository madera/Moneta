#pragma once

// MEQUEDE: This preprocessed version should be faster (theoretically) but it needs to be proven in a lab first.
//
// For now, only the iterative version will be used (costs O(n) every call).
//
// Tags: XXX: TODO: FIXME: PERF: Must explore!
//
#if 0
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

#else //////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace mplx {

	namespace detail {

		// XXX: Use the real speedy version: Boost.PP for C++03.
		//
		template <class Sequence, class Operation>
		struct super_slow_at_c_impl {
			int _target;
			int& _ordinal;
			Operation& _operation;

			super_slow_at_c_impl(int& state, const int target, Operation& operation)
			 : _ordinal(state), _target(target), _operation(operation) {
				_ordinal = 0;
			}
			
			template <class T>
			void operator()(const T&) const {
				if (_ordinal++ == _target) {
					_operation.operator()<T>();
				}
			}
		};

	}

	template <class Sequence, class Operation>
	void at_c(const int index, Operation& operation) {
		int state;
		boost::mpl::for_each<Sequence>(detail::super_slow_at_c_impl<Sequence, Operation>(state, index, operation));
	}

}

#endif
