#pragma once
#include "../encoder.hxx"
#include "../../algorithm/detail/stringize_path.hxx"
#include <boost/type_traits/is_pod.hpp>
#include <boost/format.hpp>

namespace moneta { namespace codec {

 	struct debug_dump;
 
	template <class T>
	struct value_encoder<debug_dump, T, typename boost::disable_if<
		boost::mpl::or_<boost::is_array<T>, boost::is_arithmetic<T> >
	> > {
		template <class Path, class Iterator>
		int operator()(const T value, Iterator begin, Iterator end, Path) const {
			std::cerr << value << std::endl;
			return 1;
		}
	};

	template <class T>
	struct value_encoder<debug_dump, T, typename boost::enable_if<boost::is_arithmetic<T> >::type> {
		template <class Path, class Iterator>
		int operator()(const T value, Iterator begin, Iterator end, Path) const {
			std::cerr << boost::format("0x%|08x| %|10|") % value % value << std::endl;
			return 1;
		}
	};

	template <class T, int N>
	struct value_encoder<debug_dump, T[N]> {
		template <class Path, class Iterator>
		int operator()(const T value[N], Iterator begin, Iterator end, Path) const {
			std::cerr << "T[" << std::dec << N << "] (" << sizeof(T)*N << " bytes)" << std::endl;
			return 1;
		}
	};

	template <>
	struct value_encoder<debug_dump, std::string> {
		template <class Path, class Iterator>
		int operator()(const std::string& value, Iterator begin, Iterator end, Path) const {
			std::cerr << value << std::endl;
			return 1;
		}
	};

	template <class Member, class Path>
	struct member_encoder<debug_dump, Member, Path> {
		template <class Entity, class Iterator>
		int operator()(const Entity& entity, Iterator begin, Iterator end) const {
			std::cerr << boost::format("%|-20| %|-20|: ")
				     % detail::stringize_path<Path>() % traits::detail::member_name<Member>::get();

			value_encoder<debug_dump, typename Member::result_type>()(Member()(entity), 0, 0, Path());
			
			return 1;
		}
	};

}}
