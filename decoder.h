#pragma once
#include <fstream>
#include "compressor.h"

class Decoder : public Compressor
{
	int low, high;
	int value;

	int buffer;
	int	bits_in_buf;
	bool end_decoding;

	std::ifstream in;
	std::ofstream out;
public:
	Decoder(void);

	void load_first_value(void);
	void decode(char *infile, char *outfile);
	int decode_symbol(void);
	int get_bit(void);
};
