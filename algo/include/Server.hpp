//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_SERVER_HPP
#define CLUSTER_SERVER_HPP
#include <arpa/inet.h>
#include <sys/socket.h>
#define PORT 4242
#define BACKLOG_LENGTH 1


class Server {
	int		sockfd;
	struct sockaddr_in serv_addr{};

public:
	Server();
	~Server();
	[[nodiscard]] int	getsocketFd() const;
};


#endif //CLUSTER_SERVER_HPP
