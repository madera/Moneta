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
#include "../../algorithm/hash.hxx"
#include "../../traits/rtuple.hxx"
#include "../../traits/to_tie.hxx"
#include "../../traits/to_rtuple.hxx"
#include "../../traits/to_rtie.hxx"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include <boost/format.hpp>
#include <boost/functional/hash.hpp>

// XXX: Move somewhere. The trash, maybe?
template <class T, class U, class X, const size_t Y>
std::basic_ostream<T, U>& operator<<(std::basic_ostream<T, U>& output, const boost::array<X, Y>& rhs) {
	typedef typename boost::array<X, Y>::const_iterator const_iterator_type;

	const_iterator_type last = boost::begin(rhs) + rhs.size() - 1;
	const_iterator_type  itr = boost::begin(rhs);
	for ( ; itr != boost::end(rhs); ++itr) {
		output << *itr
		       << (itr != last? ", [OLD] " : "");
	}

	return output;
}

namespace moneta { namespace container {

	template <class Entity>
	struct hash_change_tracker {

		typedef boost::array<
			size_t,
			boost::mpl::size<
				typename traits::rtuple<Entity>::type
			>::value
		> state_type;

	private:
		//struct accumulator {
		//	typedef const size_t result_type;

		//	template<typename T>
		//	result_type operator()(const size_t previous, const T& x) const {
		//		// FIXME: Make this better.
		//		return (previous ^ x) << 4 & (x & 0x0f);
		//	}
		//};

		//template <typename T>
		//static
		//typename boost::enable_if<traits::is_entity<T>, size_t>::type
		//hash(const T x) {
		//	state_type hashes;
		//	boost::fusion::copy(boost::fusion::transform(to_tie(x), std_hasher()), hashes);
		//	return boost::fusion::accumulate(hashes, 0, accumulator());
		//}

		//template <typename T>
		//static
		//typename boost::disable_if<traits::is_entity<T>, size_t>::type
		//hash(const T x) {
		//	return boost::hash<T>()(x);
		//}

		struct std_hasher {
			typedef size_t result_type;

			template <typename T>
			result_type operator()(const T value) const {
				return boost::hash<T>()(value);
				//return hash(value);
			}
		};

		//template <class Entity>
		//typename state_type hash_entity(const Entity& entity) {
		//	state_type result;

		//	boost::fusion::copy(
		//		boost::fusion::transform(
		//			traits::to_rtie(entity),
		//			std_hasher()
		//		),
		//		result
		//	);

		//	return result;
		//}

		template <class Tuple>
		state_type hash_tuple(const Tuple& tuple) const {
			state_type hashes;
			boost::fusion::copy(boost::fusion::transform(tuple, std_hasher()), hashes);
			return hashes;
		}
	public:
		state_type change_state;
		typedef typename traits::rtuple<Entity>::type tuple_type;
	public:
		hash_change_tracker() {
			change_state.assign(0);
		}

		hash_change_tracker(const tuple_type& tuple) {
			update(tuple);
		}

		void update(const tuple_type& tuple) {
			change_state = hash_tuple(tuple);
		}

		bool dirty(const tuple_type& tuple) const {
			return change_state != hash_tuple(tuple);
		}
	};

	namespace detail {

		struct std_hasher {
			typedef size_t result_type;

			template <typename T>
			result_type operator()(const T value) const {		
				return algorithm::hash(value);
			}
		};

		template <class Master, class Entity>
		struct hash_change_tracker_impl {
			typedef hash_change_tracker_impl this_type;

			struct entry {
				typedef boost::array<
					size_t,
					boost::mpl::size<
						typename traits::rtuple<Entity>::type
					>::value
				> state_type;

				state_type hash;

				entry() {}
				entry(const Entity& entity) { update(entity); }

				std::string to_string() const {
					std::ostringstream oss;

					typename state_type::const_iterator itr = boost::begin(hash);
					typename state_type::const_iterator last = itr + hash.size() - 1;

					for ( ; itr != boost::end(hash); ++itr) {
						oss << boost::format("0x%|08x|") % *itr
						    << (itr != last? ", " : "");
					}

					return oss.str();
				}

				void update(const Entity& entity) {
					Entity tmp = entity; // XXX: HACK: Until const tie is done.
					update(moneta::traits::to_tie(tmp));
				}

				void update(const typename moneta::traits::tie<Entity>::type& tie) {
					boost::fusion::copy(
						boost::fusion::transform(tie, std_hasher()),
						hash
					);
				}

				bool dirty() const {
					return false;
				}
			};
		};
	
	} // namespace detail

	template <class Entity>
	struct hash_change_tracker2 : boost::mpl::lambda<
		detail::hash_change_tracker_impl<boost::mpl::_1, Entity>
	>::type {
		typedef Entity entity_type;
	};

}}
