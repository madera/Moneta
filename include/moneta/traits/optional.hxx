#include <boost/optional.hpp>

namespace moneta { namespace traits {
	
	template <typename T>
	struct is_optional : boost::false_type {};

	template <typename T>
	struct is_optional<boost::optional<T> > : boost::true_type {};

	//

	template <class T>
	struct optional_value_type : boost::mpl::identity<T> {};

	template <class T>
	struct optional_value_type<boost::optional<T> > : boost::mpl::identity<T> {};

}}
