namespace moneta { namespace codec { namespace io {

	struct is_whitespace {		
		bool operator()(const char x) const {
			return std::isspace(x) != 0;
		}
	};

	template <class Iterator>
	int consume_whitespaces(Iterator begin, Iterator end) {
		return consume_while(begin, end, is_whitespace());
	}

}}}
