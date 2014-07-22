#pragma once
#include "../encoder.hxx"
#include <boost/type_traits/is_pod.hpp>

namespace moneta { namespace codec {

 	struct debug_dump;
 
	namespace detail {
		struct printer_path {
			template <class T>
			void operator()() const {
				std::cerr << 'x';
			}
		};

		template <class Path>
		void print_path() {
			boost::mpl::for_each<Path>(path_printer());
		}
	}

	template <class Member, class Path>
	struct member_encoder<debug_dump, Member, Path> {
		typedef typename Member::class_type entity_type;
		typedef typename Member::result_type value_type;

		template <class Iterator>
		int operator()(const entity_type& entity, Member& member, Iterator& begin, Iterator& end) const {
			std::ostringstream oss;
			
			print_path<Path>();
			oss << traits::get_entity_name<entity_type>() << '.'
			    << traits::detail::member_name<Member>::get() << ": "
			    << member(entity)
			;

			std::cerr << oss.str() << std::endl;
			return oss.str().size();
		}
	};

}}
