#include "stdafx.h"
#include <moneta/traits/detail/sub_tie.hxx>
#include <boost/mpl/vector_c.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>

static void static_test() {
	typedef boost::tuple<int, char, std::string> tuple_type;

	static const size_t y = boost::mpl::size<tuple_type>::value;

	tuple_type tuple;
	moneta::traits::detail::sub_tie<tuple_type, boost::mpl::vector_c<int, 0      > > pk (tuple);
	moneta::traits::detail::sub_tie<tuple_type, boost::mpl::vector_c<int, 0, 1   > > pk2(tuple);
	moneta::traits::detail::sub_tie<tuple_type, boost::mpl::vector_c<int, 0, 1, 2> > pk3(tuple);
}
