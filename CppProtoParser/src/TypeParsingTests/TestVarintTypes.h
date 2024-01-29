#pragma once
#include "ParserUtilities/StreamUtilities.h"
#include "TestMessages.pb.h"

static const int32_t BUFFER_SIZE = 100;

namespace Test {

static bool TestVarintTypes() {
	::TestMessages::VarintTypes message;
	uint8_t buffer[BUFFER_SIZE];

	message.set_testint32(-1);
	message.set_testint64(-1);
	message.set_testuint32(0xFFFFFFFF);
	message.set_testuint64(0xFFFFFFFFFFFFFFFF);
	message.set_testsint32(-1);
	message.set_testsint64(-1);
	message.set_testbool(0);
	message.set_testenum(TestMessages::TestEnum::VALUE_3);

	memset(buffer, 0, BUFFER_SIZE);
	message.SerializeToArray(buffer, BUFFER_SIZE);

	size_t varintValue;
	bool isFieldNumber = true;

	size_t msgSize = 0;
	while (msgSize != message.ByteSizeLong()) {
		msgSize += StreamUtilities::parseVarint(buffer + msgSize, varintValue);
		if (isFieldNumber) {
			fprintf(stderr, "%zd: ", ssize_t(varintValue >> 3));
		} else {
			fprintf(stderr, "%zd\n", varintValue);
		}
		isFieldNumber = !isFieldNumber;
	}

	for (size_t i = 0; i < message.ByteSizeLong(); i++) {
		std::cout << std::hex << *(buffer + i);
		// printf("%X", *(buffer + i));
	}

	for (size_t i = 0; i < message.ByteSizeLong(); i++) {
		fprintf(stderr, "%.2X", *(buffer + i));
	}
	fprintf(stderr, "\n");

	return true;
}
}	 // namespace Test