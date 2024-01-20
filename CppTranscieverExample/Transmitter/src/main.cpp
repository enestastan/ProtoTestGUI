#include "UnixSocket/UnixSocketServer.h"

#include <stdio.h>
#include "addressbook.pb.h"
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>

using namespace std;

using google::protobuf::util::TimeUtil;

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


int main(int argc, char* argv[]) {

	printf("Hello Transmitter!\n");

	tutorial::AddressBook address_book;

	{
	// Read the existing address book.
	fstream input(argv[1], ios::in | ios::binary);
	if (!address_book.ParseFromIstream(&input)) {
		cerr << "Failed to parse address book." << endl;
		return -1;
	}
	}

	ListPeople(address_book);

	return 0;
}