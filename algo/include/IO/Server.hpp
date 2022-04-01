//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_SERVER_HPP
#define GOMOKUBOT_SERVER_HPP

#include "Gomoku.hpp"
#include <arpa/inet.h>
#include <string>
#include <sys/socket.h>
#define PORT 4242
#define BACKLOG_LENGTH 1


class Server {
	int		sockfd;
	int		port{};
	struct sockaddr_in serv_addr{};

public:
	Server();
	~Server();
	[[nodiscard]] int	getsocketFd() const;
	[[nodiscard]] int	getport() const;

private:
	void	writePortNbToFile(const std::string& s) const;
};


#endif //GOMOKUBOT_SERVER_HPP
