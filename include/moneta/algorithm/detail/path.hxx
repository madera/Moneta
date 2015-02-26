#pragma once
#include "../../traits/is_container.hxx"
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/count_if.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/size.hpp>

namespace moneta { namespace algorithm {

	namespace detail {

		template <class Member>
		struct is_container_member : moneta::traits::is_container<
			typename Member::result_type
		> {};

		namespace yet_more_detail {
			template <class Path>
			struct is_path_back_container_member :  is_container_member<
				typename boost::mpl::back<Path>::type
			> {};
		}

		template <class Path>
		struct is_cwd_container_member : boost::mpl::if_<
			boost::mpl::empty<Path>,
			boost::false_type,
			yet_more_detail::is_path_back_container_member<Path>
		> {};

		template <class Path>
		struct container_member_count : boost::mpl::count_if<
			Path, is_container_member<boost::mpl::_1>
		> {};

		//

		template <class Path, class Member, class Enable = void>
		struct add_path : boost::mpl::identity<void> {};

		template <class Path, class Member>
		struct add_path<
			Path, Member,
			typename boost::enable_if<
				typename boost::mpl::is_sequence<Path>::type
			>::type
		> : boost::mpl::push_back<Path, Member> {};
	}

}}
