//
// Created by Peer De bakker on 3/22/22.
//

#include "Client.hpp"
#include "Colours.hpp"
#include <iostream>
#include <unistd.h>
#include <cstring>

static void	error(const char* str) {
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
	close(fd);
	fd = -1;
}

std::vector<int> Client::receive() const {
	int		recvRet;
	char	buf[BUFSIZ + 1];
	std::vector<int> intArray;

	bzero(&buf, sizeof(buf));
	if ((recvRet = read(fd, buf, BUFSIZ)) == -1)
		error("Error reading from socket");
	if (recvRet == 0)
		error("Read returned 0.");

	for (int i = 0; i < recvRet; i += 4)
		intArray.push_back(buf[i]);
	return (intArray);
}

Gamestate *Client::receiveGamestate() const {
	auto	*gs = new Gamestate();
	int 	stones_amount;

	gs->turn = this->receive()[0];
#if LOG
	dprintf(2, "Turn: %d\n", gs->turn);
#endif
	stones_amount = this->receive()[0];
#if LOG
	dprintf(2, "Stones_amount: %d\n", stones_amount);
#endif
	for (int i = 0; i < stones_amount; i++) {
		std::vector<int> arr = this->receive();
		int y = arr[0],
			x = arr[1],
			colour = arr[2];
		gs->boards[colour - 1][y * 19 + x] = true;
#if LOG
		dprintf(2, "y=%d, x=%d, colour=%d\n", y, x, colour);
#endif
	}
	return (gs);
}

void Client::send_move(const Move &move) const {
	char buff[BUFSIZ];

	bzero(buff, sizeof(buff));
	memcpy(buff, (void *)&move.y, sizeof(int));
	memcpy(buff + sizeof(int), (void *)&move.x, sizeof(int));
	memcpy(buff + 2 * sizeof(int), (void *)&move.player, sizeof(int));
//	for (int i = 0; i < 12; i++) {
//		std::cerr << "\\x" << (int)(buff[i]) << ' ';
//	}
//	std::cerr << "\n";
	int sendRet = write(fd, buff, sizeof(int) * 3);
	dprintf(2, "sendRet = %d\n", sendRet);
	if (sendRet <= 0)
		error("Error sending move");
}

