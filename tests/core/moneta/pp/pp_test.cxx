#include "pch.hxx"
#include <moneta/pp/describe_entity.hxx>
#include <moneta/traits/is_entity.hxx>
#include <boost/mpl/equal.hpp>

typedef char char_16[16];

MONETA_DEFINE_ENTITY_NS(
	(ns0)(ns1)(ns2), some_entity,
	((int,     a))
	((double,  b))
	((char,    c))
	((char_16, d))
)

MONETA_DEFINE_ENTITY_NS(
	(nsA)(nsB)(nsC), other_entity,
	((int,                        x))
	((std::string,                y))
	((ns0::ns1::ns2::some_entity, z))
)

inline void static_test() {
	BOOST_MPL_ASSERT((moneta::traits::is_entity<ns0::ns1::ns2::some_entity>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		moneta::traits::members<ns0::ns1::ns2::some_entity>::type,
		boost::mpl::vector<
			MONETA_MEMBER(ns0::ns1::ns2::some_entity, int,     a),
			MONETA_MEMBER(ns0::ns1::ns2::some_entity, double,  b),
			MONETA_MEMBER(ns0::ns1::ns2::some_entity, char,    c),
			MONETA_MEMBER(ns0::ns1::ns2::some_entity, char_16, d)
		>
	>));

	//

	BOOST_MPL_ASSERT((moneta::traits::is_entity<nsA::nsB::nsC::other_entity>));

	BOOST_MPL_ASSERT((boost::mpl::equal<
		moneta::traits::members<nsA::nsB::nsC::other_entity>::type,
		boost::mpl::vector<
			MONETA_MEMBER(nsA::nsB::nsC::other_entity, int, x),
			MONETA_MEMBER(nsA::nsB::nsC::other_entity, std::string, y),
			MONETA_MEMBER(nsA::nsB::nsC::other_entity, ns0::ns1::ns2::some_entity, z)
		>
	>));
}
