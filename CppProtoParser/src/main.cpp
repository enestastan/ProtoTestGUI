#include <stdio.h>

#include "ParserEntry.pb.h"

static const int32_t BUFFER_SIZE = 100;

uint8_t parseVarint(uint8_t* buffer, size_t& result) {
	std::vector<uint8_t> byteList;

	uint8_t offset = 0;
	while ((*(buffer + offset) & 0x80) == 0x80) {
		byteList.push_back((*(buffer + offset) & 0x7F));	// strip sign bit
		offset++;
	}

	// add last byte whose sign bit is not set
	byteList.push_back((*(buffer + offset) & 0x7F));
	offset++;

	result = 0;
	for (ssize_t i = static_cast<ssize_t>(byteList.size() - 1); i >= 0; i--) {
		// concatenate 7-bit valued bytes
		result += (static_cast<size_t>(byteList.at(static_cast<size_t>(i))) << (7 * i));
	}

	return offset;
}

int main() {
	Sample::ParserEntry message;
	uint8_t buffer[BUFFER_SIZE];

	message.set_value1(128);
	message.set_value2(512);
	message.set_value3(2048);
	message.set_value4(0x7FFFFFFF);
	message.set_value5(0);

	memset(buffer, 0, BUFFER_SIZE);
	message.SerializeToArray(buffer, BUFFER_SIZE);

	size_t varintValue;

	size_t msgSize = 0;
	while (msgSize != message.ByteSizeLong()) {
		msgSize += parseVarint(buffer + msgSize, varintValue);
		fprintf(stderr, "Varint Value: %zu\n", varintValue);
	}

	for (size_t i = 0; i < message.ByteSizeLong(); i++) {
		std::cout << std::hex << *(buffer + i);
		// printf("%X", *(buffer + i));
	}

	for (size_t i = 0; i < message.ByteSizeLong(); i++) {
		fprintf(stderr, "%.2X", *(buffer + i));
	}
	fprintf(stderr, "\n");

	return 0;
}
