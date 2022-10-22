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

		char buf[1024] = "";
		while (true) {
			try {
				auto n = client.Receive(buf, 1024);
				std::cout << "���� ����Ʈ : " << n << '\n';

				n = client.Send(buf, strlen(buf));
				std::cout << "���� ����Ʈ : " << n << '\n';
			}
			catch (std::exception& e) {
				std::cout << "������ ������";
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