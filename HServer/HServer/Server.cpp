#include "Server.hpp"

#include <thread>

#include <color.hpp>

Server::Server(u_short port, bool isLogging)
	: ep(net::IPAddress::Any(), port), isLogText(isLogging), driver(nullptr),
	listenSock(net::AddressFamily::InterNetwork, net::SocketType::Stream, net::ProtocolType::Tcp)
{
	isEnter = true;
	net::initialize();
}

Server::~Server()
{
	listenSock.Close();
	net::finalize();
}

void Server::init()
{
	ServerLog(Log::Info, "Loading...");

	driver = sql::mariadb::get_driver_instance();

	sql::SQLString url("jdbc:mariadb://localhost:3306/chatdb");
	sql::Properties properties({ 
		{"user", "root"}, 
		{"password", "1207"} 
	});

	std::unique_ptr<sql::Connection> _conn(driver->connect(url, properties));
	conn = std::move(_conn);

	if (!isLogText) {
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("TRUNCATE TABLE serverlog"));
		stmnt->executeQuery();
	}

	listenSock.Bind(ep);
	listenSock.SetBlocking(false);
}

void Server::start()
{
	listenSock.Listen(16);
	slt.Add(listenSock);

	ServerLog(Log::Info, "Server is ready!");

	std::thread{ [this]() {
		while (true) {
			system("pause > nul");

			ServerLog(Log::Cmd::Command, "");
			std::string command;

			isEnter = false;
			std::cin >> command;
			isEnter = true;

			if (command == "exit") {
				this->~Server();
			}
			else {
				system(command.c_str());
			}
		}
	} }.detach();
}

void Server::loop()
{
	int nSocket = slt.Wait(std::chrono::milliseconds(100));

	for (int i = 0; i < nSocket; ++i) {
		net::Socket sock = slt.GetReadySocket(i);

		Json::Reader reader;
		Json::Value root;
		char buf[1024] = "";
		try {
			if (sock == listenSock) {
				auto client = listenSock.Accept();
				slt.Add(client);

				ServerLog(Log::Info, "Connected " + net::GetPeerAddressString(client));
			}
			else {
				sock.Receive(buf, 1024);

				reader.parse(buf, root);
				if (root["type"].asString() == "login") {
					loginUser(root, sock);
				}
				else if (root["type"].asString() == "register") {
					registerUser(root, sock);
				}
				else if (root["type"].asString() == "message") {
					Message(root, sock);
				}
				else if (root["type"].asString() == "move") {
					//Move(root, listenSock, slt, sock);
				}
			}
		}
		catch (std::exception&) {
			if (sock == listenSock) {
				exit(true);
			}
			ServerLog(Log::Info, "Disconnected " + net::GetPeerAddressString(sock));
			slt.Remove(sock);
			sock.Close();
		}
	}
}

