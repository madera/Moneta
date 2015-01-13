#include "stdafx.h"
#include <moneta/codec/encoder_ng.hxx>
#include <moneta/codec/codec_io.hxx>
#include <moneta/codec/debug_dump/stringize_path.hxx>
#include "../model/tree/A.hxx"

struct testcodec_enter_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< moneta::codec::detail::tabs<boost::mpl::size<Path>::value>::get()
			<< "e:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << '\n'
		;
	}
};

struct testcodec_leave_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< moneta::codec::detail::tabs<boost::mpl::size<Path>::value>::get()
			<< "l:" << moneta::traits::get_entity_name<Entity>() << ','
			<< moneta::codec::detail::stringize_path<Path>() << '\n'
		;
	}
};

struct testcodec_member {
	template <class Member, class Iterator, class Entity, class Path, class State>
	int operator()(const Member&, Iterator begin, Iterator end, const Entity& entity, const Path&, State& state) const {
		return moneta::codec::detail::make_ostringstream(begin, end)
			<< moneta::codec::detail::tabs<boost::mpl::size<Path>::value>::get()
			<< "m:" << moneta::traits::detail::member_name<Member>::get() << ','
			<< moneta::codec::detail::stringize_path<Path>() << '\n'
		;
	}
};

BOOST_AUTO_TEST_CASE(encoder_basic_test) {
	using namespace moneta::codec;

	typedef encoder<
		enter_actions<testcodec_enter_entity>,
		member_actions<testcodec_member>,
		leave_actions<testcodec_leave_entity>
	> encoder_t;

	char buffer[128];
	std::fill(buffer, buffer + sizeof(buffer), 0);

	int level = 0;
	const int result = encoder_t()(buffer, buffer + sizeof(buffer) - 1, A(), level);

	std::cout << "Wrote (" << result << ") bytes" << std::endl;
	std::cout << buffer << std::endl;
}
