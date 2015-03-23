#pragma once
#include "../meta_set.hxx"
#include "../../traits/rtuple.hxx"
#include "../../traits/to_rtuple.hxx"
#include "../../traits/to_rtie.hxx"
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include <bitset>

namespace moneta { namespace container {

	namespace detail {

		template <class Master, class Entity>
		struct bitset_load_tracker_impl {
			typedef bitset_load_tracker_impl this_type;

			struct entry {
				typedef typename std::bitset<
					boost::mpl::size<
						typename moneta::traits::rtuple<Entity>::type
					>::value
				> state_type;

				state_type load_state;

				entry() {
				}

				entry(const Entity& /*entity*/) {
					// TODO
				}

				std::string to_string() const {
					return load_state.to_string();
				}

				bool operator<(const state_type& /*rhs*/) const {
					// TODO
					return false;
				}
			};

			bool is_loaded(const int /*ordinal*/) const {
				// TODO
				return 1;
			}
		};

	}

	template <class Entity>
	struct bitset_load_tracker {
		typedef typename std::bitset<
			boost::mpl::size<
				typename moneta::traits::rtuple<Entity>::type
			>::value
		> state_type;

	public:
		state_type load_state;

	public:
		bitset_load_tracker() {
		}

		bitset_load_tracker(const bool all_loaded) {
			if (all_loaded) {
				load_state.set();
			}
		}

		bool loaded(const size_t ordinal) const {
			return load_state[ordinal];
		}

		void loaded(const size_t ordinal, const bool value) {
			load_state[ordinal] = value;
		}
	};

	template <class Entity>
	struct bitset_load_tracker2 : boost::mpl::lambda<
		detail::bitset_load_tracker_impl<boost::mpl::_1, Entity>
	>::type {};

}}