void Server::ServerLog(int flag, std::string msg)
{
	timer = time(NULL);
	localtime_s(&curtime, &timer);

	t_hour = std::to_string(curtime.tm_hour);
	if (curtime.tm_hour < 10) {
		t_hour = "0" + t_hour;
	}
	t_min = std::to_string(curtime.tm_min);
	if (curtime.tm_min < 10) {
		t_min = "0" + t_min;
	}
	t_sec = std::to_string(curtime.tm_sec);
	if (curtime.tm_sec < 10) {
		t_sec = "0" + t_sec;
	}

	auto timestr = std::format("[{}:{}:{}]", t_hour, t_min, t_sec);

	if (flag != Log::Cmd::Command && !isEnter.load()) {
		std::cout << '\n';
		isEnter = true;
	}
	std::cout << dye::grey(timestr);
	switch (flag)
	{
	case Server::Log::Warning:
		std::cout << dye::yellow(" [WARNINGS] " + msg) << '\n';
		break;
	case Server::Log::Info:
		std::cout << dye::white(" [INFO|Server] " + msg) << '\n';
		break;
	case Server::Log::Error:
		std::cout << dye::red(" [ERROR] " + msg) << '\n';
		break;
	case Server::Log::Chat:
		std::cout << dye::white((" [INFO|Chat] ") + msg) << '\n';
		break;
	case Server::Log::Cmd::Log:
		std::cout << dye::light_blue(" [CMD|Log] ") << dye::white(msg) << '\n';
		break;
	case Server::Log::Cmd::Command:
		std::cout << dye::light_blue(" [CMD|Command] ");
		break;
	}
}
void Server::loginUser(const Json::Value& root, net::Socket& sock)
{
	auto userid = root["id"].asString();
	auto password = root["pwd"].asString();
	try {
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("SELECT usrpwd FROM usracc WHERE usrid=?"));
		stmnt->setString(1, userid);

		auto res = stmnt->executeQuery();
		if (res->next()) {
			if (res->getString("usrpwd") == password) {
				auto succmsg = "succ_login";
				sock.Send(succmsg, static_cast<int>(strlen(succmsg)));

				ServerLog(Log::Info, "Joined \'" + userid + "\'");

				umap.insert({ net::GetPeerAddressString(sock), userid });
			}
			else {
				auto failmsg = "fail_pwd";
				sock.Send(failmsg, static_cast<int>(strlen(failmsg)));
			}
		}
		else {
			throw std::exception();
		}
	}
	catch (std::exception&) {
		auto failmsg = "fail_noexist";
		sock.Send(failmsg, static_cast<int>(strlen(failmsg)));
	}
}
void Server::registerUser(const Json::Value& root, net::Socket& sock)
{
	auto userid = root["id"].asString();
	auto password = root["pwd"].asString();
	try {
		std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("INSERT INTO usracc VALUES(?, ?)"));
		stmnt->setString(1, userid);
		stmnt->setString(2, password);

		stmnt->executeQuery();

		ServerLog(Log::Info, "Joined \'" + userid + "\'");
		auto str = "succ_reg";
		sock.Send(str, static_cast<int>(strlen(str)));

		umap.insert({ net::GetPeerAddressString(sock), userid });
	}
	catch (std::exception& ex) {
		if (std::string(ex.what()).find("PRIMARY") != std::string::npos) {
			auto errmsg = "fail_exist";
			sock.Send(errmsg, static_cast<int>(strlen(errmsg)));
		}
	}
}

void Server::Message(const Json::Value& root, net::Socket& sock)
{
	auto usr = root["id"].asString();
	auto msg = root["message"].asString();

	if (usr != "" || msg != "") {
		++logcount;
		ServerLog(Log::Chat, std::format("[{}]: {}", usr, msg));

		Json::Value json;
		json["type"] = "message";
		json["id"] = usr;
		json["message"] = msg;

		Json::FastWriter writer;
		std::string jsonstr = writer.write(json);
		for (int i = 0; i < slt.Count(); ++i) {
			auto out = slt[i];
			if (out != listenSock && out != sock) {
				out.Send(jsonstr.c_str(), static_cast<int>(jsonstr.length()));
			}
		}
		try {
			std::unique_ptr<sql::PreparedStatement> stmnt(conn->prepareStatement("INSERT INTO serverlog VALUES(?, ?, ?, ?)"));
			stmnt->setString(1, std::to_string(logcount));
			stmnt->setString(2, to_datetime(curtime));
			stmnt->setString(3, msg);
			stmnt->setString(4, usr);

			stmnt->executeQuery();
		}
		catch (std::exception& e) {
			ServerLog(Log::Error, e.what());
		}
	}
}
std::string Server::to_datetime(const tm& tm)
{
	timer = time(NULL);
	localtime_s(&curtime, &timer);

	std::string year = std::to_string(tm.tm_year + 1900);
	std::string mon = std::to_string(tm.tm_mon + 1);
	if ((tm.tm_mon + 1) < 10) {
		mon = "0" + mon;
	}
	std::string day = std::to_string(tm.tm_mday + 1);
	if ((tm.tm_mday + 1) < 10) {
		day = "0" + day;
	}

	return year + "-" + mon + "-" + day + " " + t_hour + ":" + t_min + ":" + t_sec;
}
