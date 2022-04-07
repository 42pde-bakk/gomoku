//
// Created by Peer De bakker on 3/22/22.
//

#include "Gomoku.hpp"
#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "IO/Server.hpp"
#include "IO/Client.hpp"
#include "Colours.hpp"
#if THREADED
# include "Threadpool.hpp"
# include "AsyncQueue.hpp"
# include "JobQueue.hpp"
#endif
#include <chrono>
#include <exception>

int main() {
	try {
#if THREADED
		Threadpool& threadpool = Threadpool::GetInstance();
		JobQueue&	jobQ(getJobQueue());
		AsyncQueue<Gamestate *>&	outputQ(getOutputQueue());
		(void)threadpool;
#endif
		Server server;

		std::cout << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
		std::cout << "Waiting for client to connect\n";

		while (true) {
			Client	client(&server);
			try {
				while (client.isAlive()) {
					std::cout << "Lets receive a gamestate\n";
					Gamestate gs = client.receiveGamestate();
					std::cout << "got the gamestate\n";
					Gamestate *result = iterative_deepening(&gs, gs.get_player());
					Move move = result->get_first_move();
					std::cout << "Move: " << move;
					std::cout << "Result gamestate: h=" << result->get_h() << ".\n";
					current_time = std::chrono::steady_clock::now();
					auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(
							current_time - start_time);
					std::cout << _PURPLE "Calculating move took " << elapsed_time.count() << " ms\n" _END;
					client.send_move(move);
					std::cout << "result @ " << result << "\n";
//					result->print_history(std::cout);
				#if THREADED
					jobQ.waitFinished();
					threadpool.WaitForWorkers();
					outputQ.clear();
				#endif
				}
			} catch (const std::exception& e) {
//				std::cerr << _RED _BOLD << e.what() << _END << '\n';
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << _RED _BOLD << e.what() << _END << '\n';
	}
	// exit(0);
}
