#include "hsv/IPAddress.hpp"

namespace hsv {
	IPAddress::IPAddress(std::array<uint16_t, 4> bytes)
		: m_adr_str("") 
	{
		for (int i = 0; i < bytes.size(); ++i) {
			m_adr_str += std::to_string(bytes[i]);
			if (i < bytes.size() - 1) {
				m_adr_str += ".";
			}
		}
		bindAddr(m_adr_str);
	}

	const std::string IPAddress::ToString() const
	{
		return m_adr_str;
	}

	void hsv::IPAddress::Parse(std::string adrstr) 
	{
		bindAddr(adrstr);
	}

	const IPAddress IPAddress::Any() 
	{
		hsv::IPAddress ipadr;
		ipadr.m_adr_str = "0.0.0.0";
		ipadr.sin_addr.s_addr = htonl(INADDR_ANY);
		return ipadr;
	}
	const IPAddress IPAddress::Loopback() 
	{
		hsv::IPAddress ipadr;
		ipadr.m_adr_str = "127.0.0.1";
		ipadr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		return ipadr;
	}
	const IPAddress IPAddress::None() 
	{
		hsv::IPAddress ipadr;
		ipadr.m_adr_str = "255.255.255.255";
		ipadr.sin_addr.s_addr = htonl(INADDR_NONE);
		return ipadr;
	}
	const IPAddress IPAddress::Broadcast() 
	{
		hsv::IPAddress ipadr;
		ipadr.m_adr_str = "255.255.255.255";
		ipadr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		return ipadr;
	}

	void IPAddress::bindAddr(std::string& adrstr) 
	{
		inet_pton(AF_INET, adrstr.c_str(), &this->sin_addr);
	}
}