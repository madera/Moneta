#pragma once
//#include "../traits/detail/is_functor_callable.hxx"
#include <boost/function.hpp>

namespace moneta { namespace codec {

	namespace detail {

		// Tabs
		// TODO: Move somewhere?

		#define MONETA_CODEC_TABS_MAX 32

		namespace {
			char* tabs_cstr = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
		}

		template <int N>
		struct tabs {
			enum  { size = MONETA_CODEC_TABS_MAX };
			static const char* get() {
				return &tabs_cstr[size - (N % size)];
			}
		};

		// Tabs ends

		template <class InputIterator, class OutputIterator>
		int copy(InputIterator first, InputIterator last, OutputIterator begin, OutputIterator end) {
			int written = 0;
		
			for ( ; first != last && begin != end; ++written) {
				*begin++ = *first++;
			}

			// Check if there was data left.
			if (first != last && begin == end) {
				return 0 - std::distance(first, last);
			}

			return written;
		}

		template <class Iterator>
		int copy(const std::string& string, Iterator begin, Iterator end) {
			return copy(std::begin(string), std::end(string), begin, end);
		}

		// TODO: Validate this non-reference Iterator state.
		//
		template <class Iterator>
		struct ostringstream {
			typedef ostringstream this_type;

			bool _good;
			size_t _total_written;

			Iterator _begin;
			Iterator _end;

			ostringstream(Iterator begin, Iterator end)
			 : _begin(begin), _end(end), _good(true), _total_written(0) {}

			this_type& operator<<(boost::function<int (Iterator, Iterator)> callable) {
				if (_good) {
					int result = callable(_begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			this_type& operator<<(const char* string) {
				if (_good) {
					// XXX: strlen() is slow!!
					// TODO: Implement fast c-string copy.
					int result = copy(string, string + strlen(string), _begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			this_type& operator<<(const char c) {
				if (_good) {
					if (_begin != _end) {
						*_begin++ = c;
						++_total_written;
					} else {
						_good = false;
						_total_written = -1;
					}
				}

				return *this;
			}

			this_type& operator<<(const std::string& string) {
				if (_good) {
					int result = copy(string, _begin, _end);
					if (result > 0) {
						_begin += result;
						_total_written += result;
					} else if (result < 0) {
						_good = false;
						_total_written = result;
					}
				}

				return *this;
			}

			operator int() const {
				return _total_written;
			}
		};

		template <class Iterator>
		ostringstream<Iterator> make_ostringstream(Iterator begin, Iterator end) {
			return ostringstream<Iterator>(begin, end);
		}
	}

}}
