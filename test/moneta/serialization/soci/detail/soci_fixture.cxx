#include "stdafx.h"
#include "soci_fixture.hxx"

void create_schema(soci::session& session) {
	session.once <<
		"CREATE TABLE PERSON (					    \n"
		"	PERSON_ID      INTEGER PRIMARY KEY,		    \n"
		"	PERSON_NAME    TEXT    NOT NULL,		    \n"
		"	PERSON_HEIGHT  REAL,				    \n"
		"	PERSON_FINGERS INTEGER				    \n"
		");							    \n";

	session.once <<
		"CREATE TABLE DOG (					    \n"
		"	DOG_OWNER   TEXT    NOT NULL,			    \n"
		"	DOG_ID      INTEGER NOT NULL,			    \n"
		"	DOG_NAME    TEXT    NOT NULL,			    \n"
		"	PRIMARY KEY (DOG_OWNER, DOG_ID)			    \n"
		");							    \n";

	session.once <<
		"CREATE TABLE ADDRESS (					    \n"
		"	ADDRESS_ID      INTEGER PRIMARY KEY,		    \n"
		"	ADDRESS_NUMBER  INTEGER NOT NULL,		    \n"
		"	ADDRESS_STREET  TEXT    NOT NULL		    \n"
		");							    \n";

	session.once <<
		"CREATE TABLE CAT (					    \n"
		"	CAT_ID      INTEGER PRIMARY KEY,		    \n"
		"	CAT_NAME    TEXT    NOT NULL,			    \n"
		"	CAT_ADDRESS INTEGER REFERENCES ADDRESS (ID)	    \n"
		");							    \n";
}

void insert_data(soci::session& session) {
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (1, 'Garfield')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (2, 'Arlene')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (3, 'Nermal')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (4, 'Puss-in-Boots')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (5, 'Tom Cat')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (6, 'Stimpy')";
	session.once << "INSERT INTO CAT (CAT_ID, CAT_NAME) VALUES (7, 'Felix')";

	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (1, 'John Smith', 1.80, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (2, 'Abe Lincoln', 1.90, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (3, 'John Davis', 1.70, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (4, 'Will Smith', 1.90, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (5, 'Donald Trump', 1.85, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (6, 'Warren Buffet', 1.65, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (7, 'Steve Jobs', 1.70, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (8, 'Donald Duck', 1.20, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (9, 'Daffy Duck', 1.70, 10)";
	session.once << "INSERT INTO PERSON (PERSON_ID, PERSON_NAME, PERSON_HEIGHT, PERSON_FINGERS) "
		     << "VALUES (10, 'Bugs Bunny', 1.80, 10)";

	session.once << "INSERT INTO ADDRESS (ADDRESS_ID, ADDRESS_NUMBER, ADDRESS_STREET) "
		     << "VALUES (1, 1, 'Infinite Loop')";
	session.once << "INSERT INTO ADDRESS (ADDRESS_ID, ADDRESS_NUMBER, ADDRESS_STREET) "
		     << "VALUES (2, 10, 'Ten Ways')";
	session.once << "INSERT INTO ADDRESS (ADDRESS_ID, ADDRESS_NUMBER, ADDRESS_STREET) "
		     << "VALUES (3, 50, 'Dairy Road')";
}
