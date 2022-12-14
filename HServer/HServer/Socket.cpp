#include "net/Socket.hpp"
#include <memory>
#include <codecvt>

net::Socket::Socket(AddressFamily af, SocketType type, ProtocolType proto)
	: m_af(af)
{
	m_sock = socket((int)af, (int)type, (int)proto);
	if (m_sock == INVALID_SOCKET) {
		throw network_error();
	}
}

net::Socket::Socket(SocketType type, ProtocolType proto)
{
	Socket(AddressFamily::InterNetwork, type, proto);
}

net::Socket::Socket(const Socket& sock)
{
	this->m_sock = sock.m_sock;
	this->m_af = sock.m_af;
}

net::Socket::Socket(Socket&& sock) noexcept
{
	this->m_sock = std::move(sock.m_sock);
	this->m_af = std::move(sock.m_af);
}

void net::Socket::operator=(const Socket& sock)
{
	this->m_sock = sock.m_sock;
	this->m_af = sock.m_af;
}

bool net::Socket::operator==(const Socket& sock) const
{
	return m_sock == sock.m_sock;
}

void net::Socket::SetBlocking(bool blocking)
{
	u_long n = blocking ? 0 : 1;
	if (ioctlsocket(m_sock, FIONBIO, &n) == SOCKET_ERROR)
	{
		throw network_error();
	}
}

net::Socket::Socket(SOCKET acpt)
	: m_af(AddressFamily::InterNetwork)
{
	this->m_sock = acpt;
}

void net::Socket::Close()
{
	closesocket(m_sock);
}

void net::Socket::Listen(uint16_t size)
{
	if (listen(m_sock, size) == SOCKET_ERROR) {
		throw network_error();
	}
}

void net::Socket::Bind(IPEndPoint& endpoint)
{
	SOCKADDR_IN* addr = &endpoint;
	addr->sin_family = (ADDRESS_FAMILY)m_af;
	if (bind(m_sock, (SOCKADDR*)addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		throw net::network_error();
	}
}

const net::Socket net::Socket::Accept()
{
	Socket t = accept(GetHandle(), nullptr, nullptr);
	if (t.m_sock == INVALID_SOCKET) {
		throw network_error();
	}
	return t;
}

void encodeToUTF8(char* str) {
	int nLen = MultiByteToWideChar(CP_UTF8, 0, str, lstrlen(str) + 1, NULL, NULL);
	BSTR bstr = SysAllocStringLen(NULL, nLen);

	nLen = MultiByteToWideChar(CP_UTF8, 0, str, lstrlen(str) + 1, bstr, nLen);

	nLen = WideCharToMultiByte(CP_ACP, 0, bstr, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, bstr, -1, str, nLen, NULL, NULL);
	SysFreeString(bstr);
}

int net::Socket::Receive(char* buffer, int size, SocketFlags flag)
{
	int len = recv(m_sock, buffer, size, (int)flag);
	encodeToUTF8(buffer);

	if (len <= 0) {
		throw network_error();
	}
	return len;
}

int net::Socket::Send(const char* buffer, int size, SocketFlags flag)
{
	int len = send(m_sock, buffer, size, (int)flag);
	if (len == SOCKET_ERROR) {
		throw network_error();
	}
	return len;
}

const SOCKET net::Socket::GetHandle() const
{
	return m_sock;
}

void net::Socket::SetHandle(SOCKET s)
{
	this->m_sock = s;
}

bool net::Socket::operator==(Socket other)
{
	return this->m_sock == other.m_sock;
}

