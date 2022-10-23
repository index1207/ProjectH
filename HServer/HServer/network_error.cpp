#include "hsv/ws2.hpp"
#include "hsv/network_error.hpp"


namespace hsv {
	network_error::network_error() : buf("") 
	{
		GetErrorMessage();
	}

	const char* network_error::what() const {
		return buf ? buf : "Invaild exception";
	}

	void network_error::GetErrorMessage()
	{
		const char* s = "";
		code = WSAGetLastError();
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, code,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&s, 0, NULL);
		strcpy_s(buf, s);
		LocalFree((HLOCAL)s);
	}
}

