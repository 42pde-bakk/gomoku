//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "IO/Server.hpp"
#include "IO/Client.hpp"
#include "Colours.hpp"
#include <chrono>

int main() {
	try {
		Server server;
		std::cerr << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
		std::cerr << "Waiting for client to connect\n";
		Client	client(&server);

		while (true) {
			std::cerr << "Lets receive a gamestate\n";
			Gamestate* gs = client.receiveGamestate();
			auto start_time = std::chrono::steady_clock::now();
			std::cerr << "got the gamestate\n";
//			gs->print_board(std::cerr, true);
			Gamestate *result = minimax(gs, 2, static_cast<bool>(gs->get_player()));
			Move move = result->get_first_move();
			std::cerr << "Move: " << move;
			std::cerr << "Result gamestate: h=" << result->get_heuristic() << ".\n";
//			result->print_board(std::cerr, true);
			auto end_time = std::chrono::steady_clock::now();
			auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
			std::cerr << _PURPLE "Calculating move took " << elapsed_time.count() << " ms\n" _END;
			client.send_move(move);
			delete gs;
		}
	}
	catch (std::exception& e) {
		std::cerr << _RED _BOLD << e.what() << _END << '\n';
	}
	exit(0);
}