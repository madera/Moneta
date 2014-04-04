#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../../sql/traits/rtuple.hxx"
#include "../../sql/traits/to_rtuple.hxx"
#include "../../sql/traits/to_rtie.hxx"

// XXX: Move somewhere. The trash, maybe?
template <class T, class U, class X, const int Y>
std::basic_ostream<T, U>& operator<<(std::basic_ostream<T, U>& output, const boost::array<X, Y>& rhs) {
	typedef typename boost::array<X, Y>::const_iterator const_iterator_type;

	const_iterator_type last = std::begin(rhs) + rhs.size() - 1;
	const_iterator_type  itr = std::begin(rhs);
	for ( ; itr != std::end(rhs); ++itr) {
		output << *itr
		       << (itr != last? ", " : "");
	}

	return output;
}

namespace moneta { namespace container {

	template <class EntityType>
	struct hash_change_tracker {

		typedef boost::array<
			size_t,
			boost::mpl::size<
				typename sql::traits::rtuple<EntityType>::type
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
		//	return std::hash<T>()(x);
		//}

		struct std_hasher {
			typedef const size_t result_type;

			template <typename T>
			const result_type operator()(const T value) const {		
				return std::hash<T>()(value);
				//return hash(value);
			}
		};

		//template <class EntityType>
		//typename state_type hash_entity(const EntityType& entity) {
		//	state_type result;

		//	boost::fusion::copy(
		//		boost::fusion::transform(
		//			sql::traits::to_rtie(entity),
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
		typedef typename sql::traits::rtuple<EntityType>::type tuple_type;
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

		const bool dirty(const tuple_type& tuple) const {
			return change_state != hash_tuple(tuple);
		}
	};

}}
