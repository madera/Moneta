#pragma once
#include "../../traits/is_container.hxx"
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/back.hpp>

namespace moneta { namespace algorithm {

	namespace detail {

		template <class Member>
		struct is_container_member : moneta::traits::is_container<
			typename Member::result_type
		> {};

		template <class Path>
		struct is_cwd_container_member : boost::mpl::if_<
			boost::mpl::empty<Path>,
			boost::false_type,
			typename boost::mpl::apply<
				is_container_member<boost::mpl::_1>,
				typename boost::mpl::back<Path>::type
			>::type
		> {};

		template <class Path>
		struct container_member_count : boost::mpl::count_if<
			Path, is_container_member<boost::mpl::_1>
		> {};

	}

}}
