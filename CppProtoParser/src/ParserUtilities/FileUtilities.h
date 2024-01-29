#pragma once
#include <google/protobuf/compiler/importer.h>
#include <stdint.h>

#include <algorithm>
#include <map>

class FileUtilities {
 public:
	static void printProtoFile(google::protobuf::FileDescriptorProto& protoDescriptor);
};

void FileUtilities::printProtoFile(google::protobuf::FileDescriptorProto& protoDescriptor) {
	std::cerr << "syntax = " << protoDescriptor.syntax().c_str() << std::endl;
	std::cerr << std::endl;

	std::cerr << "package = " << protoDescriptor.package().c_str() << std::endl;
	std::cerr << std::endl;

	for (int i = 0; i < protoDescriptor.enum_type_size(); i++) {
		const google::protobuf::EnumDescriptorProto& enumm = protoDescriptor.enum_type(i);
		std::cerr << enumm.name().c_str() << " {" << std::endl;

		for (int j = 0; j < enumm.value_size(); j++) {
			std::cerr << "\t" << enumm.value(j).name().c_str() << " = " << enumm.value(j).number() << ";" << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}

	for (int i = 0; i < protoDescriptor.message_type_size(); i++) {
		const google::protobuf::DescriptorProto& msg = protoDescriptor.message_type(i);
		std::cerr << msg.name().c_str() << " {" << std::endl;

		for (int j = 0; j < msg.field_size(); j++) {
			std::cerr << "\t" << msg.field(j).type_name() << " " << msg.field(j).name() << " = " << msg.field(j).number() << ";" << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}
}