#pragma once

#include "IPEndPoint.hpp"
#include "Socket.hpp"
#include "network_error.hpp"

namespace hsv {
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