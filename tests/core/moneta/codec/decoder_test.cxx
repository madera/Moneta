//
// TODO: Decode this file using Codec IO primitives.
//

#include "stdafx.h"
#include <moneta/codec/decoder.hxx>
#include <moneta/codec/_aux/io/ostringstream.hxx>
#include <moneta/codec/_aux/path_tabs.hxx>
#include <moneta/algorithm/detail/stringize_path.hxx>
#include <moneta/pp/describe_entity.hxx>

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Point3D,
	((boost::uint32_t, x))
	((boost::uint32_t, y))
	((boost::uint32_t, z))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Triangle,
	((Point3D, center))
	((std::vector<Point3D>, points))
)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	LotteryNumbers,
	((boost::uint32_t, date))
	((std::vector<boost::uint32_t>, numbers))
)

struct testcodec_enter_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Entity& entity, const Path&, State& state) const {
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
	template <class Iterator, class Member, class Entity, class Path, class State>
	typename boost::enable_if<
		boost::is_pod<typename Member::result_type>,
		int
	>::type
	operator()(Iterator begin, Iterator end, const Member&, Entity& entity, const Path&, State& state) const {
		const size_t needed = sizeof(typename Member::result_type);
		if (end - begin < needed) {
			return 0 - (needed - (end - begin));
		}

		Member()(entity) = *(typename Member::result_type*)begin;

		return needed;
	}
};

struct testcodec_leave_entity {
	template <class Iterator, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Entity& entity, const Path&, State& state) const {
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
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
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
	template <class Iterator, class Value, class Member, class Entity, class Path, class State>
	int operator()(
		Iterator begin, Iterator end, Value& value, Member&, Entity& entity, const Path&, State& state
	) const {
		if (begin == end) {
			return -1;
		}

		value = *(Value*)begin;

		return sizeof(Value);
	}
};

struct testcodec_leave_container {
	template <class Iterator, class Member, class Entity, class Path, class State>
	int operator()(Iterator begin, Iterator end, Member&, Entity& entity, const Path&, State& state) const {
		if (begin == end) {
			return -1;
		}

		if (*begin != 'l') {
			return 0;
		}

		return 1;
	}
};

using namespace moneta::codec;
typedef decoder<
	enter_actions<testcodec_enter_entity>,
	member_actions<testcodec_member>,
	leave_actions<testcodec_leave_entity>,
	enter_container_actions<testcodec_enter_container>,
	container_item_actions<testcodec_container_item>,
	leave_container_actions<testcodec_leave_container>
> decoder_t;

BOOST_AUTO_TEST_CASE(point3d_simple_decoder_test) {
	const unsigned char buffer[] = {
		'E',
			0x01, 0x00, 0x00, 0x00,
			0x02, 0x00, 0x00, 0x00,
			0x03, 0x00, 0x00, 0x00,
		'L'
	};

	Point3D point;
	//const unsigned char* itr = std::begin(buffer);
	//const int result = decoder_t()(itr, std::end(buffer), point);
	const int result = moneta::codec::decode<decoder_t>(std::begin(buffer), std::end(buffer), point);

	BOOST_CHECK_EQUAL(result, 1 + 3 * sizeof(boost::uint32_t) + 1);
	BOOST_CHECK_EQUAL(point.x, 1);
	BOOST_CHECK_EQUAL(point.y, 2);
	BOOST_CHECK_EQUAL(point.z, 3);
}

// TODO: Test triangle decoder.

BOOST_AUTO_TEST_CASE(lottery_simple_decoder_test) {
	const unsigned char buffer[] = {
		'E',
		0x04, 0x03, 0x02, 0x01,
			'e',
				0x03,
				0x01, 0x00, 0x00, 0x00,
				0x02, 0x00, 0x00, 0x00,
				0x03, 0x00, 0x00, 0x00,
			'l',
		'L'
	};

	LotteryNumbers lottery;
	//const unsigned char* itr = std::begin(buffer);
	//const int result = decoder_t()(itr, std::end(buffer), lottery);
	const int result = moneta::codec::decode<decoder_t>(std::begin(buffer), std::end(buffer), lottery);

	BOOST_CHECK_EQUAL(result, sizeof(buffer));
	BOOST_CHECK_EQUAL(lottery.date, 0x01020304);
	BOOST_REQUIRE_EQUAL(lottery.numbers.size(), 3);
	BOOST_CHECK_EQUAL(lottery.numbers[0], 1);
	BOOST_CHECK_EQUAL(lottery.numbers[1], 2);
	BOOST_CHECK_EQUAL(lottery.numbers[2], 3);
}

// --------------------------------------------------------------------------------------------------------------------

struct some_visitor {
	std::ostringstream& _output;

	some_visitor(std::ostringstream& output)
	 : _output(output) {
	}

	template <class Entity>
	void operator()(Entity*) const {
		_output << "x";
	}
};

BOOST_AUTO_TEST_CASE(test_moneta_codec_decoder_typecode_based) {
	const unsigned char buffer[] = {
		0x01, 0x02, 0x03
	};

	std::ostringstream oss;
	some_visitor visitor(oss);
	//const int result = moneta::codec::decode<decoder_t>(std::begin(buffer), std::end(buffer), visitor);

	std::cout << oss.str() << std::endl;
}
