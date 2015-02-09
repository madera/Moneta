#pragma once
#include "../decoder.hxx"
#include "../_aux/io/copy_n.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec { namespace rawbin_implementation {

	struct member_decoder {
		template <class Iterator, class Member, class Entity, class Path>
		typename boost::enable_if<
			boost::is_pod<typename Member::result_type>,
			int
		>::type
		operator()(Iterator begin, Iterator end, const Member&, Entity& entity, const Path&) const {
			typedef typename boost::add_const<
				typename Member::result_type
			>::type value_type;
			
			// XXX: FIXME: Use Codec IO to do this properly.
			// XXX: This is trash. Fix this!!

			const size_t value_size = sizeof(value_type);
			const size_t length = std::distance(begin, end); // XXX: No. Just no.
			if (length < value_size) {
				return length - value_size;
			}
			
			// XXX: FIX THIS!!
			value_type* p = (value_type*)begin;
			Member()(entity) = *p;

			//io::copy_n(data, data + value_size, value_size, begin);

			return value_size;
		}

	};

	typedef moneta::codec::decoder<
		member_actions<member_decoder>
	> rawbin_decoder;

}}}

namespace moneta { namespace codec {
	using rawbin_implementation::rawbin_decoder;
}}
