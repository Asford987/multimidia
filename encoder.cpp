#include "encoder.h"
#include <iostream>

Encoder::Encoder()
	: buffer(0), bits_in_buf(0), low(0), high(MAX_VALUE), opposite_bits(0)
{
}

void Encoder::encode(char* infile, char* outfile)
{
	in.open(infile, std::ios::binary);
	out.open(outfile, std::ios::binary);
	
	if (!in.is_open() || !out.is_open())
	{
		std::cerr << "Error: Can't open file" << std::endl;
		return;
	}

	while (true)
	{
		int ch = in.get();
		if (in.eof())
		{
			std::cout << "Encoding is done" << std::endl;
			break;
		}

		int symbol = char_to_index[static_cast<unsigned char>(ch)];
		encode_symbol(symbol);
		update_tables(symbol);
	}

	encode_symbol(EOF_SYMBOL);
	end_encoding();

	in.close();
	out.close();
}

void Encoder::encode_symbol(int symbol)
{
	int range = high - low;

	high = low + (range * cum_freq[symbol - 1]) / cum_freq[0];
	low  = low + (range * cum_freq[symbol]) / cum_freq[0];

	while (true)
	{
		if (high < HALF)
		{
			output_bits(0);
		}
		else if (low >= HALF)
		{
			output_bits(1);
			low -= HALF;
			high -= HALF;
		}
		else if (low >= FIRST_QTR && high < THIRD_QTR)
		{
			opposite_bits++;
			low -= FIRST_QTR;
			high -= FIRST_QTR;
		}
		else
		{
			break;
		}

		low *= 2;
		high *= 2;
	}
}

void Encoder::end_encoding()
{
	opposite_bits++;

	if (low < FIRST_QTR)
		output_bits(0);
	else
		output_bits(1);

	out.put(static_cast<char>(buffer >> bits_in_buf));
}

void Encoder::output_bits(int bit)
{
	write_bit(bit);
	while (opposite_bits > 0)
	{
		write_bit(!bit);
		opposite_bits--;
	}
}

void Encoder::write_bit(int bit)
{
	buffer >>= 1;
	if (bit)
		buffer |= 0x80;

	bits_in_buf++;

	if (bits_in_buf == 8)
	{
		out.put(static_cast<char>(buffer));
		bits_in_buf = 0;
	}
}
