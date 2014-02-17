# Entity Descriptors

Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.

## C++ Metadata

### TLDR
	get_member_types<ET>()     --> std::vector<std::type_info>
	get_member_names<ET>()     --> std::vector<std::string>
	get_member_typenames<ET>() --> std::vector<std::string>

	moneta::textual
	get_member_types(char*)     --> std::vector<std::type_info>
	get_member_names(char*)     --> std::vector<std::string>
	get_member_typenames(char*) --> std::vector<std::string>

	get_entity_names() --> std::vector<std::string> // ["Person", "Address", "Gateway"]

C++ doen't support reflection very well. Actually, it doesn't have any. So we need to create our own using some special tricks.

### Member Names

One classic example are member names, or entity attributes, in Data Modeling lingo. This means that we can't easily get a string at runtime that gives us the identifier used in source code for a given variable. Well, at least not portably.

In order to provide this information, entities must have a Mamber Names map.

	const char* member_names[] = {
		"Name", "Age", "FavoriteColor"
	};

In the future, Boost MPL strings will be supported, giving full compile time string availability for algorithms. But only if this really really proves itself useful (and needed).

	mpl::vector<
		mpl::string<'Name'>,
		mpl::string<'Age'>,
		mpl::string<'Favo', 'rite', 'Colo', 'r'>
	>

### Member Types

Just as important as knowing the member names, it's important to know the member types.


## The easy way

By using macros we can automate all the (small) metadata generation for a given class.

Example:

	#define ENTITY_DEFINITION \
		FQN((Application)(Model)(Person)) \
		MEMBERS_BEGIN \
			MEMBER(int,                              ID            ) \
			MEMBER(std::string,                      Name          ) \
			MEMBER(optional<int>,                    FavoriteNumber) \
			MEMBER(optional<boost::gregorian::date>, Timestamp     ) \
 			MEMBER(Address,                          Address       ) \
		MEMBERS_END

SQL entity:

	#define ENTITY_DEFINITION \
		FQN((Application)(Model)(Person)) \
		MEMBERS_BEGIN \
			SQL_MEMBER(int,                              ID,             ID             ) \
			SQL_MEMBER(std::string,                      Name,           NAME           ) \
			SQL_MEMBER(optional<int>,                    FavoriteNumber, FAVORITE_NUMBER) \
			SQL_MEMBER(optional<boost::gregorian::date>, Timestamp,      TIMESTAMP      ) \
 			SQL_MEMBER(Application::Model::Address,      Address,        ADDRESS        ) \
		MEMBERS_END

	#define ENTITY_DEFINITION \
		FQN((Application)(Model)(Person)) \
		MEMBERS_BEGIN \
			MEMBER( \
				TYPE_AND_NAME(int, ID) \
				SQL_COLUMN(('ID')) \
				SQL_PK \
				EBML_ID(0xa2c4e5) \
			) \
			MEMBER( \
				TYPE_AND_NAME(std::string, Name) \
				SQL_COLUMN(('Name')) \
				EBML_ID(0xc1) \
			) \
			\
			MEMBER( \
				TYPE_AND_NAME(optional<int>, FavoriteNumber) \
				SQL_COLUMN(('Favo', 'rite', 'Numb', 'er')) \
				EBML_ID(0xc2) \
			) \
			\
			MEMBER( \
				TYPE_AND_NAME(optional<boost::gregorian::date>, Timestamp) \
				SQL_COLUMN(('Time', 'stam', 'p')) \
				EBML_ID(0xc2) \
			) \
			\
			MEMBER( \
				TYPE_AND_NAME(Application::Model::Address, Address) \
				SQL_COLUMN(('Addr', 'ess')) \
				EBML_ID(0xc2) \
			) \
		MEMBERS_END
