#pragma once
#include <moneta/pp/describe_entity.hxx>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/optional.hpp>
#include "Address.hxx"
#include "Dog.hxx"

MONETA_DEFINE_AND_DESCRIBE_ENTITY(
	Customer,
	((std::string,                                Name       ))
	((boost::gregorian::date,                     DOB        ))
	((boost::optional<Address>,                   HomeAddress))
	((boost::optional<Address>,                   WorkAddress))
	((boost::optional<int>,                       Rating     ))
	((boost::optional<std::vector<Dog> >,         Dogs       ))
	((boost::optional<std::vector<std::string> >, Tags       ))
)
