#include <iostream>
#include <string>
#include <sstream>
#include "../include/ProtocolHandler.hpp"

ProtocolHandler::ProtocolHandler(sf::TcpSocket & sock) : socket(sock)
{
}

bool ProtocolHandler::greet(const std::string & name)
{
	std::string msg = "HI " + std::string(name) + '\0';

	if (socket.send(msg.c_str(), msg.length()) != sf::Socket::Done) {
		std::cerr << "Could not greet" << std::endl;
		return false;
	}

	return true;
}

bool ProtocolHandler::move(const std::string & mv)
{
	std::string msg = "MV " + std::string(mv) + '\0';

	if (socket.send(msg.c_str(), msg.length()) != sf::Socket::Done) {
		std::cerr << "Could not move" << std::endl;
		return false;
	}

	return true;
}

bool ProtocolHandler::recieve()
{
	std::size_t received;
	char tmpBuf[BUFFER_LENGTH] = { 0 };

	auto status = sf::Socket::Done;

	while (status != sf::Socket::NotReady) {
		status = socket.receive(tmpBuf, BUFFER_LENGTH, received);
		if (status == sf::Socket::NotReady) {
			return true;
		}
		if (status == sf::Socket::Disconnected || status == sf::Socket::Error) {
			std::cerr << "Couldn't receive." << std::endl;
			return false;
		}
		std::cout << "[Receiving ..." << received << "]" << std::endl;

		std::string buf = std::string(tmpBuf);
		std::stringstream ss(buf);

		std::string token;

		while (std::getline(ss, token, '\n'))
		{
			std::cout << "[Rcv] " << token << std::endl;
			cmds.push(token);
			ss.clear();
		}
	}

	return true;
}

bool ProtocolHandler::haveCmds()
{
	return cmds.size() > 0;
}

std::string ProtocolHandler::extract()
{
	std::string ret = cmds.front();
	cmds.pop();

	return ret;
}

