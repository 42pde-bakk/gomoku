//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Colours.hpp"

int main() {
	try {
		Server server;
		std::cerr << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
		std::cerr << "Waiting for client to connect\n";
		Client	client(&server);

		while (true) {
			std::cerr << "Lets receive a gamstate\n";
			Gamestate* gs = client.receiveGamestate();
			std::cerr << "got the gamestate\n";
			gs->print_board(std::cerr);
			gs->generate_children();
			Move move = get_random_move(gs);
			std::cerr << "Move: " << move;
			client.send_move(move);
		}
	}
	catch (std::exception& e) {
		std::cerr << _BLUE _BOLD << e.what() << _END << '\n';
	}
	exit(1);
}
