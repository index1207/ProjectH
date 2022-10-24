#pragma once

#include "IPEndPoint.hpp"
#include "Socket.hpp"
#include "network_error.hpp"
#include "Selector.hpp"

namespace net {
	std::string GetPeerAddressString(const Socket& sock) {
		SOCKADDR_IN addr;
		int addrlen = sizeof(SOCKADDR_IN);
		getpeername(sock.GetHandle(), (SOCKADDR*)&addr, &addrlen);
		char ipstr[16] = "";
		inet_ntop(AF_INET, &addr.sin_addr, ipstr, 16);

		return IPEndPoint(IPAddress::Parse(ipstr), ntohs(addr.sin_port)).ToString();
	}
	inline void initialize() {
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
			throw network_error();
		}
	}
	inline void finalize() {
		WSACleanup();
	}
}