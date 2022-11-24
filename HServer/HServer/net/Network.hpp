#pragma once

#include "IPEndPoint.hpp"
#include "Socket.hpp"
#include "network_error.hpp"
#include "SocketSelector.hpp"

namespace net {
	extern std::string GetPeerAddressString(const Socket& sock);

	extern inline void initialize();

	extern inline void finalize();
}