//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "Colours.hpp"
#include <chrono>

int main() {
	try {
		Server server;
		std::cerr << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
		std::cerr << "Waiting for client to connect\n";
		Client	client(&server);

		while (true) {
			std::cerr << "Lets receive a gamstate\n";
			Gamestate* gs = client.receiveGamestate();
			auto start_time = std::chrono::steady_clock::now();
			std::cerr << "got the gamestate\n";
			gs->print_board(std::cerr);
			Gamestate *result = minimax(gs, 2, static_cast<bool>(gs->get_player()));
			Move move = result->moves[0];
			std::cerr << "Move: " << move;
			auto end_time = std::chrono::steady_clock::now();
			auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			std::cerr << _PURPLE "Calculating move took " << elapsed_time.count() << " ms\n" _END;
			client.send_move(move);
			delete gs;
		}
	}
	catch (std::exception& e) {
		std::cerr << _BLUE _BOLD << e.what() << _END << '\n';
	}
	exit(1);
}
