#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/util/time_util.h>
#include <stdio.h>

#include "UnixSocket/UnixSocketClient.h"
#include "descriptor.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

static const std::size_t MAX_BUFFER_LENGTH = 10000;
static const char* SOCKET_PATH = "/tmp/mysocket";

void printProtoFile(const custom::FileDescriptorCustom& protoDescriptor) {
	std::cerr << "syntax = " << protoDescriptor.syntax().c_str() << std::endl;
	std::cerr << std::endl;

	std::cerr << "package = " << protoDescriptor.package().c_str() << std::endl;
	std::cerr << std::endl;

	for (int i = 0; i < protoDescriptor.enum_type_size(); i++) {
		const custom::EnumDescriptorProto& enumm = protoDescriptor.enum_type(i);
		std::cerr << enumm.name().c_str() << " {" << std::endl;

		for (int j = 0; j < enumm.value_size(); j++) {
			std::cerr << "\t" << enumm.value(j).name().c_str() << " = " << enumm.value(j).number() << ";" << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}

	for (int i = 0; i < protoDescriptor.message_type_size(); i++) {
		const custom::DescriptorProto& msg = protoDescriptor.message_type(i);
		std::cerr << msg.name().c_str() << " {" << std::endl;

		for (int j = 0; j < msg.field_size(); j++) {
			std::cerr << "\t" << msg.field(j).type_name() << " " << msg.field(j).name() << " = " << msg.field(j).number() << ";" << std::endl;
		}

		std::cerr << "}" << std::endl;
		std::cerr << std::endl;
	}
}

int main() {
	UnixSocketClient socketConnection;

	if (!socketConnection.initialize()) {
		printf("Client socket init failed!\n");
		return -1;
	}

	if (!socketConnection.connect(SOCKET_PATH)) {
		printf("Client socket connect failed!\n");
		return -1;
	}

	custom::FileDescriptorCustom incomingMsgInfo;

	uint8_t uReceiveBuffer[MAX_BUFFER_LENGTH];

	size_t uReceivedSize;
	if ((uReceivedSize = socketConnection.receive(uReceiveBuffer, MAX_BUFFER_LENGTH)) == 0) {
		printf("Receive failed!\n");
		return -1;
	}

	incomingMsgInfo.ParseFromArray(uReceiveBuffer, uReceivedSize);

	printProtoFile(incomingMsgInfo);
	return 0;
}