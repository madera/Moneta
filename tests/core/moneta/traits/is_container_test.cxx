#include "pch.hxx"
#include <moneta/traits/is_container.hxx>
#include "../model/Person.hxx"
#include "../model/Address.hxx"

inline void static_test() {
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_container<std::string>));

	BOOST_MPL_ASSERT((moneta::traits::is_container<std::vector<std::string> >));
	BOOST_MPL_ASSERT((moneta::traits::is_container<std::list<std::string> >));

	BOOST_MPL_ASSERT_NOT((moneta::traits::is_container<std::vector<std::string>*>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_container<int>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_container<float>));
	BOOST_MPL_ASSERT_NOT((moneta::traits::is_container<void>));

	BOOST_MPL_ASSERT((moneta::traits::is_container<std::vector<Person> >));
	BOOST_MPL_ASSERT((moneta::traits::is_container<std::vector<Address> >));
}
