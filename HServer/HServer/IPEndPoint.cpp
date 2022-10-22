#include "stdafx.h"
#include "hsv/IPEndPoint.hpp"

namespace hsv {
	IPEndPoint::IPEndPoint(IPAddress&& address, unsigned short port)
		: m_port(port), m_adr(std::move(address))
	{
		SOCKADDR_IN* addrin = &m_adr;
		addrin->sin_port = htons(port);
	}
	IPEndPoint::IPEndPoint(const IPAddress& address, unsigned short port)
		: m_port(port), m_adr(address)
	{
		SOCKADDR_IN* addrin = &m_adr;
		addrin->sin_port = htons(port);
	}

	const std::string IPEndPoint::ToString() const
	{
		return m_adr.ToString() + ":" + std::to_string(m_port);
	}
	SOCKADDR_IN* IPEndPoint::operator&()
	{
		return &m_adr;
	}
}