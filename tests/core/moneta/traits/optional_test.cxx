#include "stdafx.h"
#include <moneta/traits/optional.hxx>

inline void static_test() {
	BOOST_MPL_ASSERT((moneta::traits::is_optional<boost::optional<char> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<boost::optional<int> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<boost::optional<std::string> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<boost::optional<std::vector<std::string> > >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<const boost::optional<char> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<const boost::optional<int> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<const boost::optional<std::string> >));
	BOOST_MPL_ASSERT((moneta::traits::is_optional<const boost::optional<std::vector<std::string> > >));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<void>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<char>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<int>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<std::string>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<std::vector<std::string> >));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<const void>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<const char>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<const int>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<const std::string>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_optional<const std::vector<std::string> >));

	//

	using moneta::traits::optional_value_type;

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<char> >::type,
		char
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<std::string> >::type,
		std::string
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<std::vector<std::string> > >::type,
		std::vector<std::string>
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<const char> >::type,
		const char
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<const std::string> >::type,
		const std::string
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<boost::optional<const std::vector<std::string> > >::type,
		const std::vector<std::string>
	>));

	//

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<const boost::optional<char> >::type,
		const char
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<const boost::optional<std::string> >::type,
		const std::string
	>));

	BOOST_MPL_ASSERT((boost::is_same<
		typename optional_value_type<const boost::optional<std::vector<std::string> > >::type,
		const std::vector<std::string>
	>));
}

BOOST_AUTO_TEST_CASE(test_moneta_traits_is_optional_get_optional_value) {
	boost::optional<int> oi = 1;
	const boost::optional<int> coi = 123;

	BOOST_CHECK_EQUAL(moneta::traits::get_optional_value(oi), 1);
	BOOST_CHECK_EQUAL(moneta::traits::get_optional_value(coi), 123);
}
