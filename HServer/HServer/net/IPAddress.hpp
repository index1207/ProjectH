#pragma once
#include "ws2.hpp"

#include <string>
#include <array>

namespace net
{
	class IPAddress : private SOCKADDR_IN
	{
	public:
		IPAddress() = default;

		IPAddress(std::array<uint16_t, 4> bytes);

		const std::string ToString() const;

		void Parse(std::string adrstr);
	public:
		const static IPAddress Any();

		const static IPAddress Loopback();

		const static IPAddress None();

		const static IPAddress Broadcast();
	private:
		void bindAddr(std::string& adrstr);
	private:
		std::string m_adr_str;
		friend class IPEndPoint;
	};
}