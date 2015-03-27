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

namespace moneta { namespace codec { namespace aux {

	namespace detail {

		class null_iterator {
			size_t _count;
		public:
			null_iterator()
				: _count(0) {}

			size_t count() const {
				return _count;
			}

			bool operator==(const null_iterator&) const {
				return false;
			}
			bool operator!=(const null_iterator&) const {
				return true;
			}
			void operator+=(const size_t size) {
				_count += size;
			}

			null_iterator& operator++() {
				++_count; return *this;
			}
			null_iterator& operator++(int) {
				++_count; return *this;
			}

			char& operator*() {
				static char ch;
				return ch;
			}
		};

	}

	template <class Encoder, class Entity>
	size_t get_encoded_size(const Entity& entity) {
		return Encoder()(detail::null_iterator(), detail::null_iterator(), entity);
	}

	template <class Encoder, class Entity, class State>
	size_t get_encoded_size(const Entity& entity, State& state = algorithm::detail::no_state()) {
		State tmp = state;
		return Encoder()(detail::null_iterator(), detail::null_iterator(), entity, tmp);
	}

}}}
