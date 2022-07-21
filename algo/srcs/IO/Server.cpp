//
// Created by Peer De bakker on 3/22/22.
//

#include "IO/Server.hpp"
#include "Colours.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fstream>

static void error(const char *str) {
	std::cout << "server error\n"; // cerr
	std::cout << _RED _BOLD << str << "\n" _END; // cerr
	perror("Server::error");
	exit(1);
//	throw std::runtime_error(strerror(errno));
}

Server::Server() : port(4242u) {
	bzero(&this->serv_addr, sizeof(struct sockaddr_in));
	if ((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) // PF_INET maybe?
		error("Error setting server socket");

	this->serv_addr.sin_family = AF_INET;
	this->serv_addr.sin_port = htons(this->port);
	this->serv_addr.sin_family = INADDR_ANY;

	while (bind(this->sockfd, (struct sockaddr *) &this->serv_addr, sizeof(struct sockaddr)) == -1) {
		std::cout << _RED _BOLD "Address in use, sleeping 1\n" << _END;
		port++;
		this->serv_addr.sin_port = htons(this->port);
	}
	this->writePortNbToFile("portnb.txt");

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

unsigned int Server::getport() const {
	return (this->port);
}

void Server::writePortNbToFile(const std::string &s) const {
	std::ofstream ofile;
	ofile.open(s);

	ofile << this->getport() << std::endl;
	ofile.close();
}
