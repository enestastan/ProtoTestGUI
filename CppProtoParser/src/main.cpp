#include <fcntl.h>
#include <google/protobuf/compiler/parser.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <stdio.h>

#include "TypeParsingTests/TestVarintTypes.h"

int main() {
	// relative to binary output location
	int fd = open("../../../../Protocol/Proto/TestMessages.proto", O_RDONLY);
	google::protobuf::io::FileInputStream inputStream(fd);
	google::protobuf::io::Tokenizer tokenizer(&inputStream, nullptr);
	google::protobuf::FileDescriptorProto testMessagesFd;
	google::protobuf::compiler::Parser parser;

	if (!parser.Parse(&tokenizer, &testMessagesFd)) {
		std::cerr << "Parsing failed!" << std::endl;
		return 1;
	}

	for (int i = 0; i < testMessagesFd.enum_type_size(); i++) {
		const google::protobuf::EnumDescriptorProto& enumm = testMessagesFd.enum_type(i);
		std::cerr << enumm.name().c_str() << " {" << std::endl;

		for (int j = 0; j < enumm.value_size(); j++) {
			std::cerr << "\t" << enumm.value(j).name().c_str() << " = " << enumm.value(j).number() << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}

	for (int i = 0; i < testMessagesFd.message_type_size(); i++) {
		const google::protobuf::DescriptorProto& msg = testMessagesFd.message_type(i);
		std::cerr << msg.name().c_str() << " {" << std::endl;

		for (int j = 0; j < msg.field_size(); j++) {
			std::cerr << "\t" << msg.field(j).type_name() << " " << msg.field(j).name() << " : " << msg.field(j).number() << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}

	Test::TestVarintTypes();

	return 0;
}
