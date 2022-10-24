#include "net/IPAddress.hpp"

namespace net {
	IPAddress::IPAddress(std::array<uint16_t, 4> bytes)
		: m_adr_str("") 
	{
		for (int i = 0; i < bytes.size(); ++i) {
			m_adr_str += std::to_string(bytes[i]);
			if (i < bytes.size() - 1) {
				m_adr_str += ".";
			}
		}
		bindAddr(m_adr_str.c_str());
	}

	const std::string IPAddress::ToString() const
	{
		return m_adr_str;
	}

	net::IPAddress net::IPAddress::Parse(const char* adrstr)
	{
		IPAddress ipadr;
		ipadr.m_adr_str = adrstr;
		ipadr.bindAddr(adrstr);

		return ipadr;
	}

	const IPAddress IPAddress::Any() 
	{
		net::IPAddress ipadr;
		ipadr.m_adr_str = "0.0.0.0";
		ipadr.sin_addr.s_addr = htonl(INADDR_ANY);
		return ipadr;
	}
	const IPAddress IPAddress::Loopback() 
	{
		net::IPAddress ipadr;
		ipadr.m_adr_str = "127.0.0.1";
		ipadr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		return ipadr;
	}
	const IPAddress IPAddress::None() 
	{
		net::IPAddress ipadr;
		ipadr.m_adr_str = "255.255.255.255";
		ipadr.sin_addr.s_addr = htonl(INADDR_NONE);
		return ipadr;
	}
	const IPAddress IPAddress::Broadcast() 
	{
		net::IPAddress ipadr;
		ipadr.m_adr_str = "255.255.255.255";
		ipadr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		return ipadr;
	}

	void IPAddress::bindAddr(const char* adrstr) 
	{
		inet_pton(AF_INET, adrstr, &this->sin_addr);
	}
}