#pragma once
#include <moneta/pp/sql_entity.hxx>

//
// Fictitious simple network protocol handshake.
//

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Handshake,
	((int, Magic      ))
	((int, Category   ))
	((int, Description))
	((int, Version    ))
)
