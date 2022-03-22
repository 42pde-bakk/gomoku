//
// Created by Peer De bakker on 3/22/22.
//

#include "Server.hpp"
#include "Colours.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>

static void	error(const char* str) {
	std::cerr << _RED _BOLD << str << "\n" _END;
	throw std::runtime_error(strerror(errno));
}

Server::Server() {
	bzero(&this->serv_addr, sizeof(struct sockaddr_in));
	if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // PF_INET maybe?
		error("Error setting server socket");

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(PORT);
	this->serv_addr.sin_family = INADDR_ANY;

	if (bind(this->sockfd, (struct sockaddr *)&this->serv_addr, sizeof(struct sockaddr)) == -1)
		error("Error binding socket fd");

	if (listen(this->sockfd, BACKLOG_LENGTH) == -1)
		error("Error starting listening");

	//TODO: perhaps I wanna make the socket nonblocking later on
}

int Server::getsocketFd() const {
	return (this->sockfd);
}

Server::~Server() {
	close(this->sockfd);
	this->sockfd = -1;
}
