//
// User entity.
//
#include <moneta/meta/members_of.hxx> // members_of
#include <moneta/meta/member.hxx>
#include <moneta/meta/member_name.hxx>
#include <boost/mpl/vector.hpp>
#include <string>

struct Person {
	int ID;
	std::string Name;
	double Height;
	int Fingers;
};

namespace moneta { namespace meta { namespace detail {
	template <>
	struct members_of<Person> : boost::mpl::vector<
		MONETA_MEMBER(Person, int,         ID     ),
		MONETA_MEMBER(Person, std::string, Name   ),
		MONETA_MEMBER(Person, double,      Height ),
		MONETA_MEMBER(Person, int,         Fingers)
	> {};
}}}

MONETA_MEMBER_NAME(MONETA_MEMBER(Person, int,         ID     ), ID     )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, std::string, Name   ), Name   )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, double,      Height ), Height )
MONETA_MEMBER_NAME(MONETA_MEMBER(Person, int,         Fingers), Fingers)
