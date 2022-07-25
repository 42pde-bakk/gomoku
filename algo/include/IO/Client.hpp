//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_CLIENT_HPP
#define GOMOKUBOT_CLIENT_HPP

#include "Gomoku.hpp"
#include "Server.hpp"
#include "Gamestate.hpp"

class Client {
	Server *parent;
	int fd{};
	struct sockaddr_in addr{};

	[[nodiscard]] std::vector<int> receive(size_t bufsize, bool &error);

public:
	explicit Client(Server *s);

	~Client();

	[[nodiscard]] Gamestate receiveGamestate(bool &error);

	void closeClient();

	void send_move(const Move &move);

	[[nodiscard]] bool isAlive() const;
};


#endif //GOMOKUBOT_CLIENT_HPP
