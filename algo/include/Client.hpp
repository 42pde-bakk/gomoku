//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_CLIENT_HPP
#define CLUSTER_CLIENT_HPP
#include "Server.hpp"
#include "Gamestate.hpp"
#define BUFLEN 1000

class Client {
	Server* parent;
	int	fd{};
	struct sockaddr_in	addr{};

	[[nodiscard]] std::vector<int> receive() const;

public:
	explicit Client(Server *s);
	~Client();
	[[nodiscard]] Gamestate	*receiveGamestate() const;
	void send_move(const Move &move) const;
};


#endif //CLUSTER_CLIENT_HPP
