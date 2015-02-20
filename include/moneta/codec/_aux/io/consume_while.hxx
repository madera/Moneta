#include <cctype>

namespace moneta { namespace codec { namespace io {

	template <class Iterator, class Predicate>
	int consume_while(Iterator begin, Iterator end, const Predicate& predicate) {

		Iterator itr;
		for (itr = begin; itr != end && predicate(*itr); ++itr)
			;

		return std::distance(begin, itr);
	}

}}}
