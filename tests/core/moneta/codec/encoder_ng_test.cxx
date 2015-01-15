#include "stdafx.h"
#include <moneta/codec/encoder_ng.hxx>
#include <moneta/codec/codec_io.hxx>
#include <moneta/codec/debug_dump/stringize_path.hxx>
#include "../model/tree/A.hxx"
#include "../model/Cat.hxx"
#include "../model/SportsTeam.hxx"

template <class Path>
static const char* tabs() {
	return moneta::codec::detail::tabs<
		boost::mpl::size<Path>::value + moneta::algorithm::detail::container_member_count<Path>::value
	>::get();
}

struct testcodec_enter_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< tabs<Path>()
			<< "e:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_member {
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, const Member&, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< tabs<Path>()
			<< "m:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_leave_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< tabs<Path>()
			<< "l:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_enter_container {
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< moneta::codec::detail::tabs<boost::mpl::size<Path>::value>::get()
			<< "ec:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_container_member {
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< tabs<Path>()
			<< "cm:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_leave_container {
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< moneta::codec::detail::tabs<boost::mpl::size<Path>::value>::get()
			<< "lc:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

BOOST_AUTO_TEST_CASE(encoder_basic_test) {
	using namespace moneta::codec;

	typedef encoder<
		enter_actions<testcodec_enter_entity>,
		member_actions<testcodec_member>,
		leave_actions<testcodec_leave_entity>,
		enter_container_actions<testcodec_enter_container>,
		container_member_actions<testcodec_container_member>,
		leave_container_actions<testcodec_leave_container>
	> encoder_t;

	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	int level = 0;
	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, A(), level);

	const std::string expected =
		"e:A, 0\n"
		"m:f, 1\n"
		"m:g, 2\n"
		"\te:B,/A::b 3\n"
		"\t\te:C,/A::b/B::c 4\n"
		"\t\tm:j,/A::b/B::c 5\n"
		"\t\tm:k,/A::b/B::c 6\n"
		"\t\tl:C,/A::b/B::c 7\n"
		"\tm:i,/A::b 8\n"
		"\t\te:D,/A::b/B::d 9\n"
		"\t\tm:l,/A::b/B::d 10\n"
		"\t\t\te:E,/A::b/B::d/D::e 11\n"
		"\t\t\tm:m,/A::b/B::d/D::e 12\n"
		"\t\t\tm:n,/A::b/B::d/D::e 13\n"
		"\t\t\tl:E,/A::b/B::d/D::e 14\n"
		"\t\tl:D,/A::b/B::d 15\n"
		"\tl:B,/A::b 16\n"
		"m:h, 17\n"
		"l:A, 18\n"
	;

	BOOST_CHECK_EQUAL(result, 316);
	BOOST_CHECK_EQUAL(expected, buffer);
}

BOOST_AUTO_TEST_CASE(traversal_encoder_test) {

	Person jordan;
	jordan.Name = "Michael Jordan";
	jordan.Height = 2.00;
	
	Person johnson;
	johnson.Name = "Magic Johnson";
	johnson.Height = 2.10;

	SportsTeam team;
	team.Players.push_back(jordan);
	team.Players.push_back(johnson);

	team.Tags.push_back("tag0");
	team.Tags.push_back("tag1");
	team.Tags.push_back("tag2");

	using namespace moneta::codec;

	typedef encoder<
		enter_actions<testcodec_enter_entity>,
		member_actions<testcodec_member>,
		leave_actions<testcodec_leave_entity>,
		enter_container_actions<testcodec_enter_container>,
		container_member_actions<testcodec_container_member>,
		leave_container_actions<testcodec_leave_container>
	> encoder_t;

	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	int level = 0;
	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, team, level);

	const std::string expected =
		"e:SportsTeam, 0\n"
		"m:Name, 1\n"
		"\tec:Players,/SportsTeam::Players 2\n"
		"\t\te:Person,/SportsTeam::Players 3\n"
		"\t\tm:ID,/SportsTeam::Players 4\n"
		"\t\tm:Name,/SportsTeam::Players 5\n"
		"\t\tm:Height,/SportsTeam::Players 6\n"
		"\t\tm:Fingers,/SportsTeam::Players 7\n"
		"\t\tl:Person,/SportsTeam::Players 8\n"
		"\t\te:Person,/SportsTeam::Players 9\n"
		"\t\tm:ID,/SportsTeam::Players 10\n"
		"\t\tm:Name,/SportsTeam::Players 11\n"
		"\t\tm:Height,/SportsTeam::Players 12\n"
		"\t\tm:Fingers,/SportsTeam::Players 13\n"
		"\t\tl:Person,/SportsTeam::Players 14\n"
		"\tlc:Players,/SportsTeam::Players 15\n"
		"\tec:Tags,/SportsTeam::Tags 16\n"
		"\t\tcm:Tags,/SportsTeam::Tags 17\n"
		"\tlc:Tags,/SportsTeam::Tags 18\n"
		"l:SportsTeam, 19\n"
;
	BOOST_CHECK_EQUAL(result, 608);
	BOOST_CHECK_EQUAL(expected, buffer);
}
