//
// TODO: Decode this file using Codec IO primitives.
//

#include "stdafx.h"
#include <moneta/codec/decoder.hxx>
#include <moneta/codec/_aux/io/ostringstream.hxx>
#include <moneta/codec/_aux/path_tabs.hxx>
#include <moneta/algorithm/detail/stringize_path.hxx>
#include <moneta/pp/describe_entity.hxx>

MONETA_DEFINE_ENTITY(
	Point3D,
	((boost::uint32_t, x))
	((boost::uint32_t, y))
	((boost::uint32_t, z))
)

MONETA_DEFINE_ENTITY(
	Triangle,
	((Point3D, center))
	((std::vector<Point3D>, points))
)

MONETA_DEFINE_ENTITY(
	LotteryNumbers,
	((boost::uint32_t, date))
	((std::vector<boost::uint32_t>, numbers))
)

struct testcodec_start_decoding {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Path) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'S') {
			return 0;
		}

		return 1;
	}
};

struct testcodec_enter_entity {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Path) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'E') {
			return 0;
		}

		return 1;
	}
};

struct testcodec_member {
	template <class Iterator, class Entity, class State, class Member, class Path>
	typename boost::enable_if<
		boost::is_pod<typename Member::result_type>,
		int
	>::type
	operator()(Iterator begin, Iterator end, Entity& entity, State&, Member, Path) const {
		const size_t needed = sizeof(typename Member::result_type);
		const size_t available = std::distance(begin, end);
		if (available < needed) {
			return 0 - (needed - available);
		}

		Member()(entity) = *(typename Member::result_type*)begin;

		return needed;
	}
};

struct testcodec_leave_entity {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Path) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'L') {
			return 0;
		}

		return 1;
	}
};

struct testcodec_enter_container {
	template <class Iterator, class Entity, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, Entity& entity, State&, Member, Path) const {
		if (begin == end) {
			return -1;
		}

		const bool correct = (*begin == 'e');
		if (!correct) {
			return 0;
		}

		++begin;
		const size_t size = *begin++; // Char becomes size_t;
		
		// Resize the container. It'll get called once per item.
		Member()(entity).resize(size);

		return 1 + 1; // 'e' + size
	}
};

struct testcodec_container_item {
	//
	// This will be called once for each value of the container set in testcodec_enter_container.
	//
	template <class Iterator, class Entity, typename Value, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, Value& value, State&, Member, Path) const {
		if (begin == end) {
			return -1;
		}

		value = *(Value*)begin;

		return sizeof(Value);
	}
};

struct testcodec_leave_container {
	template <class Iterator, class Entity, class State, class Member, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Member, Path) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'l') {
			return 0;
		}

		return 1;
	}
};

struct testcodec_finish_decoding {
	template <class Iterator, class Entity, class State, class Path>
	int operator()(Iterator begin, Iterator end, Entity&, State&, Path) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'F') {
			return 0;
		}

		return 1;
	}
};

using namespace moneta::codec;
typedef decoder<
	start_actions<testcodec_start_decoding>,
	enter_actions<testcodec_enter_entity>,
	member_actions<testcodec_member>,
	leave_actions<testcodec_leave_entity>,
	enter_container_actions<testcodec_enter_container>,
	container_item_actions<testcodec_container_item>,
	leave_container_actions<testcodec_leave_container>,
	finish_actions<testcodec_finish_decoding>
> decoder_t;

BOOST_AUTO_TEST_CASE(point3d_simple_decoder_test) {
	const unsigned char buffer[] = {
		'S',
			'E',
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00,
				0x03, 0x00, 0x00, 0x00,
			'L',
		'F'
	};

	Point3D point;
	const int result = decoder_t()(std::begin(buffer), std::end(buffer), point);

	BOOST_CHECK_EQUAL(result, 1 + (1 + 3 * sizeof(boost::uint32_t) + 1) + 1);
	BOOST_CHECK_EQUAL(point.x, 1);
	BOOST_CHECK_EQUAL(point.y, 2);
	BOOST_CHECK_EQUAL(point.z, 3);
}

// TODO: Test triangle decoder.

BOOST_AUTO_TEST_CASE(lottery_simple_decoder_test) {
	const unsigned char buffer[] = {
		'S',
			'E',
			0x04, 0x03, 0x02, 0x01,
				'e',
					0x03,
					0x01, 0x00, 0x00, 0x00,
					0x02, 0x00, 0x00, 0x00,
					0x03, 0x00, 0x00, 0x00,
				'l',
			'L',
		'F'
	};

	LotteryNumbers lottery;
	const int result = decoder_t()(std::begin(buffer), std::end(buffer), lottery);

	BOOST_CHECK_EQUAL(result, sizeof(buffer));
	BOOST_CHECK_EQUAL(lottery.date, 0x01020304);
	BOOST_REQUIRE_EQUAL(lottery.numbers.size(), 3);
	BOOST_CHECK_EQUAL(lottery.numbers[0], 1);
	BOOST_CHECK_EQUAL(lottery.numbers[1], 2);
	BOOST_CHECK_EQUAL(lottery.numbers[2], 3);
}
