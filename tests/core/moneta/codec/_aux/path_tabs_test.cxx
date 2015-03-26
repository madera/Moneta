#include "stdafx.h"
#include <moneta/codec/_aux/path_tabs.hxx>
#include <boost/mpl/vector.hpp>
#include "../../model/SportsTeam.hxx"

/*
	TODO: Delete this table after coding.

	|								      PathSz   Cmem    CwdC    Extra	|  Total      
--------+-----------------------------------------------------------------------+-------+-------+--------+------+-------									      
/	|<Country>								0	0	0	 0	|    0
/	|	<Name>Germany</Name>						0	0	0	 1	|    1
/Address|	<Address Number="1" Street="Strange St.">			1	0	0	 0	|    1
/Address|	   +1   <Reference>Abbey Something</Reference>			1	0	0	 1	|    2
/Address|	</Address>							1	0	0	 0	|    1
/Persons|	<Persons>							1	1	0	-1	|    1
/Persons|	        <Person ID="0">						1	1	0	 0	|    2
/Persons|		   +1   <Name>Michael Jordan</Name>			1	1	0	 1	|    3
/Persons|		   +1   <Height>2</Height>				1	1	0	 1	|    3
/Persons|		   +1   <Ratings>0</Ratings>				1	1	0	 1	|    3
/Persons|		</Person>						1	1	0	 0	|    2
/Persons|		<Person ID="0">						1	1	0	 0	|    2
/Persons|		   +1   <Name>Magic Johnson</Name>			1	1	0	 1	|    3
/Persons|		   +1   <Height>2.1000000000000001</Height>		1	1	0	 1	|    3
/Persons|		   +1   <Ratings>0</Ratings>				1	1	0	 1	|    3
/Persons|		</Person>						1	1	0	 0	|    2
/Persons|	</Persons>							1	1	0	-1	|    1
/Tags	|	<Tags>								1	1	0	-1	|    1
/Tags	|		<CrazyTag>Sometag</CrazyTag>				1	1	0	 0	|    2
/Tags	|		<CrazyTag>Anothertag</CrazyTag>				1	1	0	 0	|    2
/Tags	|	</Tags>								1	1	0	-1	|    1
/	|</Country>								0	0	0	 0	|    0

*/

BOOST_AUTO_TEST_CASE(path_tabs_test) {
	using moneta::codec::aux::path_tabs;

	typedef MONETA_MEMBER(SportsTeam, std::string, Name) M;

	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector0<                   > >()), ""              );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector1<M                  > >()), "\t"            );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector2<M, M               > >()), "\t\t"          );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector3<M, M, M            > >()), "\t\t\t"        );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector4<M, M, M, M         > >()), "\t\t\t\t"      );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector5<M, M, M, M, M      > >()), "\t\t\t\t\t"    );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector6<M, M, M, M, M, M   > >()), "\t\t\t\t\t\t"  );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector7<M, M, M, M, M, M, M> >()), "\t\t\t\t\t\t\t");

	typedef MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags) C;

	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector0<                   > >()),      ""              );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector1<C                  > >()), "\t" "\t"            );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector2<C, M               > >()), "\t" "\t\t"          );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector3<C, M, M            > >()), "\t" "\t\t\t"        );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector4<C, M, M, M         > >()), "\t" "\t\t\t\t"      );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector5<C, M, M, M, M      > >()), "\t" "\t\t\t\t\t"    );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector6<C, M, M, M, M, M   > >()), "\t" "\t\t\t\t\t\t"  );
	BOOST_CHECK_EQUAL((path_tabs<boost::mpl::vector7<C, M, M, M, M, M, M> >()), "\t" "\t\t\t\t\t\t\t");

	typedef boost::mpl::vector7<
		MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags),
		MONETA_MEMBER(SportsTeam, std::string, Name),
		MONETA_MEMBER(SportsTeam, std::vector<Person>, Players),
		MONETA_MEMBER(SportsTeam, std::string, Name),
		MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags),
		MONETA_MEMBER(SportsTeam, std::string, Name),
		MONETA_MEMBER(SportsTeam, std::vector<std::string>, Tags)
	> fake_path;

	BOOST_CHECK_EQUAL((path_tabs<fake_path      >()), "\t\t\t\t\t\t\t\t\t\t\t"); // 7 + 4 = 11 tabs
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -1>()), "\t\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -2>()), "\t\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -3>()), "\t\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -4>()), "\t\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -5>()), "\t\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -6>()), "\t\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -7>()), "\t\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -8>()), "\t\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,   -9>()), "\t\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path,  -10>()), "\t");
	BOOST_CHECK_EQUAL((path_tabs<fake_path, -100>()), "");
	BOOST_CHECK_EQUAL((path_tabs<fake_path, -999>()), "");
}
