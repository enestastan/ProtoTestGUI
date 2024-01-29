#include <fcntl.h>
#include <google/protobuf/compiler/importer.h>
#include <stdio.h>

#include "ParserUtilities/FileUtilities.h"
#include "TypeParsingTests/TestVarintTypes.h"

int main() {
	google::protobuf::compiler::DiskSourceTree sourceTree;
	sourceTree.MapPath("", "../../../../Protocol/Proto");
	google::protobuf::compiler::SourceTreeDescriptorDatabase sourceTreeDatabase(&sourceTree);
	google::protobuf::FileDescriptorProto testMessagesFd;
	sourceTreeDatabase.FindFileByName("TestMessages.proto", &testMessagesFd);

	FileUtilities::printProtoFile(testMessagesFd);

	Test::TestVarintTypes();

	return 0;
}
