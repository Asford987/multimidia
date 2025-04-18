#include "decoder.h"
#include <iostream>

Decoder::Decoder()
	: buffer(0), bits_in_buf(0), end_decoding(false),
	  low(0), high(MAX_VALUE), value(0)
{
}

void Decoder::load_first_value()
{
	value = 0;
	for (int i = 0; i < CODE_VALUE; ++i)
		value = 2 * value + get_bit();
}

void Decoder::decode(char* infile, char* outfile)
{
	in.open(infile, std::ios::binary);
	out.open(outfile, std::ios::binary);

	if (!in.is_open() || !out.is_open())
	{
		std::cerr << "Error: Can't open file" << std::endl;
		return;
	}

	load_first_value();

	while (true)
	{
		if (end_decoding)
			break;

		int symbol_index = decode_symbol();

		if (symbol_index == EOF_SYMBOL)
			break;

		char ch = static_cast<char>(index_to_char[symbol_index]);
		out.put(ch);
		update_tables(symbol_index);
	}

	std::cout << "Decoding is done" << std::endl;
	in.close();
	out.close();
}

int Decoder::decode_symbol()
{
	int range = high - low;
	int cum = (((value - low + 1) * cum_freq[0] - 1) / range);

	int symbol_index = 1;
	while (cum_freq[symbol_index] > cum)
		symbol_index++;

	high = low + (range * cum_freq[symbol_index - 1]) / cum_freq[0];
	low = low + (range * cum_freq[symbol_index]) / cum_freq[0];

	while (true)
	{
		if (high < HALF){}
		else if (low >= HALF)
		{
			value -= HALF;
			low -= HALF;
			high -= HALF;
		}
		else if (low >= FIRST_QTR && high < THIRD_QTR)
		{
			value -= FIRST_QTR;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		}
		else
		{
			break;
		}

		low *= 2;
		high *= 2;
		value = 2 * value + get_bit();
	}

	return symbol_index;
}

int Decoder::get_bit()
{
	if (bits_in_buf == 0)
	{
		buffer = in.get();

		if (in.eof() || buffer == EOF)
		{
			end_decoding = true;
			return 0;
		}

		bits_in_buf = 8;
	}

	int bit = buffer & 1;
	buffer >>= 1;
	--bits_in_buf;

	return bit;
}
