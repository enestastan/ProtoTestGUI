#include "UnixSocket/UnixSocketClient.h"

#include "addressbook.pb.h"
#include <google/protobuf/util/time_util.h>
#include <stdio.h>

using namespace std;

using google::protobuf::util::TimeUtil;

static const std::size_t MAX_BUFFER_LENGTH = 1000;
static const char* SOCKET_PATH = "/tmp/mysocket";

// Iterates though all people in the AddressBook and prints info about them.
void ListPeople(const tutorial::AddressBook& address_book) {
  for (int i = 0; i < address_book.people_size(); i++) {
    const tutorial::Person& person = address_book.people(i);

    cout << "Person ID: " << person.id() << endl;
    cout << "  Name: " << person.name() << endl;
    if (person.email() != "") {
      cout << "  E-mail address: " << person.email() << endl;
    }

    for (int j = 0; j < person.phones_size(); j++) {
      const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

      switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
          cout << "  Mobile phone #: ";
          break;
        case tutorial::Person::HOME:
          cout << "  Home phone #: ";
          break;
        case tutorial::Person::WORK:
          cout << "  Work phone #: ";
          break;
        default:
          cout << "  Unknown phone #: ";
          break;
      }
      cout << phone_number.number() << endl;
    }
    if (person.has_last_updated()) {
      cout << "  Updated: " << TimeUtil::ToString(person.last_updated()) << endl;
    }
  }
}

int main() {
	UnixSocketClient socketConnection;

	if(!socketConnection.initialize()) {
		printf("Client socket init failed!\n");
		return -1;
	}


	if(!socketConnection.connect(SOCKET_PATH)) {
		printf("Client socket connect failed!\n");
		return -1;
	}

	tutorial::AddressBook address_book;

	uint8_t uReceiveBuffer[MAX_BUFFER_LENGTH];


	size_t uReceivedSize;
	if((uReceivedSize = socketConnection.receive(uReceiveBuffer, MAX_BUFFER_LENGTH)) == 0) {
		printf("Receive failed!\n");
		return -1;
	}

	address_book.ParseFromArray(uReceiveBuffer, uReceivedSize);

	ListPeople(address_book);
	return 0;
}