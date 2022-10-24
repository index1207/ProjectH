#include <iostream>
#include <locale>

#include "net/Network.hpp"

using namespace net;

int main() {
	net::initialize();
	
	net::Socket listenSock(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

	IPEndPoint ep(IPAddress::Any(), 1207);
	listenSock.Bind(ep);
	listenSock.Listen(16);

	Selector slt;
	slt.Add(listenSock);
	while (true) {
		int nSocket = slt.Wait(std::chrono::milliseconds(350));
		for (int i = 0; i < nSocket; ++i) {
			Socket sock = slt.GetReadySocket(i);
			char buf[1024] = "";
			try {
				if (sock == listenSock) {
					auto client = listenSock.Accept();
					slt.Add(client);
					
					std::cout << "[System] Connected " << GetPeerAddressString(client) << '\n';
				}
				else {
					int len = sock.Receive(buf, 1024);
					
					std::cout << std::format("[{}] {}\n", GetPeerAddressString(sock), buf);
					for (int j = 0; j < slt.Count(); ++j) {
						auto out = slt[j];
						if (out != listenSock && out != sock) {
							out.Send(buf, len);
						}
					}
				}
			}
			catch (std::exception& ex) {
				std::cout << "[System] Disconnected " << GetPeerAddressString(sock) << "\n";
				slt.Remove(sock);
				sock.Close();
			}
		}
	}

	net::finalize();
}