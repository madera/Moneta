#pragma once
#include "../encoder.hxx"
#include "../_aux/io/copy_n.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec { namespace rawbin_implementation {

	struct member_encoder {

		template <class Iterator, class Member, class Entity, class Path>
		typename boost::enable_if<
			boost::is_pod<typename Member::result_type>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, const Entity& entity, const Path&) const {
			typedef typename boost::add_const<
				typename Member::result_type
			>::type value_type;
			
			value_type value = Member()(entity);
			const size_t value_size = sizeof(value_type);

			// XXX: FIXME: Use Codec IO to do this properly.

			const size_t length = std::distance(begin, end); // XXX: No. Just no.
			if (length < value_size) {
				return length - value_size;
			}

			const char* data = reinterpret_cast<const char*>(&value);
			std::copy(data, data + value_size, begin);
			begin += value_size;

			//io::copy_n(data, data + value_size, value_size, begin);

			return value_size;
		}

	};

	typedef moneta::codec::encoder<
		member_actions<member_encoder>
	> rawbin_encoder;

}}}

namespace moneta { namespace codec {
	using rawbin_implementation::rawbin_encoder;
}}
