-- [===========================================================================]
-- [                                M o n e t a                                ]
-- [---------------------------------------------------------------------------]
-- [                                                                           ]
-- [                          Copyright (C) 2005-2015                          ]
-- [                      Rodrigo Madera <madera@acm.org>                      ]
-- [                                                                           ]
-- [---------------------------------------------------------------------------]
-- [         Distributed under the Boost Software License, Version 1.0         ]
-- [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
-- [===========================================================================]

CREATE TABLE PERSON (
	PERSON_ID      INTEGER PRIMARY KEY,
	PERSON_NAME    TEXT    NOT NULL,
	PERSON_HEIGHT  REAL,
	PERSON_FINGERS INTEGER
);

CREATE TABLE DOG (
	DOG_OWNER   TEXT    NOT NULL,
	DOG_ID      INTEGER NOT NULL,
	DOG_NAME    TEXT    NOT NULL,
	PRIMARY KEY (DOG_OWNER, DOG_ID)
);

CREATE TABLE ADDRESS (
	ADDRESS_ID      INTEGER PRIMARY KEY,
	ADDRESS_NUMBER  INTEGER,
	ADDRESS_STREET  TEXT    NOT NULL
);

CREATE TABLE CAT (
	CAT_ID      INTEGER PRIMARY KEY,
	CAT_NAME    TEXT    NOT NULL,
	CAT_ADDRESS INTEGER NOT NULL REFERENCES ADDRESS (ID)
);

-- CREATE TABLE COMPOSITE (
--	COMPOSITE_ID     INTEGER PRIMARY KEY,
--	COMPOSITE_PERSON INTEGER NOT NULL REFERENCES PERSON (ID),
--	COMPOSITE_DOG    INTEGER NOT NULL REFERENCES DOG (DOG_OWNER, DOG_ID)
--);
