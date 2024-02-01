#include "UnixSocketClient.h"

#include "errno.h"
#include "stdio.h"

UnixSocketClient::UnixSocketClient(){};

UnixSocketClient::~UnixSocketClient() {
	close(m_clientSocket);
}

bool UnixSocketClient::initialize() {
	if ((m_clientSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		printf("Creating socket failed \n");
		return false;
	}

	return true;
}

bool UnixSocketClient::connect(const std::string& sSocketFilePath) {
	m_serverAddress.sun_family = AF_UNIX;
	strncpy(m_serverAddress.sun_path, sSocketFilePath.c_str(), sSocketFilePath.size());
	uint32_t uAddressLength = sSocketFilePath.size() + sizeof(m_serverAddress.sun_len) + sizeof(m_serverAddress.sun_family);

	if (::connect(m_clientSocket, (struct sockaddr*)&m_serverAddress, uAddressLength) == -1) {
		printf("Error on connect() call \n");
		return false;
	}

	printf("Connected to server!\n");
	return true;
}

bool UnixSocketClient::send(std::uint8_t* pBuffer, std::size_t uSize) {
	if (::send(m_clientSocket, pBuffer, uSize, 0) <= 0) {
		printf("Send error! errno: %d - %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

std::size_t UnixSocketClient::receive(std::uint8_t* pBuffer, std::size_t uSize) {
	ssize_t nReceivedSize;
	if ((nReceivedSize = ::recv(m_clientSocket, pBuffer, uSize, 0)) <= 0) {
		printf("Receive error! errno: %d - %s\n", errno, strerror(errno));
		return 0;
	}

	return static_cast<std::size_t>(nReceivedSize);
}
