#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

#include <string>

class UnixSocketClient {
public:
	UnixSocketClient();
	~UnixSocketClient();

	bool initialize();
	bool connect(const std::string& sSocketFilePath);

	bool send(std::uint8_t* pBuffer, std::size_t uSize);
	std::size_t receive(std::uint8_t* pBuffer, std::size_t uSize);

private:
	int m_clientSocket;
	struct sockaddr_un m_serverAddress;
	const std::string m_SOCKET_FILE_PATH;
};