#include "stdafx.h"
#include <moneta/traits/detail/sub_tie_vector.hxx>
#include <boost/mpl/vector_c.hpp>
#include <string>
#include "../../model/Cat.hxx"

#include <moneta/traits/rtuple.hxx>
#include <moneta/traits/rtie.hxx>
#include <moneta/traits/to_rtuple.hxx>
#include <moneta/traits/to_rtie.hxx>

#include <boost/mpl/print.hpp>

// XXX: Move!
template <class EntityType>
struct rtie_pk_extractor {
	BOOST_MPL_ASSERT((moneta::traits::is_entity<EntityType>));
	BOOST_MPL_ASSERT_NOT((boost::is_const<EntityType>)); // XXX: Temporary... just for now.

	typedef typename moneta::sql::traits::rtuple<EntityType>::type rtuple_type;
	typedef typename moneta::sql::traits::rtie  <EntityType>::type   rtie_type;
	
	typedef boost::mpl::vector_c<int, 0> pk_indeces;
	typedef moneta::traits::detail::sub_tie_vector<rtie_type, pk_indeces> rtie_pk_type;
	
	//typedef moneta::traits::detail::deref_if_unary<rtie_pk_type> pk_derefer_type;
	//typedef typename pk_derefer_type::type type;

	typedef rtie_pk_type type;

	//type operator()(rtuple_type& rtuple) {
	//	auto a = rtie_pk_type(rtuple);
	//	moneta::traits::detail::deref_if_unary<rtie_type, true> b;
	//	//b(a);
	//	return a;
	//}
};

//template <class EntityType>
//typename rtie_pk_extractor<EntityType>::type
//extract_rtuple_pk(typename rtie_pk_extractor<EntityType>::rtuple_type& rtuple) {
//	return rtie_pk_extractor<EntityType>()(rtuple);
//}
// !evoM :XXX

BOOST_AUTO_TEST_CASE(temp_move) {
	Cat cat;
	cat.ID = 555;
	cat.Name = "Garfield";
	cat.Address.ID = 255;
	cat.Address.Street = 1;
	cat.Address.Street = "Infinite Loop";

//	moneta::sql::traits::rtuple<Cat>::type rtuple = moneta::sql::traits::to_rtuple(cat);
//	auto fx = extract_rtuple_pk<Cat>(rtuple);

}

BOOST_AUTO_TEST_CASE(tuple_sub_tie_test) {
	using boost::mpl::vector_c;
	using moneta::traits::detail::sub_tie_vector;

	typedef moneta::traits::tuple<Cat>::type tuple_type;
	tuple_type tuple;
	//sub_tie_vector<tuple_type, vector_c<int, 0      > > pk (tuple);
	//sub_tie_vector<tuple_type, vector_c<int, 0, 1   > > pk2(tuple);
	//sub_tie_vector<tuple_type, vector_c<int, 0, 1, 2> > pk3(tuple);

	//typedef moneta::traits::tuple<const Cat>::type const_tuple_type;
	//const_tuple_type const_tuple;
	//sub_tie_vector<const_tuple_type, vector_c<int, 0      > > cpk (const_tuple);
	//sub_tie_vector<const_tuple_type, vector_c<int, 0, 1   > > cpk2(const_tuple);
	//sub_tie_vector<const_tuple_type, vector_c<int, 0, 1, 2> > cpk3(const_tuple);
}

//BOOST_AUTO_TEST_CASE(rtuple_sub_tie_test) {
//	using boost::mpl::vector_c;
//	using moneta::traits::detail::sub_tie_vector;
//
//	typedef moneta::traits::rtuple<Cat>::type rtuple_type;
//	rtuple_type tuple;
//	sub_tie_vector<rtuple_type, vector_c<int, 0      > > pk (rtuple);
//	sub_tie_vector<rtuple_type, vector_c<int, 0, 1   > > pk2(rtuple);
//	sub_tie_vector<rtuple_type, vector_c<int, 0, 1, 2> > pk3(rtuple);
//
//	typedef moneta::traits::rtuple<const Cat>::type const_rtuple_type;
//	const_tuple_type const_tuple;
//	sub_tie_vector<const_rtuple_type, vector_c<int, 0      > > cpk (const_rtuple);
//	sub_tie_vector<const_rtuple_type, vector_c<int, 0, 1   > > cpk2(const_rtuple);
//	sub_tie_vector<const_rtuple_type, vector_c<int, 0, 1, 2> > cpk3(const_rtuple);
//}

// TODO: FIXME: Add tests. This is far from finished.

#include <boost/mpl/equal.hpp>

static void static_test() {

	using moneta::traits::detail::sub_tie_vector;
	using boost::fusion::vector;
	using boost::mpl::vector_c;

	typedef boost::fusion::vector<char , short , int > tuple_t;
	typedef boost::fusion::vector<char&, short&, int&>   tie_t;

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0>
		>::type,
		vector<char&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0, 1>
		>::type,
		vector<char&, short&>
	>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		sub_tie_vector<
			vector<char, short, int>,
			vector_c<int, 0, 1, 2>
		>::type,
		vector<char&, short&, int&>
	>));

	// This test should cause an assertion fail inside sub_tie_vector.
	//BOOST_MPL_ASSERT((boost::mpl::equal<
	//	sub_tie_vector<
	//		vector<char, short, int>,
	//		vector_c<int, 0, 1, 2, 3>
	//	>::type,
	//	vector<char&, short&, int&>
	//>));
}

BOOST_AUTO_TEST_CASE(sub_tie_test) {
	typedef boost::fusion::vector<char, short, int> tuple_t;

	typedef moneta::traits::detail::sub_tie_vector<
		tuple_t,
		boost::mpl::vector_c<int, 0>
	> sub_tie;

	tuple_t tuple('a', 1, 10);
	sub_tie::type tie = sub_tie()(tuple);

	using moneta::traits::detail::deref_if_unary;
	using moneta::traits::detail::unary_deref;
	using boost::fusion::at_c;

	int first = unary_deref(tie);
	BOOST_CHECK_EQUAL(at_c<0>(tuple), first);

	deref_if_unary<tuple_t>::type same = unary_deref(same);
	BOOST_CHECK_EQUAL(at_c<0>(tuple), at_c<0>(same));
}
