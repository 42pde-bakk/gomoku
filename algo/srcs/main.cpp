//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "Server.hpp"
#include "Client.hpp"

int main() {
	Server server;

	std::cerr << "Welcome to Gomokubot!\n";

	while (true) {
		Client	client(&server);
		Gamestate* gs = client.receiveGamestate();
		gs->print_board(std::cerr);
		Move move = get_random_move(gs);
		std::cerr << "Move: " << move;
		client.send_move(move);
	}
}
