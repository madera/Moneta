// XXX: Move this file away from traits. Maybe root?

#pragma once
#include "../container/detail/seek_entity_types.hxx"
#include "../limits/member.hxx"
#include "../traits/tuple.hxx"
#include "../traits/is_entity.hxx"
#include "../_aux/mplx_flatten.hxx"
#include "../_aux/mplx_remove_duplicates.hxx"
#include <boost/mpl/vector.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/push_back.hpp>

namespace moneta { namespace traits {

	namespace detail {

		template <class T>
		struct get_template_args : boost::mpl::identity<
			typename T::args
		> {};

		template <class Sequence>
		struct vectorize_entity_groups : boost::mpl::fold<
			Sequence,
			boost::mpl::vector0<>,
			boost::mpl::push_back<
				boost::mpl::_1,
				boost::mpl::if_<
					traits::is_entity<boost::mpl::_2>,
					boost::mpl::_2,
					moneta::traits::detail::vectorize_entity_groups<
						get_template_args<boost::mpl::_2>
					>
				>
			>
		> {};

		using moneta::container::detail::seek_entity_types;
	}

	template <BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(MONETA_MAX_ENTITY_GROUP_ELEMENTS, class E, boost::mpl::na)>
	struct entity_group {
		typedef boost::mpl::vector<
			BOOST_PP_ENUM_PARAMS(MONETA_MAX_ENTITY_GROUP_ELEMENTS, E)
		> args;

		// entity_group<>::top contains all the top entities. That is, all the entities of all groups passed.
		//
		typedef typename mplx::remove_duplicates<
			// (2) Linearizes the sequence to get rid of mpl::vector<> elements.
			typename mplx::flatten<
				// (1) Obtains sequence of composite Entity or mpl::vector<> elements.
				typename detail::vectorize_entity_groups<
					args
				>::type
			>::type
		>::type top;

		// entity_group<>::all contains all the top entities and their entity member dependencies.
		//
		typedef typename mplx::remove_duplicates<
			// Obtains a sequence of all ::top Entity member dependencies.
			typename boost::mpl::fold<
				top,
				boost::mpl::vector0<>,
				boost::mpl::copy<
					detail::seek_entity_types<boost::mpl::_2>,
					boost::mpl::back_inserter<boost::mpl::_1>
				>
			>::type
		>::type all;
	};

}}
