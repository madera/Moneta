namespace moneta { namespace codec { namespace io {

	template <class Iterator, class OutputIterator, class Predicate>
	int copy_while(Iterator begin, Iterator end, OutputIterator result, const Predicate& predicate) {

		Iterator itr;
		for (itr = begin; itr != end && predicate(*itr); ++itr) {
			*result++ = *itr;
		}

		return std::distance(begin, itr);
	}

	template <class Iterator, class OutputIterator, class Predicate>
	int copy_while(
		Iterator begin, Iterator end, OutputIterator result_begin, OutputIterator result_end,
		const Predicate& predicate
	) {
		Iterator itr;
		for (itr = begin; itr != end && result_begin != result_end && predicate(*itr); ++itr) {
			*result_begin++ = *itr;
		}

		return std::distance(begin, itr);
	}

}}}
