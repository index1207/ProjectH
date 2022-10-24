#include "net/IPEndPoint.hpp"

namespace net {
	IPEndPoint::IPEndPoint(IPAddress&& address, unsigned short port)
		: m_port(port), m_adr(std::move(address))
	{
		auto* addrin = dynamic_cast<SOCKADDR_IN*>(&m_adr);
		addrin->sin_port = htons(port);
	}
	IPEndPoint::IPEndPoint(const IPAddress& address, unsigned short port)
		: m_port(port), m_adr(address)
	{
		auto* addrin = dynamic_cast<SOCKADDR_IN*>(&m_adr);
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