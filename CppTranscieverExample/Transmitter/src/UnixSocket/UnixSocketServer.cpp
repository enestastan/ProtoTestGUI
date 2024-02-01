#include "UnixSocketServer.h"

UnixSocketServer::UnixSocketServer(const std::string& socketFilePath) : m_SOCKET_FILE_PATH{socketFilePath} {};

UnixSocketServer::~UnixSocketServer() {
	close(m_clientSocket);
	close(m_serverSocket);
}

bool UnixSocketServer::initialize() {
	if ((m_serverSocket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		printf("Creating socket failed \n");
		return false;
	}

	m_serverAddress.sun_family = AF_UNIX;
	strncpy(m_serverAddress.sun_path, m_SOCKET_FILE_PATH.c_str(), m_SOCKET_FILE_PATH.size());
	unlink(m_SOCKET_FILE_PATH.c_str());
	uint32_t uAddressLength = m_SOCKET_FILE_PATH.size() + sizeof(m_serverAddress.sun_len) + sizeof(m_serverAddress.sun_family);

	if (bind(m_serverSocket, (struct sockaddr*)&m_serverAddress, uAddressLength) != 0) {
		printf("Error on binding socket \n");
		return false;
	}

	if (listen(m_serverSocket, 1) != 0) {
		printf("Error on listen call \n");
		return false;
	}

	return true;
}

bool UnixSocketServer::acceptConnection() {
	printf("Waiting for connection.... \n");

	unsigned int nSocketAddressLength = 0;
	if ((m_clientSocket = accept(m_serverSocket, (struct sockaddr*)&m_clientAddress, &nSocketAddressLength)) == -1) {
		printf("Error on accept() call \n");
		return false;
	}

	printf("Client connected.\n");
	return true;
}

bool UnixSocketServer::send(std::uint8_t* pBuffer, std::size_t uSize) {
	if (::send(m_clientSocket, pBuffer, uSize, 0) <= 0) {
		printf("Send error! errno: %d - %s\n", errno, strerror(errno));
		return false;
	}

	return true;
}

std::size_t UnixSocketServer::receive(std::uint8_t* pBuffer, std::size_t uSize) {
	ssize_t nReceivedSize;
	if ((nReceivedSize = ::recv(m_clientSocket, pBuffer, uSize, 0)) <= 0) {
		printf("Receive error! errno: %d - %s\n", errno, strerror(errno));
		return 0;
	}

	return static_cast<std::size_t>(nReceivedSize);
}
