#pragma once
#include "Socket.hpp"
#include <chrono>

namespace net
{
	class SocketSelector
	{
	public:
		SocketSelector();
	public:
		// Clear socket set
		void Clear();
		// Is exist at socket set
		bool isReady(const Socket& sock);
		// Remove from socket set
		void Remove(Socket& sock);
		// Add socket at socket set
		void Add(Socket& sock);
		// Get ready number of sockets
		int Wait(std::chrono::milliseconds time);
		// Get ready socket
		Socket GetReadySocket(int num);
		// Get ready socket count
		int Count() const;
	public:
		Socket operator[](size_t i);
	private:
		int m_sockCount;
		FD_SET master;
		FD_SET copy;
	};
}
