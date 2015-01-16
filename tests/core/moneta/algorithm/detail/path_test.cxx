#include "stdafx.h"
#include <moneta/traits/member.hxx>
#include <moneta/algorithm/detail/path.hxx>
#include "../../model/SportsTeam.hxx"

#include <boost/mpl/print.hpp>

// TODO: Elaborate these tests.

inline void static_test() {
	using namespace moneta::algorithm::detail;

	//
	// container_member_count
	//
	BOOST_STATIC_ASSERT(
		1 == container_member_count<
			boost::mpl::vector1<
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players)
			>
		>::value
	);

	BOOST_STATIC_ASSERT(
		2 == container_member_count<
			boost::mpl::vector2<
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags)
			>
		>::value
	);

	BOOST_STATIC_ASSERT(
		2 == container_member_count<
			boost::mpl::vector3<
				MONETA_MEMBER(SportsTeam, std::string, Name),
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags)
			>
		>::value
	);

	BOOST_STATIC_ASSERT(
		4 == container_member_count<
			// Note: Surreal path.
			boost::mpl::vector7<
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags),
				MONETA_MEMBER(SportsTeam, std::string, Name),
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
				MONETA_MEMBER(SportsTeam, std::string, Name),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags),
				MONETA_MEMBER(SportsTeam, std::string, Name),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags)
			>
		>::value
	);


	//
	// is_container_member
	//
	BOOST_MPL_ASSERT((
		is_container_member<
			MONETA_MEMBER(SportsTeam, std::vector<Person>, Players)
		>
	));

	BOOST_MPL_ASSERT_NOT((
		is_container_member<
			MONETA_MEMBER(SportsTeam, std::string, Name)
		>
	));



	//
	// is_cwd_container_member
	//
	BOOST_MPL_ASSERT_NOT((
		is_cwd_container_member<
			boost::mpl::vector0<>
		>
	));

	BOOST_MPL_ASSERT((
		is_cwd_container_member<
			boost::mpl::vector1<
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players)
			>
		>
	));

	BOOST_MPL_ASSERT((
		is_cwd_container_member<
			boost::mpl::vector2<
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags)
			>
		>
	));

	BOOST_MPL_ASSERT_NOT((
		is_cwd_container_member<
			boost::mpl::vector3<
				MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
				MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags),
				MONETA_MEMBER(SportsTeam, std::string, Name)
			>
		>
	));
}
