#pragma once
#include "../stdafx.h"
#include "network_error.hpp"
#include "IPEndPoint.hpp"

namespace hsv {
	enum class AddressFamily
	{
		InterNetwork = AF_INET,
		InterNetworkV6 = AF_INET6
	};
	enum class SocketType
	{
		Dgram = SOCK_DGRAM,
		Stream = SOCK_STREAM
	};

	enum class ProtocolType
	{
		Raw = IPPROTO_RAW,
		IP = IPPROTO_IP,
		IPv4 = IPPROTO_IPV4,
		IPv6 = IPPROTO_IPV6,
		Tcp = IPPROTO_TCP,
		Udp = IPPROTO_UDP
	};
	enum class SocketFlags
	{
		None = NULL,
		OutOfBand = MSG_OOB,
		DontRoute = MSG_DONTROUTE,
		Peek = MSG_PEEK,
		WaitAll = MSG_WAITALL
	};
	class Socket
	{
	public:
		Socket() = default;

		Socket(AddressFamily af, SocketType type, ProtocolType proto);

		Socket(SocketType type, ProtocolType proto);

		Socket(const Socket& sock);

		Socket(Socket&& sock) noexcept;

		void operator=(const Socket& sock);
	public:
		void Close();

		void Listen(uint16_t size);

		void Bind(IPEndPoint& endpoint);

		const Socket Accept();
	public:
		size_t Receive(char* buffer, int size, SocketFlags flag = SocketFlags::None);

		size_t Send(const char* buffer, int size, SocketFlags flag = SocketFlags::None);
	public:
		const SOCKET GetHandle() const;
	private:
		Socket(SOCKET acpt);
	private:
		AddressFamily m_af;
		SOCKET m_sock;
	};
}


