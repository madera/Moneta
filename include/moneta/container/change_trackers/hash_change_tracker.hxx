#pragma once
#include <boost/array.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>
#include <boost/fusion/include/boost_array.hpp>
#include "../../sql/traits/db_tuple.hxx"
#include "../../sql/traits/to_db_tuple.hxx"
#include "../../sql/traits/to_db_tie.hxx"

namespace moneta { namespace container {

	template <class EntityType>
	struct hash_change_tracker {

		typedef boost::array<
			size_t,
			boost::mpl::size<
				typename sql::traits::db_tuple<EntityType>::type
			>::value
		> state_type;

	private:
		struct accumulator {
			typedef const size_t result_type;

			template<typename T>
			result_type operator()(const size_t previous, const T& x) const {
				// FIXME: Make this better.
				return (previous ^ x) << 4 & (x & 0x0f);
			}
		};

		template <typename T>
		static
		typename boost::enable_if<traits::is_entity<T>, size_t>::type
		hash(const T x) {
			state_type hashes;
			boost::fusion::copy(boost::fusion::transform(to_tie(x), std_hasher()), hashes);
			return boost::fusion::accumulate(hashes, 0, accumulator());
		}

		template <typename T>
		static
		typename boost::disable_if<traits::is_entity<T>, size_t>::type
		hash(const T x) {
			return std::hash<T>()(x);
		}

		struct std_hasher {
			typedef const size_t result_type;

			template <typename T>
			const result_type operator()(const T value) const {		
				return hash(value);
			}
		};

		template <class EntityType>
		typename state_type hash_entity(const EntityType& entity) {
			state_type result;

			boost::fusion::copy(
				boost::fusion::transform(
					sql::traits::to_db_tie(entity),
					std_hasher()
				),
				result
			);

			return result;
		}

	public:
		state_type change_state;
	public:
		hash_change_tracker() {
			change_state.assign(0);
		}

		hash_change_tracker(const EntityType& entity) {
			update(entity);
		}

		void update(const EntityType& entity) {
			change_state = hash_entity(entity);
		}

		const bool dirty() const {
	//		flags = 1;
//			return hash_entity;
		}
	};

}}
