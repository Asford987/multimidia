#pragma once
#include <fstream>
#include "compressor.h"

class Encoder : public Compressor
{
	int low, high;
	int opposite_bits;
	int buffer;
	int	bits_in_buf;

	std::ifstream in;
	std::ofstream out;
public:
	Encoder(void);
	
	void write_bit( int bit);
	void output_bits(int bit);
	void end_encoding(void);
	void encode_symbol(int symbol);
	void encode(char *infile, char *outfile);

};
