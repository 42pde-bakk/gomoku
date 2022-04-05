//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "IO/Server.hpp"
#include "IO/Client.hpp"
#include "Colours.hpp"
#include <chrono>
#include <exception>

int main() {
	try {
		Server server;
		std::cerr << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
		std::cerr << "Waiting for client to connect\n";

		while (true) {
			Client	client(&server);
			try {
				while (client.isAlive()) {
					std::cerr << "Lets receive a gamestate\n";
					Gamestate *gs = client.receiveGamestate();
					std::cerr << "got the gamestate\n";
					Gamestate *result = iterative_deepening(gs, gs->get_player());
					Move move = result->get_first_move();
					std::cerr << "Move: " << move;
					std::cerr << "Result gamestate: h=" << result->get_h() << ".\n";
					current_time = std::chrono::steady_clock::now();
					auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
							current_time - start_time);
					std::cerr << _PURPLE "Calculating move took " << elapsed_time.count() << " ms\n" _END;
					client.send_move(move);
					result->print_history(std::cerr);
					delete gs;
				}
			} catch (const std::exception& e) {
				std::cerr << _RED _BOLD << e.what() << _END << '\n';
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << _RED _BOLD << e.what() << _END << '\n';
	}
	// exit(0);
}
