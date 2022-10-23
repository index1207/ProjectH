#pragma once
#include "Socket.hpp"

namespace hsv
{
	class Selector
	{
	public:
		// Clear socket set
		void Clear();
		// Remove from socket set
		void Remove(Socket& sock);
		// // Add socket at socket set
		void Add(Socket& sock);
	};
}
