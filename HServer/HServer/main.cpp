#include "Server.hpp"

int main() {
	Server server(1207, false);
	server.init();

	server.start();
	while (true) {
		server.loop();
	}

	return 0;
}