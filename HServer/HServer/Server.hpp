#pragma once
#include <json/json.h>
#include <mariadb/conncpp.hpp>
#include "net/Network.hpp"
#include <unordered_map>

class Server {
public:
	Server(u_short port, bool isLogging);

	~Server();

	void init();

	void start();

	void loop();
private:
	struct Log {
		static const int Warning = 0;
		static const int Info = 1;
		static const int Error = 2;
		static const int Chat = 3;
		struct Cmd {
			static const int Log = 4;
			static const int Command = 5;
		};
	};
	void ServerLog(int flag, std::string msg);
private:
	void loginUser(const Json::Value& root, net::Socket& sock);

	void registerUser(const Json::Value& root, net::Socket& sock);

	void Message(const Json::Value& root, net::Socket& sock);
private:
	std::string to_datetime(const tm& tm);
private:
	bool isLogText;

	time_t timer{};
	tm curtime{};

	std::string t_hour;
	std::string t_min;
	std::string t_sec;

	int logcount = 0;
private:
	std::unordered_map<std::string, std::string> umap;
private:
	sql::Driver* driver;
	std::unique_ptr<sql::Connection> conn;
private:
	net::IPAddress adr;
	net::IPEndPoint ep;

	net::Socket listenSock;
	net::SocketSelector slt;
private:
	std::atomic<bool> isEnter;
};
