#include "hsv/Socket.hpp"

hsv::Socket::Socket(AddressFamily af, SocketType type, ProtocolType proto)
	: m_af(af)
{
	m_sock = socket((int)af, (int)type, (int)proto);
	if (m_sock == INVALID_SOCKET) {
		throw network_error();
	}
}

hsv::Socket::Socket(SocketType type, ProtocolType proto)
{
	Socket(AddressFamily::InterNetwork, type, proto);
}

hsv::Socket::Socket(const Socket& sock)
{
	this->m_sock = sock.m_sock;
	this->m_af = sock.m_af;
}

hsv::Socket::Socket(Socket&& sock) noexcept
{
	this->m_sock = std::move(sock.m_sock);
	this->m_af = std::move(sock.m_af);
}

void hsv::Socket::operator=(const Socket& sock)
{
	this->m_sock = sock.m_sock;
	this->m_af = sock.m_af;
}

void hsv::Socket::setBlocking(bool blocking)
{
	u_long n = blocking ? 0 : 1;
	if (ioctlsocket(m_sock, FIONBIO, &n) == SOCKET_ERROR)
	{
		throw network_error();
	}
}

hsv::Socket::Socket(SOCKET acpt)
	: m_af(AddressFamily::InterNetwork)
{
	this->m_sock = acpt;
}

void hsv::Socket::Close()
{
	closesocket(m_sock);
}

void hsv::Socket::Listen(uint16_t size)
{
	if (listen(m_sock, size) == SOCKET_ERROR) {
		throw network_error();
	}
}

void hsv::Socket::Bind(IPEndPoint& endpoint)
{
	SOCKADDR_IN* addr = &endpoint;
	addr->sin_family = (ADDRESS_FAMILY)m_af;
	if (bind(m_sock, (SOCKADDR*)addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		throw hsv::network_error();
	}
}

const hsv::Socket hsv::Socket::Accept()
{
	Socket t = accept(GetHandle(), nullptr, nullptr);
	if (t.m_sock == INVALID_SOCKET) {
		throw network_error();
	}
	return t;
}

size_t hsv::Socket::Receive(char* buffer, int size, SocketFlags flag)
{
	size_t len = recv(m_sock, buffer, size, (int)flag);
	if (len == SOCKET_ERROR) {
		throw network_error();
	}
	return len;
}

size_t hsv::Socket::Send(const char* buffer, int size, SocketFlags flag)
{
	size_t len = send(m_sock, buffer, size, (int)flag);
	if (len == SOCKET_ERROR) {
		throw network_error();
	}
	return len;
}

const SOCKET hsv::Socket::GetHandle() const
{
	return m_sock;
}

