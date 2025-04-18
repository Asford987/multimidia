#pragma once

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <array>

constexpr int CODE_VALUE = 16;
constexpr int MAX_VALUE = ((1 << CODE_VALUE) - 1);
constexpr int MAX_FREQ = 16383;

constexpr int FIRST_QTR = (MAX_VALUE / 4 + 1);
constexpr int HALF = (2 * FIRST_QTR);
constexpr int THIRD_QTR = (3 * FIRST_QTR);

constexpr int NO_OF_CHARS = 256;
constexpr int EOF_SYMBOL = (NO_OF_CHARS + 1);
constexpr int NO_OF_SYMBOLS = (NO_OF_CHARS + 1);

class Compressor
{
public:
	std::array<unsigned char, NO_OF_SYMBOLS> index_to_char;
	std::array<int, NO_OF_CHARS> char_to_index;
	std::array<int, NO_OF_SYMBOLS + 1> cum_freq;
	std::array<int, NO_OF_SYMBOLS + 1> freq;

	Compressor(void);
	void update_tables(int sym_index);

};
