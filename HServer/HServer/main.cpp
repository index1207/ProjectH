#include <iostream>

#include "net/Network.hpp"

using namespace net;

int main() {
	net::initialize();
	
	net::Socket listenSock(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);
	listenSock.setBlocking(true);

	IPEndPoint ep(IPAddress::Any(), 1207);
	listenSock.Bind(ep);
	listenSock.Listen(16);

	try {
		Socket clientSock = listenSock.Accept();
		clientSock.Send("hi", 3);
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
	}

	net::finalize();
}