#pragma once
#include "IPAddress.hpp"

namespace net
{
	class IPEndPoint
	{
	public:
		IPEndPoint() = delete;

		IPEndPoint(IPAddress&& address, unsigned short port);

		IPEndPoint(const IPAddress& address, unsigned short port);
	public:
		const std::string ToString() const;
	public:
		SOCKADDR_IN* operator&();
	private:
		unsigned short m_port;
		IPAddress m_adr;
	};
}

