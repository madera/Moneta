#pragma once
#include "optional.hxx"

namespace moneta { namespace traits {

	namespace detail {

		template <class Test, class Type>
		struct const_if_const : boost::mpl::if_<
			boost::is_const<Test>,
			typename boost::add_const<Type>::type,
			Type
		> {};

		template <class Container, class Enable = void>
		struct container_iterator : boost::mpl::identity<
			typename Container::iterator
		> {};

		template <class Container>
		struct container_iterator<
			Container,
			typename boost::enable_if<
				boost::is_const<Container>
			>::type
		> : boost::mpl::identity<
			typename Container::const_iterator
		> {};

		template <class Container, class Operation>
		struct for_each_impl {

		};

	}

	template <class Container, class Operation>
	void container_for_each(Container& container, const Operation& operation) {
		if (!traits::is_optional_present(container)) {
			return;
		}

		typedef typename detail::const_if_const<
			Container,
			typename traits::optional_value_type<Container>::type
		>::type container_type;
		container_type& kontainer = traits::get_optional_value(container);

		typedef typename detail::container_iterator<
			container_type
		>::type iterator_type;
		iterator_type itr = kontainer.begin();

		for ( ; itr != kontainer.end(); ++itr) {
			operation(*itr);
		}
	}

}}
