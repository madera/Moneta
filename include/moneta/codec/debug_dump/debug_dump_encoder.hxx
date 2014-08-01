#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>
#include <boost/format.hpp>

namespace moneta { namespace codec {

 	struct debug_dump;
 
	namespace detail {
		// TODO: Continue this when Path-enabled codecs arise.

		//struct path_printer {
		//	std::ostringstream& oss;

		//	path_printer(std::ostringstream& oss_)
		//	 : oss(oss_) {}

		//	template <class T>
		//	void operator()(const T x) const {
		//		oss << 'x';
		//	}
		//};

		//template <class Path>
		//std::string stringize_path() {
		//	std::ostringstream oss;
		//	path_printer state(oss);
		//	boost::mpl::for_each<Path>(state);
		//	return state.oss.str();
		//}
	}

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

	template <class Member, class Path>
	struct member_encoder<debug_dump, Member, Path> {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class Iterator>
		int operator()(const entity_type& entity, Member& member, Iterator& begin, Iterator& end) const {
			std::cerr << boost::format("%|-20|: ") % traits::detail::member_name<Member>::get();
			value_encoder<debug_dump, value_type>()(member(entity), 0, 0, Path());
			return 1;
		}
	};

}}
