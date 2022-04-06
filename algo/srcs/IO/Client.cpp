//
// Created by Peer De bakker on 3/22/22.
//

#include "IO/Client.hpp"
#include "Colours.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cassert>

void	Client::error(const char* str) {
	this->closeClient();
	std::cerr << _RED _BOLD << str << "\n" _END;
	throw std::runtime_error(strerror(errno));
}

Client::Client(Server *s) : parent(s) {
	bzero(&this->addr, sizeof(struct sockaddr_in));
	socklen_t size = sizeof(this->addr);
	if ((this->fd = accept(this->parent->getsocketFd(), (struct sockaddr *)&this->addr, &size)) == -1)
		error("Error accepting client");
	std::cerr << "Accepted a client at fd " << this->fd << "\n";
}

Client::~Client() {
	this->closeClient();
}

std::vector<int> Client::receive(size_t bufsize) {
	int		recvRet;
	char	buf[bufsize + 1];
	std::vector<int> intArray;

	bzero(&buf, sizeof(buf));
	if ((recvRet = read(fd, buf, bufsize)) == -1)
		error("Error reading from socket");
	if (recvRet == 0)
		error("Read returned 0");

	for (int i = 0; i < recvRet; i += 4)
		intArray.push_back(buf[i]);
	return (intArray);
}

Gamestate Client::receiveGamestate() {
	Gamestate gs;
	int 	stones_amount;
	int		turn;

	turn = this->receive(4)[0];
	gs.player = turn % 2;
	gs.depth = 0;

	stones_amount = this->receive(4)[0];
	for (int i = 0; i < stones_amount; i++) {
		std::vector<int> arr = this->receive(12);
		int y = arr[0],
			x = arr[1],
			colour = arr[2];
		assert(colour == 1 || colour == 2);
		gs.set(y * 20 + x, colour - 1);
	}
	return (gs);
}


void Client::send_move(const Move &move) {
	char buff[BUFSIZ];
	// We have to translate the move to Pythons X,Y system (no seperating bit)
	const int y = move.move_idx / REALBOARDWIDTH; // -1 to account for the seperating bit
	const int x = move.move_idx % REALBOARDWIDTH;
	const int player = move.player + 1;

	dprintf(2, "move_idx = %d, y,x=[%d, %d], player=%d\n", move.move_idx, y, x, player);

	bzero(buff, sizeof(buff));
	memcpy(buff, (void *)&y, sizeof(int));
	memcpy(buff + sizeof(int), (void *)&x, sizeof(int));
	memcpy(buff + 2 * sizeof(int), (void *)&player, sizeof(int));
	for (int i = 0; i < 12; i++) {
		std::cerr << "\\x" << (int)(buff[i]) << ' ';
	}
	std::cerr << "\n";
	int sendRet = write(fd, buff, sizeof(int) * 3);
	if (sendRet <= 0)
		error("Error sending move");
}

bool Client::isAlive() const {
	return (this->fd != -1);
}

void Client::closeClient() {
	close(this->fd);
	this->fd = -1;
}

