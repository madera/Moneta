#pragma once
#include <iostream>
#include <iomanip>

inline void hexdump_line(const unsigned char* data, const size_t size, std::ostream& stream = std::cout) {
	const unsigned char* buffer = data;

	size_t addr = 0;

	bool done = false;
	while (!done) {
		int available = size - addr;
		if (available > 16) {
			available = 16;
		} else {
			done = true;
		}

		stream << "[ " << std::setw(6);
		stream.fill('0');
		stream << std::hex << addr << "  ";

		bool printed_separator = false;
		for (int n = 0; n < 16; n++) {
			if (n < available) {
				stream << std::hex << std::setw(2);
				stream.fill('0');
				stream << (int) (buffer[addr + n]);
				if (n == 7) {
					stream << " -";
					printed_separator = true;
				}

			} else {
				stream << "  ";
			}

			stream << " ";
		}

		if (!printed_separator) {
			stream << "  ";
		}

		stream.fill(' ');
		stream << " ";

		for (int n = 0; n < 16; n++) {
			if (n < available) {
				char c = buffer[addr + n];
				stream << ((c >= 32 && c < 127)? c : '.');
			} else {
				stream << ' ';
			}
		}

		stream << " ]\n";
		addr += 16;
	}
}

inline void hexdump(const unsigned char* data, const size_t size, std::ostream& stream = std::cout) {
	const unsigned char* buffer = data;

	stream << "\n";
	stream << "[=============================================================================]\n";
	stream << "[ Capacity = 0x" << std::setw(8);
	stream.fill('0');
	stream << std::hex << size;

	// Output the max size of a uint32 decimal value ('4294967296').
	stream << " (" << std::setw(10);
	stream.fill('0');
	stream << std::dec << size;
	stream << " dec)";

	stream << ",  Size = 0x" << std::setw(8);
	stream.fill('0');
	stream << std::hex << size;

	// Output the mas size of a uint32 decimal value ('4294967296').
	stream << " (" << std::setw(10);
	stream.fill('0');
	stream << std::dec << size;
	stream << " dec) ]\n";
	stream << "[=============================================================================]\n";
	stream << "[ Offset  x0 x1 x2 x3 x4 x5 x6 x7   x8 x9 xA xB xC xD xE xF       ASCII       ]\n";
	stream << "[-----------------------------------------------------------------------------]\n";

	size_t addr = 0;

	bool done = false;
	while (!done) {
		int available = size - addr;
		if (available > 16) {
			available = 16;
		} else {
			done = true;
		}

		stream << "[ " << std::setw(6);
		stream.fill('0');
		stream << std::hex << addr << "  ";

		bool printed_separator = false;
		for (int n = 0; n < 16; n++) {
			if (n < available) {
				stream << std::hex << std::setw(2);
				stream.fill('0');
				stream << (int) (buffer[addr + n]);
				if (n == 7) {
					stream << " -";
					printed_separator = true;
				}

			} else {
				stream << "  ";
			}

			stream << " ";
		}

		if (!printed_separator) {
			stream << "  ";
		}

		stream.fill(' ');
		stream << " ";

		for (int n = 0; n < 16; n++) {
			if (n < available) {
				char c = buffer[addr + n];
				stream << ((c >= 32 && c < 127)? c : '.');
			} else {
				stream << ' ';
			}
		}

		stream << " ]\n";
		addr += 16;
	}

	stream << "[=============================================================================]\n"
		<< std::dec;
}
