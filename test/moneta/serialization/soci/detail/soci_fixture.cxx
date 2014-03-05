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
		"	ADDRESS_NUMBER  INTEGER,			    \n"
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
}
