#pragma once
#include <boost/fusion/algorithm/iteration/for_each_fwd.hpp>
#include <vector>

namespace moneta {

	typedef unsigned long long member_ptr_interceptor_t;

	namespace detail {

		template <typename MemberPointerType, typename InterceptorType = member_ptr_interceptor_t>
		union memptr_hacker {
			typedef memptr_hacker<MemberPointerType, InterceptorType> this_type;

			MemberPointerType member_pointer;
			InterceptorType interceptor;

			memptr_hacker(MemberPointerType member_pointer_) {
				memset(this, 0, sizeof(this_type));
				member_pointer = member_pointer_;
			}

			operator InterceptorType() const {
				return interceptor;
			}
		};

		template <typename T>
		member_ptr_interceptor_t get_memptr_fingerprint(T t) {
			return detail::memptr_hacker<T>(t);
		}

		struct memptr_fingerprinter {
			std::vector<member_ptr_interceptor_t>& _vector;

			memptr_fingerprinter(std::vector<member_ptr_interceptor_t>& vector)
			 : _vector(vector) {
			}

			template <typename R, class K>
			void operator()(R K::*x) const {
				_vector.push_back(get_memptr_fingerprint<>(x));
			}
		};

		template <typename EntityType>
		struct member_pointer_indices {
			static inline const std::vector<member_ptr_interceptor_t>& get() {
				static std::vector<member_ptr_interceptor_t> result;
				if (result.empty()) {
					boost::fusion::for_each(traits::member_pointers<EntityType>::get(), memptr_fingerprinter(result));
				}

				return result;
			}
		};

	} // namespace detail

	template <typename R, class K>
	inline const size_t member_ordinal(R K::* memptr) {
		const std::vector<member_ptr_interceptor_t>& fingerprints =
			detail::member_pointer_indices<K>::get();

		std::vector<member_ptr_interceptor_t>::const_iterator itr = std::find(
			fingerprints.begin(),
			fingerprints.end(),
			detail::get_memptr_fingerprint(memptr)
		);

		assert(itr != fingerprints.end());
		return std::distance(fingerprints.begin(), itr);
	}

}
