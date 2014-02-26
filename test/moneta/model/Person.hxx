//
// User entity.
//
#include <moneta/traits/members.hxx>
#include <moneta/traits/member.hxx>
#include <moneta/traits/member_names.hxx>
#include <moneta/traits/pk.hxx>
#include <boost/mpl/vector.hpp>
#include <string>

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
};

namespace moneta { namespace traits { namespace detail {
	template <>
	struct members_of<Person> : boost::mpl::vector<
		MONETA_MEMBER(Person, int,         ID     ),
		MONETA_MEMBER(Person, std::string, Name   ),
		MONETA_MEMBER(Person, double,      Height ),
		MONETA_MEMBER(Person, int,         Fingers)
	> {};
}}}

MONETA_PRIMARY_KEY(MONETA_MEMBER(Person, int, ID))

MONETA_MEMBER_NAME(MONETA_MEMBER(Person, int,         ID     ), ID     )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, std::string, Name   ), Name   )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, double,      Height ), Height )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, int,         Fingers), Fingers)
