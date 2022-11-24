#include "net/Network.hpp"

extern std::string net::GetPeerAddressString(const net::Socket& sock)
{
	SOCKADDR_IN addr;
	int addrlen = sizeof(SOCKADDR_IN);
	getpeername(sock.GetHandle(), (SOCKADDR*)&addr, &addrlen);
	char ipstr[16] = "";
	inet_ntop(AF_INET, &addr.sin_addr, ipstr, 16);

	return IPEndPoint(IPAddress::Parse(ipstr), ntohs(addr.sin_port)).ToString();
}

void net::initialize()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		throw network_error();
	}
}

void net::finalize()
{
	WSACleanup();
}