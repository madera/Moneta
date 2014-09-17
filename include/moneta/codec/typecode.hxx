#pragma once
#include "../traits/detail/member_trait_base.hxx"

#define MONETA_CODEC_DECLARE(name)           \
	namespace moneta { namespace codec { \
		struct name;                 \
	}}

#define MONETA_CODEC_TYPECODE(codec_, entity, value)                               \
	namespace moneta { namespace codec { namespace detail {                    \
		template <>                                                        \
		struct typecode<codec_, entity> {                                  \
			static detail::typecode_type<codec_, entity>::type get() { \
				return value;                                      \
			}                                                          \
		};                                                                 \
	}}}

#define MONETA_CODEC_TYPECODE_TYPE(codec_, type)                                      \
	namespace moneta { namespace codec { namespace detail {                       \
		template <class Entity>                                               \
		struct typecode_type<codec_, Entity> : boost::mpl::identity<type> {}; \
	}}}

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
