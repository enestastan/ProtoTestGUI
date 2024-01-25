#include <stdio.h>
#include "ParserEntry.pb.h"

static const int32_t BUFFER_SIZE = 100;

int main()
{
	Sample::ParserEntry message;
	uint8_t buffer[BUFFER_SIZE];

	message.set_uinteger(150);
	message.set_fixed64double(151);
	message.set_fixed32float(152);

	message.SerializeToArray(buffer, BUFFER_SIZE);

	for (int i = 0; i < message.ByteSizeLong(); i++)
	{
		std::cout << std::hex << *(buffer + i);
		// printf("%X", *(buffer + i));
	}

	for (int i = 0; i < message.ByteSizeLong(); i++)
	{
		fprintf(stderr, "%.2X", *(buffer + i));
	}

	fprintf(stderr, "\n");

	return 0;
}