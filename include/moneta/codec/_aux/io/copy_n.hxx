#pragma once
#include <iterator>

namespace moneta { namespace codec {

	namespace io {

		namespace detail {
			template <typename Iterator>
			struct is_random_access_iterator : boost::is_same<
				typename std::iterator_traits<Iterator>::iterator_category,
				std::random_access_iterator_tag
			> {};
		}

		namespace detail {

			struct copy_n_impl {
				//
				// Slow generic single-pass InputIterator implementation.
				//
				template <typename InputIterator, typename Size, typename OutputIterator>
				typename boost::disable_if<
					detail::is_random_access_iterator<InputIterator>,
					Size
				>::type
				operator()(
					InputIterator& next, InputIterator end,
					const Size count,
					OutputIterator result
				) const {
					Size consumed = 0;

					while (next != end && consumed < count) {
						*result++ = *next++;
						++consumed;
					}

					const Size left = count - consumed;
					return (left == 0)? consumed : 0 - left;
				}

				//
				// RandomAccessIterator optimization.
				//
				template <typename InputIterator, typename Size, typename OutputIterator>
				typename boost::enable_if<
					detail::is_random_access_iterator<InputIterator>,
					Size
				>::type
				operator()(
					InputIterator& next, InputIterator end, const Size count, OutputIterator result
				) const {
					const typename std::iterator_traits<
						InputIterator
					>::difference_type available = std::distance(next, end);

					const Size usable = (available < count)? available : count;
					std::copy(next, next + usable, result); // XXX
					next += usable;

					const Size left = count - usable;
					return (left == 0)? usable : 0 - left;
				}
			};

		}

		// Copies up to 'count' elements from input range to output range.
		// If there's no room, copies the most it can and returns the missing element count needed as a negative.
		template <typename InputIterator, typename Size, typename OutputIterator>
		Size copy_n(InputIterator& next, InputIterator end, const Size count, OutputIterator result) {
			return detail::copy_n_impl()(next, end, count, result);
		}

	}

}}
