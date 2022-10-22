#pragma once

#include "../stdafx.h"

namespace hsv
{
	class IPAddress : public SOCKADDR_IN
	{
	public:
		IPAddress() = default;

		IPAddress(std::vector<uint16_t> bytes);

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
	};
}