#pragma once

namespace moneta { namespace codec { namespace detail {

	struct path_printer {
		std::ostringstream& oss;

		path_printer(std::ostringstream& oss_)
			: oss(oss_) {}

		template <class Member>
		void operator()(Member&) const {
			oss << '/'
				<< moneta::traits::get_entity_name<typename Member::class_type>()
				<< "::"
				<< moneta::traits::detail::member_name<Member>::get();
		}
	};

	template <class Path>
	typename boost::enable_if<boost::is_same<Path, void>, std::string>::type
	stringize_path() {
		return "<void>";
	}

	template <class Path>
	typename boost::disable_if<boost::is_same<Path, void>, std::string>::type
	stringize_path() {
		std::ostringstream oss;
		path_printer state(oss);
		boost::mpl::for_each<Path>(state);
		return state.oss.str();
	}

}}}
