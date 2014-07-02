#include "stdafx.h"
#include <moneta/serialization/rawbin/decoder.hxx>
#include <moneta/serialization/rawbin/rawbin_io.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/serialization/for_each_member.hxx>
#include <moneta/pp/sql_entity.hxx> // Entity defs

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Handshake,
	((int, Magic))
	((int, Category))
	((int, Unknown))
	((int, Version))
)

MONETA_FIXED_VALUE(MONETA_MEMBER(Handshake, int, Magic), 'vOeG')
MONETA_FIXED_VALUE(MONETA_MEMBER(Handshake, int, Category), 0x0010)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(serial_sandbox) {
	using moneta::traits::detail::fixed_value;

	unsigned char data[16] = {
		0x47, 0x65, 0x4F, 0x76, 0x10, 0x00, 0x00, 0x00,
		0xA7, 0x02, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00
	};

	Handshake handshake;
	int result = moneta::serialization::rawbin::decode(data, data + 16, handshake);

	{
		std::string message = (result == 0)? "unable" :
			(result > 0)? "decoded" :
			(result < 0)? "need more" : "<fuck>";

		std::cout << message << std::endl;
	}

	BOOST_CHECK_EQUAL(result, 16);

	hexdump(data, 16);
	hexdump_line(data, 16);

}
