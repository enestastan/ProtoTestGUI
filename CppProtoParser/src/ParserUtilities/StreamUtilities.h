#pragma once
#include <stdint.h>

#include <algorithm>
#include <map>

class StreamUtilities {
 public:
	enum class EncodingTypes : u_int8_t {
		VARINT = 0,
		I64 = 1,
		LEN = 2,
		I32 = 5,
		INVALID = 10
	};

	static uint8_t parseVarint(uint8_t* buffer, size_t& result);
	static size_t getFieldNumber(const size_t fieldVarint);
	static EncodingTypes getFieldEncodeType(const size_t fieldVarint);

 private:
	static const uint8_t ENCODING_TYPE_BIT_LENGTH = 3;
};

uint8_t StreamUtilities::parseVarint(uint8_t* buffer, size_t& result) {
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

size_t StreamUtilities::getFieldNumber(const size_t fieldVarint) {
	return fieldVarint >> ENCODING_TYPE_BIT_LENGTH;
}

StreamUtilities::EncodingTypes StreamUtilities::getFieldEncodeType(const size_t fieldVarint) {
	static const std::map<uint8_t, EncodingTypes> encodingTypeMap{
			std::make_pair(0, EncodingTypes::VARINT),
			std::make_pair(1, EncodingTypes::I64),
			std::make_pair(2, EncodingTypes::LEN),
			std::make_pair(5, EncodingTypes::I32),
	};

	uint8_t typeAsInt = fieldVarint & ENCODING_TYPE_BIT_LENGTH;
	if (std::any_of(encodingTypeMap.cbegin(), encodingTypeMap.cend(), [&](const auto& pair) { return pair.first == typeAsInt; })) {
		return encodingTypeMap.at(typeAsInt);
	} else {
		return StreamUtilities::EncodingTypes::INVALID;
	}
}