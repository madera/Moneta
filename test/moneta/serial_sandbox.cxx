#include "stdafx.h"
#include <moneta/serialization/rawbin/decoder.hxx>
#include <moneta/serialization/rawbin/io.hxx>
#include <moneta/serialization/detail/hexdump.hxx>
#include <moneta/pp/sql_entity.hxx>
#include "model/Person.hxx"

#define MONETA_DESCRIBE_ENTITY_NG(entity, members) \
	MONETA_DESCRIBE_ENTITY_BASE(entity, members) \
	MONETA_ENTITY_NAME(entity, entity)

#define MONETA_DEFINE_AND_DESCRIBE_ENTITY(entity, members) \
	MONETA_PP_DEFINE_ENTITY_STRUCT(entity, members) \
	MONETA_DESCRIBE_ENTITY_NG(entity, members)

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	server_handshake,
	((int, magic   ))
	((int, code    ))
	((int, unknown0))
	((int, version ))
)

namespace moneta { namespace serialization { namespace rawbin {

	namespace detail {

		template <>
		struct decoder<server_handshake> {
			enum {
				MINIMUM_RECOGNITION_SIZE = 2*sizeof(int)
			};

			template <class IteratorType>
			int operator()(IteratorType begin, IteratorType end, server_handshake& entity) const {
				int delta = std::distance(begin, end) - MINIMUM_RECOGNITION_SIZE; // XXX: No std::distance().
				if (delta < 0) {
					return delta;
				}

				begin = read_raw(begin, end, entity.magic);
				begin = read_raw(begin, end, entity.code);

				if (entity.magic != 'vOeG' || entity.code != 0x10) {
					return 0;
				}

				begin = read_raw(begin, end, entity.unknown0);
				begin = read_raw(begin, end, entity.version);
				
				return 4*sizeof(int);
			}
		};

	}

}}}

// --- --- ---



// --- --- ---

BOOST_AUTO_TEST_CASE(serial_sandbox) {
	unsigned char data[16] = {
		0x47, 0x65, 0x4F, 0x76, 0x10, 0x00, 0x00, 0x00,
		0xA7, 0x02, 0x00, 0x00, 0x00, 0x50, 0x01, 0x00
	};

	server_handshake handshake;
	int result = moneta::serialization::rawbin::decode(data, data + 16, handshake);

	BOOST_CHECK_EQUAL(result, 16);

	hexdump(data, 16);
	hexdump_line(data, 16);
}
