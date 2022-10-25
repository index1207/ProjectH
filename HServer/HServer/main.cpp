#include <iostream>

#include "net/Network.hpp"

#include <json/json.h>
#include <mariadb/conncpp.hpp>

using namespace net;

static sql::Driver* driver;
std::unique_ptr<sql::Connection> conn;

void initMariaDB() {
	driver = sql::mariadb::get_driver_instance();

	sql::SQLString url("jdbc:mariadb://localhost:3306/chatdb");
	sql::Properties properties({ {"user", "root"}, {"password", "1207"} });

	std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));
}

int main() {
	initMariaDB();
	net::initialize();
	net::Socket listenSock(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::Tcp);

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
					int len = sock.Receive(buf, 1024);

					reader.parse(buf, root);
					if (root["type"].asString() == "request") {
						std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("INSERT INTO usrinfo VALUES(?, ?)"));
						stmnt->setString(1, sql::SQLString(root["id"].asString()));
						stmnt->setString(2, sql::SQLString(root["pwd"].asString()));

						stmnt->executeQuery();
					}
					else if(root["type"].asString() == "message") {
						auto usr = root["id"].asString();
						auto msg = root["message"].asString();
						std::cout << std::format("[{}] {}\n", usr, msg);

						Json::Value json;
						json["type"] = "message";
						json["id"] = usr;

						Json::FastWriter writer;
						std::string jsonstr = writer.write(json);
						for (int j = 0; j < slt.Count(); ++j) {
							auto out = slt[j];
							if (out != listenSock && out != sock) {
								out.Send(jsonstr.c_str(), jsonstr.length());
							}
						}
					}
				}
			}
			catch (std::exception&) {
				std::cout << "[System] Disconnected " << GetPeerAddressString(sock) << "\n";
				slt.Remove(sock);
				sock.Close();
			}
		}
	}
	listenSock.Close();
	net::finalize();
}