// [===========================================================================]
// [                                M o n e t a                                ]
// [---------------------------------------------------------------------------]
// [                                                                           ]
// [                          Copyright (C) 2005-2015                          ]
// [                      Rodrigo Madera <madera@acm.org>                      ]
// [                                                                           ]
// [---------------------------------------------------------------------------]
// [         Distributed under the Boost Software License, Version 1.0         ]
// [ Read accompanying LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt ]
// [===========================================================================]

#pragma once
#include "../meta_set.hxx"
#include "../../traits/rtuple.hxx"
#include "../../traits/to_rtuple.hxx"
#include <boost/fusion/include/io.hpp>

namespace moneta { namespace container {

	namespace detail {

		template <class Master, class Entity>
		struct rtuple_data_impl {
			typedef Master base_type;
			typedef Entity entity_type;

			typedef rtuple_data_impl this_type;

			struct entry {
				typedef typename traits::rtuple<
					Entity
				>::type state_type;

				state_type data;

				entry() {}

				entry(const Entity& entity) {
					data = traits::to_rtuple(entity);
				}

				std::string to_string() const {
					using namespace boost::fusion;
					std::ostringstream oss;
					oss << tuple_open("tuple={ ")
					    << tuple_close(" }")
					    << tuple_delimiter(", ")
					    << data;
			
					return oss.str();
				}
			};
		};

	}

	template <class Entity>
	struct rtuple_data : boost::mpl::lambda<
		detail::rtuple_data_impl<boost::mpl::_1, Entity>
	>::type {
		typedef Entity entity_type;
	};

}}
