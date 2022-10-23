#include <iostream>

#include "hsv/Network.hpp"

using namespace hsv;

int main() {
	Socket sock;
	try {
		hsv::initialize();
		sock = Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

		IPEndPoint ep(IPAddress::Any(), 1207);
		sock.Bind(ep);

		sock.Listen(16);

		Socket client = sock.Accept();

		sock.setBlocking(false);

		char buf[1024] = "";
		while (true) {
			memset(buf, 0, 1024);
			try {
				auto n = client.Receive(buf, 1024);
				std::cout << "받은 바이트 : " << n << '\n';

				n = client.Send(buf, strlen(buf));
				std::cout << "보낸 바이트 : " << n << '\n';
			}
			catch (std::exception& exc) {
				std::cout << "연결이 끊어짐";
				break;
			}
		}
	}
	catch (std::exception& e) {
		std::cout << e.what() << '\n';
	}
	sock.Close();
	hsv::finalize();
}