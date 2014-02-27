#pragma once
#include <iostream>
#include "shell/shell_codec.hxx"

// XXX: Put in a cxx.
static int const output_format_flag = std::ios_base::xalloc();

enum {
	fmt_textual,
	fmt_xml,
	fmt_json
};

#define FUCKER_IOMANIP(name) \
template <typename T, typename U> \
std::basic_ios<T, U>& name(std::basic_ios<T, U>& stream) { \
	stream.iword(output_format_flag) = fmt_##name; \
	return stream; \
}

FUCKER_IOMANIP(textual)
FUCKER_IOMANIP(xml)
FUCKER_IOMANIP(json)

template <typename T, typename U>
std::basic_ios<T, U>& set_format(std::basic_ios<T, U>& stream, const long flags) {
	stream.iword(output_format_flag) = flags;
	return stream;
}

template <typename T, typename U, class EntityType>
std::basic_ostream<T, U>& operator<<(std::basic_ostream<T, U>& output, const EntityType& rhs) {
	const long format = output.iword(output_format_flag);
	switch (format) {
	case fmt_textual:
		output << fucker::serialization::textual::to_line(rhs);
		break;
	case fmt_xml:
		output << "XXX: OUTPUT: xml_format_here\n";
		break;
	case fmt_json:
		output << "XXX: OUTPUT: json_format_here\n";
		break;
	default:
		assert(false);
	}

	return output;
}

template <typename T, typename U, class EntityType>
std::basic_istream<T, U>& operator>>(std::basic_istream<T, U>& input, EntityType& rhs) {
	const long format = input.iword(output_format_flag);
	switch (format) {
	case fmt_textual: {
			std::string line;
			std::getline(input, line);
			rhs = fucker::serialization::textual::from_line<EntityType>(line);
		} break;
	case fmt_xml:
		std::cerr << "XXX: INPUT: xml_format_here\n" << std::endl;
		break;
	case fmt_json:
		std::cerr << "XXX: INPUT: json_format_here\n" << std::endl;
		break;
	default:
		assert(false);
	}

	return input;
}
