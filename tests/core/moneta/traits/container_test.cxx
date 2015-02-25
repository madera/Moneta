#include "stdafx.h"
#include <moneta/traits/container.hxx>
#include <boost/lexical_cast.hpp>

inline void static_test() {
}

struct copy_operation {
	std::vector<std::string>& _output;

	copy_operation(std::vector<std::string>& output)
	 : _output(output) {}

	template <class Value>
	void operator()(const Value& value) const {
		_output.push_back(value);
	}
};

BOOST_AUTO_TEST_CASE(test_moneta_traits_container_for_each) {
	std::vector<std::string> tags;
	for (int i=0; i<5; ++i) {
		tags.push_back(boost::lexical_cast<std::string>(i));
	}

	std::vector<std::string> copy;
	moneta::traits::container_for_each(tags, copy_operation(copy));

	BOOST_CHECK_EQUAL_COLLECTIONS(tags.begin(), tags.end(), copy.begin(), copy.end());
}

BOOST_AUTO_TEST_CASE(test_moneta_traits_container_for_each_optional) {
	boost::optional<std::vector<std::string> > otags = std::vector<std::string>();
	for (int i=0; i<5; ++i) {
		otags->push_back(boost::lexical_cast<std::string>(i));
	}

	std::vector<std::string> copy;
	moneta::traits::container_for_each(otags, copy_operation(copy));

	BOOST_CHECK_EQUAL_COLLECTIONS(otags->begin(), otags->end(), copy.begin(), copy.end());
}
