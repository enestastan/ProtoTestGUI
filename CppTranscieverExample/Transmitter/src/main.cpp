#include "UnixSocket/UnixSocketServer.h"

#include <stdio.h>
#include "addressbook.pb.h"
#include <google/protobuf/util/time_util.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

using google::protobuf::util::TimeUtil;

static const char* SOCKET_PATH = "/tmp/mysocket";

int main(int argc, char* argv[]) {

	printf("Hello Transmitter!\n");

  UnixSocketServer socketConnection(SOCKET_PATH);

  if(!socketConnection.initialize()) {
    printf("Socket init failed!\n");
    return -1;
  }

  socketConnection.acceptConnection();

	tutorial::AddressBook address_book;

	{
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
	}

  uint8_t uBuffer[address_book.ByteSizeLong()];
  address_book.SerializeToArray(uBuffer, address_book.ByteSizeLong());
  socketConnection.send((uint8_t*)uBuffer, sizeof(uBuffer));
	return 0;
}