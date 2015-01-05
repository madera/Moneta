#pragma once

namespace moneta { namespace traits {

	template <class Entity, class Enable = void>
	struct pk;

	template <class Entity>
	struct pk_members;

}}
