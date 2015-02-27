#include "stdafx.h"
#include <moneta/codec/encoder.hxx>
#include <moneta/codec/_aux/io/ostringstream.hxx>
#include <moneta/codec/_aux/path_tabs.hxx>
#include <moneta/algorithm/detail/stringize_path.hxx>
#include "../model/tree/A.hxx"
#include "../model/Cat.hxx"
#include "../model/SportsTeam.hxx"
#include "../model/Customer.hxx"

using moneta::codec::aux::path_tabs;

struct testcodec_start_encoding {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path>()
			<< "S:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_enter_entity {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path>()
			<< "e:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_member {
	template <class Iterator, class Entity, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Member, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path, 1>()
			<< "m:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_present_member {
	template <class Iterator, class Entity, class Value, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, const Value&, State& state, Member, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path, 1>()
			<< "pm:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_leave_entity {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path>()
			<< "l:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_enter_container {
	template <class Iterator, class Entity, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Member, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path, -1>()
			<< "ec:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_container_item {
	template <class Iterator, class Entity, typename Value, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, Value& value, State& state, Member, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path>()
			<< "ci:" << value << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_leave_container {
	template <class Iterator, class Entity, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Member, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path, -1>()
			<< "lc:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

struct testcodec_finish_encoding {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, const Entity&, State& state, Path) const {
		return moneta::codec::io::make_ostringstream(begin, end)
			<< path_tabs<Path>()
			<< "F:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << ' ' << state++ << '\n'
		;
	}
};

// --------------------------------------------------------------------------------------------------------------------

using namespace moneta::codec;
typedef encoder<
	start_actions<testcodec_start_encoding>,
	enter_actions<testcodec_enter_entity>,
	present_member_actions<testcodec_present_member>,
	leave_actions<testcodec_leave_entity>,
	enter_container_actions<testcodec_enter_container>,
	container_item_actions<testcodec_container_item>,
	leave_container_actions<testcodec_leave_container>,
	finish_actions<testcodec_finish_encoding>
> encoder_t;

BOOST_AUTO_TEST_CASE(encoder_basic_test) {

	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const std::string expected =
		"S:A, 0\n"
		"e:A, 1\n"
		"\tpm:f, 2\n"
		"\tpm:g, 3\n"
		"\te:B,/A::b 4\n"
		"\t\te:C,/A::b/B::c 5\n"
		"\t\t\tpm:j,/A::b/B::c 6\n"
		"\t\t\tpm:k,/A::b/B::c 7\n"
		"\t\tl:C,/A::b/B::c 8\n"
		"\t\tpm:i,/A::b 9\n"
		"\t\te:D,/A::b/B::d 10\n"
		"\t\t\tpm:l,/A::b/B::d 11\n"
		"\t\t\te:E,/A::b/B::d/D::e 12\n"
		"\t\t\t\tpm:m,/A::b/B::d/D::e 13\n"
		"\t\t\t\tpm:n,/A::b/B::d/D::e 14\n"
		"\t\t\tl:E,/A::b/B::d/D::e 15\n"
		"\t\tl:D,/A::b/B::d 16\n"
		"\tl:B,/A::b 17\n"
		"\tpm:h, 18\n"
		"l:A, 19\n"
		"F:A, 20\n"
	;

	int level = 0;
	const int result = encoder_t()(std::begin(buffer), buffer + sizeof(buffer) - 1, A(), level);

	BOOST_CHECK_EQUAL(result, expected.size());
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

	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const std::string expected =
		"S:SportsTeam, 0\n"
		"e:SportsTeam, 1\n"
		"\tpm:Name, 2\n"
		"\tec:Players,/SportsTeam::Players 3\n"
		"\t\te:Person,/SportsTeam::Players 4\n"
		"\t\t\tpm:ID,/SportsTeam::Players 5\n"
		"\t\t\tpm:Name,/SportsTeam::Players 6\n"
		"\t\t\tpm:Height,/SportsTeam::Players 7\n"
		"\t\t\tpm:Fingers,/SportsTeam::Players 8\n"
		"\t\tl:Person,/SportsTeam::Players 9\n"
		"\t\te:Person,/SportsTeam::Players 10\n"
		"\t\t\tpm:ID,/SportsTeam::Players 11\n"
		"\t\t\tpm:Name,/SportsTeam::Players 12\n"
		"\t\t\tpm:Height,/SportsTeam::Players 13\n"
		"\t\t\tpm:Fingers,/SportsTeam::Players 14\n"
		"\t\tl:Person,/SportsTeam::Players 15\n"
		"\tlc:Players,/SportsTeam::Players 16\n"
		"\tec:Tags,/SportsTeam::Tags 17\n"
		"\t\tci:tag0,/SportsTeam::Tags 18\n"
		"\t\tci:tag1,/SportsTeam::Tags 19\n"
		"\t\tci:tag2,/SportsTeam::Tags 20\n"
		"\tlc:Tags,/SportsTeam::Tags 21\n"
		"l:SportsTeam, 22\n"
		"F:SportsTeam, 23\n"
	;

	int level = 0;
	const int result = encoder_t()(std::begin(buffer), buffer + sizeof(buffer) - 1, team, level);
	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}

BOOST_AUTO_TEST_CASE(test_moneta_codec_encoder_optionals) {

	typedef encoder<
		start_actions<testcodec_start_encoding>,
		enter_actions<testcodec_enter_entity>,
		member_actions<testcodec_member>,
		present_member_actions<testcodec_present_member>,
		leave_actions<testcodec_leave_entity>,
		enter_container_actions<testcodec_enter_container>,
		container_item_actions<testcodec_container_item>,
		leave_container_actions<testcodec_leave_container>,
		finish_actions<testcodec_finish_encoding>
	> encoder_t;

	char buffer[1024];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	const std::string expected =
		"S:Customer, 0\n"
		"e:Customer, 1\n"
		"\tm:Name, 2\n"
		"\tpm:Name, 3\n"
		"\tm:DOB, 4\n"
		"\tpm:DOB, 5\n"
		"\tm:Rating, 6\n"
		"\tpm:Rating, 7\n"
		"l:Customer, 8\n"
		"F:Customer, 9\n"
	;

	Customer customer;
	customer.Rating = 1;

	int level = 0;
	const int result = encoder_t()(std::begin(buffer), buffer + sizeof(buffer) - 1, customer, level);
	BOOST_CHECK_EQUAL(result, expected.size());
	BOOST_CHECK_EQUAL(expected, buffer);
}
