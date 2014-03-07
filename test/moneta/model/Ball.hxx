#define ENTITY_DEFINITION \
	FQN((Model)(Ball)) \
	MEMBERS_BEGIN \
		MEMBER(int,         ID         ) \
		MEMBER(std::string, Description) \
		MEMBER(std::string, Sport      ) \
	MEMBERS_END

#include <moneta/end_entity>
