#pragma once
#include <exception>

namespace hsv
{
	class network_error : public std::exception
	{
	public:
		network_error();

		virtual const char* what() const override;
	private:
		void GetErrorMessage();

		char buf[512];
		int code;
	};
}
