#include "encoder.h"
#include "decoder.h"
#include <iostream>

int main ( int argc, char **argv)
{
	if (argc < 4)
	{
		std::cout << "Using: name_programm <input_of_file> <output_file>" << std::endl;
		return -1;
	}
	if (argv[1][0] == 'e')
	{
		Encoder encoder;
		encoder.encode(argv[2], argv[3]);
	}
	else if (argv[1][0] == 'd')
	{
		Decoder decoder;
		decoder.decode(argv[2], argv[3]);
	}
	return 0;
}