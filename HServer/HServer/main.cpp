#include <iostream>
#include <unordered_map>

#include "net/Network.hpp"

#include <json/json.h>
#include <mariadb/conncpp.hpp>

using namespace net;

static std::unordered_map<std::string, std::string> umap;
static std::map<Socket, int> room;

void loginUser(const Json::Value& root, Socket& sock, std::unique_ptr<sql::Connection>& conn) {
	auto userid = root["id"].asString();
	auto password = root["pwd"].asString();
	try {
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("SELECT usrpwd FROM usracc WHERE usrid=?"));
		stmnt->setString(1, userid);

		auto res = stmnt->executeQuery();
		if (res->next()) {
			if (res->getString("usrpwd") == password) {
				auto succmsg = "로그인됨.";
				sock.Send(succmsg, strlen(succmsg));
				std::cout << "[System] Joined \'" << userid << "\'\n";

				umap.insert({ net::GetPeerAddressString(sock), userid });
			}
			else {
				auto failmsg = "잘못된 비밀번호 입니다.";
				sock.Send(failmsg, strlen(failmsg));
			}
		}
		else {
			throw std::exception();
		}
	}
	catch (std::exception&) {
		auto failmsg = "아이디가 존재하지 않습니다.";
		sock.Send(failmsg, strlen(failmsg));
	}
}

void registerUser(const Json::Value& root, net::Socket& sock, std::unique_ptr<sql::Connection>& conn) {
	auto userid = root["id"].asString();
	auto password = root["pwd"].asString();
	try {
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("INSERT INTO usracc VALUES(?, ?)"));
		stmnt->setString(1, userid);
		stmnt->setString(2, password);

		stmnt->executeQuery();

		std::cout << "[System] Joined \'" << userid << "\'\n";
		auto str = "새 아이디가 등록됨.";
		sock.Send(str, strlen(str));

		umap.insert({ net::GetPeerAddressString(sock), userid });
	}
	catch (std::exception& ex) {
		if (std::string(ex.what()).find("PRIMARY") != std::string::npos) {
			auto errmsg = "이미 존재하는 아이디 입니다.";
			sock.Send(errmsg, strlen(errmsg));
		}
	}
}

void sendMessage(const Json::Value& root, Socket& listenSock, SocketSelector& slt, Socket& sock) {
	auto usr = root["id"].asString();
	auto msg = root["message"].asString();
	std::cout << std::format("[{}] {}\n", usr, msg);

	Json::Value json;
	json["type"] = "message";
	json["id"] = usr;

	Json::FastWriter writer;
	std::string jsonstr = writer.write(json);
	for (int i = 0; i < slt.Count(); ++i) {
		auto out = slt[i];
		for (int j = 0; j < slt.Count(); ++j) {
			room[]
		}
		if (out != listenSock && out != sock) {
			out.Send(jsonstr.c_str(), jsonstr.length());
		}
	}
}

int main() {
	sql::Driver* driver = sql::mariadb::get_driver_instance();

	sql::SQLString url("jdbc:mariadb://localhost:3306/chatdb");
	sql::Properties properties({ {"user", "root"}, {"password", "1207"} });

	std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));

	net::initialize();
	Socket listenSock(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

	IPEndPoint ep(IPAddress::Any(), 1207);
	listenSock.Bind(ep);

	listenSock.SetBlocking(false);

	listenSock.Listen(16);
	std::cout << "[System] Server is Ready!\n";

	SocketSelector slt;
	slt.Add(listenSock);
	while (true) {
		int nSocket = slt.Wait(std::chrono::milliseconds(100));
		for (int i = 0; i < nSocket; ++i) {
			Socket sock = slt.GetReadySocket(i);

			Json::Reader reader;
			Json::Value root;
			char buf[1024] = "";
			try {
				if (sock == listenSock) {
					auto client = listenSock.Accept();
					slt.Add(client);

					std::cout << "[System] Connected " << GetPeerAddressString(client) << '\n';
				}
				else {
					sock.Receive(buf, 1024);

					reader.parse(buf, root);
					if (root["type"].asString() == "login") {
						loginUser(root, sock, conn);
					}
					else if (root["type"].asString() == "register") {
						registerUser(root, sock, conn);
					}
					/*else if (root["type"].asString() == "enter") {
						room.insert({sock, root["room"].asInt()});
					}*/
					else if (root["type"].asString() == "message") {
						sendMessage(root, listenSock, slt, sock);
					}
				}
			}
			catch (std::exception&) {
				std::cout << "[System] Disconnected " << net::GetPeerAddressString(sock) << "\n";
				slt.Remove(sock);
				sock.Close();
			}
		}
	}
	listenSock.Close();
	net::finalize();
}