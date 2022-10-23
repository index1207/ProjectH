#include <iostream>

#include "hsv/Network.hpp"

using namespace hsv;

int main() 
{
	Socket sock;
	try 
	{
		hsv::initialize();
		sock = Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

		int optval = 1;
		setsockopt(sock.GetHandle(), SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&optval), sizeof(optval));

		IPEndPoint ep(IPAddress::Any(), 1207);
		sock.Bind(ep);

		sock.Listen(16);
		std::cout << "[System] Server is Ready!\n";
		sock.setBlocking(false);

		Selector selector;
		selector.Add(sock);

		char buf[1024] = "";
		while (true) 
		{
			int nSocket = selector.Wait(std::chrono::milliseconds(100));
			for (int i = 0; i < nSocket; ++i) 
			{
				memset(buf, 0, 1024);
				Socket s = selector.GetReadySocket(i);
				if (sock == s)
				{
					Socket client = sock.Accept();
					selector.Add(client);

					std::cout << "[System] Connected " << client.GetHandle() << '\n';
				}
				else
				{
					try
					{
						int len = s.Receive(buf, 1024);
						std::cout << std::format("[{}] {}\n", s.GetHandle(), buf) << '\n';
						s.Send(buf, len);
					}
					catch (std::exception& ex)
					{
						selector.Remove(s);
						std::cout << "[System] Disconnected " << s.GetHandle() << '\n';
						s.Close();
					}
				}
			}
		}
	}
	catch (std::exception& e) 
	{
		std::cout << e.what() << '\n';
	}
	sock.Close();
	hsv::finalize();
}