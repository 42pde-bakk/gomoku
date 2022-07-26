//
// Created by Peer De bakker on 3/22/22.
//

#include "IO/Client.hpp"
#include "Colours.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cassert>


Client::Client(Server *s) : parent(s) {
	bzero(&this->addr, sizeof(struct sockaddr_in));
	socklen_t size = sizeof(this->addr);
	std::cout << "waiting to accept a client\n";
	if ((this->fd = accept(this->parent->getsocketFd(), (struct sockaddr *) &this->addr, &size)) == -1) {
		perror("accept");
		return ;
	}
	std::cout << "Accepted a client at fd " << this->fd << "\n"; // cerr
}

Client::~Client() {
	this->closeClient();
}

std::vector<int> Client::receive(size_t bufsize, bool &error) {
	int recvRet;
	char buf[bufsize + 1];
	std::vector<int> intArray;

	bzero(&buf, sizeof(buf));
	if ((recvRet = read(fd, buf, bufsize)) <= 0) {
		error = true;
		this->closeClient();
		return (intArray);
	}
	for (int i = 0; i < recvRet; i += 4)
		intArray.push_back(buf[i]);
	return (intArray);
}

Gamestate Client::receiveGamestate(bool &error) {
	Gamestate gs;
	int stones_amount;
	int turn;
	std::vector<int> intArray;

	intArray = this->receive(4, error);
	if (error) {
		return (gs);
	}
	if (!this->isAlive())
		return (gs);
	turn = intArray[0];
	gs.player = turn % 2;
	std::vector<int> captures = this->receive(8, error);
	if (error) {
		return (gs);
	}
	std::copy_n(captures.begin(), 2, gs.captures.begin());

	intArray = this->receive(4, error);
	if (error) {
		return (gs);
	}
	stones_amount = intArray[0];

	for (int i = 0; i < stones_amount; i++) {
		std::vector<int> arr = this->receive(12, error);
		if (error) {
			return (gs);
		}

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

	bzero(buff, sizeof(buff));
	memcpy(buff, (void *) &y, sizeof(int));
	memcpy(buff + sizeof(int), (void *) &x, sizeof(int));
	memcpy(buff + 2 * sizeof(int), (void *) &player, sizeof(int));

	const ssize_t sendRet = write(fd, buff, sizeof(int) * 3);
	if (sendRet == -1) {
		perror("Send");
		this->closeClient();
	}
}

bool Client::isAlive() const {
	return (this->fd != -1);
}

void Client::closeClient() {
	close(this->fd);
	this->fd = -1;
}
