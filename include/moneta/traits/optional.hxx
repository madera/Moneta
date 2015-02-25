#pragma once
#include <boost/optional.hpp>

namespace moneta { namespace traits {
	
	template <typename T>
	struct is_optional : boost::false_type {};

	template <typename T>
	struct is_optional<boost::optional<T> > : boost::true_type {};

	template <typename T>
	struct is_optional<boost::optional<T>&> : boost::true_type {};

	template <typename T>
	struct is_optional<const boost::optional<T> > : boost::true_type {};

	template <typename T>
	struct is_optional<const boost::optional<T>&> : boost::true_type {};

	template <typename T>
	struct isnt_optional : boost::mpl::not_<is_optional<T> > {};

	//

	template <class T, class Enable = void>
	struct optional_value_type : boost::mpl::identity<T> {};

	template <class T>
	struct optional_value_type<
		T,
		typename boost::enable_if<
			is_optional<T>
		>::type
	> : boost::mpl::if_<
		boost::is_const<T>,
		typename boost::add_const<typename T::value_type>::type,
		typename T::value_type
	> {};

	//

	namespace detail {
		template <typename T, typename Enable = void>
		struct get_optional_value_impl {
			T& operator()(T& x) const {
				return x;
			}
		};

		template <typename T>
		struct get_optional_value_impl<
			T,
			typename boost::enable_if<
				is_optional<T>
			>::type
		> {
			typename optional_value_type<T>::type& operator()(T& x) {
				return x.get();
			}
		};
	}

	template <typename T>
	typename optional_value_type<T>::type& get_optional_value(T& x) {
		return detail::get_optional_value_impl<T>()(x);
	}

	//

	namespace detail {
		template <typename T, typename Enable = void>
		struct is_optional_present_impl {
			bool operator()(T&) const {
				return true;
			}
		};

		template <typename T>
		struct is_optional_present_impl<
			T,
			typename boost::enable_if<
				is_optional<T>
			>::type
		> {
			bool operator()(T& x) {
				return x.is_initialized();
			}
		};
	}

	template <typename T>
	bool is_optional_present(T& x) {
		return detail::is_optional_present_impl<T>()(x);
	}

}}
