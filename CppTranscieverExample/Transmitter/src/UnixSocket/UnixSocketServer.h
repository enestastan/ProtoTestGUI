#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include <string>

class UnixSocketServer {

	UnixSocketServer(const std::string& sSocketFilePath);
	~UnixSocketServer();

	bool initialize();
	bool acceptConnection();

	bool send(std::uint8_t* pBuffer, std::size_t uSize);
	std::size_t receive(std::uint8_t* pBuffer, std::size_t uSize);

	int m_serverSocket;
	int m_clientSocket;
	struct sockaddr_un m_serverAddress;
	struct sockaddr_un m_clientAddress;
	const std::string m_SOCKET_FILE_PATH;
};