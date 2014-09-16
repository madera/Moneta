#pragma once
#include "../traits/detail/member_trait_base.hxx"

namespace moneta { namespace codec {

	namespace detail {
	
		template <class Codec, class Entity = void>
		struct typecode_type : boost::false_type {};

		template <class Codec, class Entity>
		struct typecode : boost::false_type {};
		
	}

	template <class Codec>
	struct typecode_reader;

	template <class Codec, class TypeCode, class Iterator>
	int read_typecode(TypeCode& code, Iterator begin, Iterator end) {
		return typecode_reader<Codec>()(code, begin, end);
	}

}}
