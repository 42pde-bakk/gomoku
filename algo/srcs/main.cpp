//
// Created by Peer De bakker on 3/22/22.
//

#include "Gomoku.hpp"
#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "IO/Server.hpp"
#include "IO/Client.hpp"
#include "Colours.hpp"
#include <cstring>

#include <chrono>
#include <csignal>
#include <exception>

static bool isAlive = true;
static Client *c;
struct sigaction sigact;

void sighandler(int sig, siginfo_t *siginfo, void *context) {
	(void)sig;
	(void)siginfo;
	(void)context;
	isAlive = false;
}

void	set_signal_handler() {
	sigact.sa_sigaction = &sighandler;
	sigact.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &sigact, NULL)) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}
}

int loop(unsigned int flags) {
#if THREADED
# include "Threadpool.hpp"
# include "AsyncQueue.hpp"
# include "JobQueue.hpp"
	Threadpool& threadpool = Threadpool::GetInstance();
	AsyncQueue<Gamestate *>&	outputQ(getOutputQueue());
#endif
	Server server;

	std::cout << "Welcome to Gomokubot! Connect on port " << server.getport() << "\n";
	std::cout << "Waiting for client to connect\n";
	std::cout << "Sizeof gamestate = " << sizeof(Bitboard) << ", " << sizeof(Heuristic) << ", " << sizeof(Gamestate)
			  << "\n";

	set_signal_handler();

	while (isAlive) {
		c = NULL;
		Client client(&server);
		while (client.isAlive()) {
			bool error = false;
			std::cout << "Lets receive a gamestate\n";
			Gamestate gs = client.receiveGamestate(error);
			gs.calcH((unsigned int)-1);
			if (!client.isAlive()) {
				std::cerr << "Client disconnected.\n";
				break;
			}
			Gamestate *result = iterative_deepening(&gs, gs.get_player());
			Move move = result->get_first_move(&gs);
			std::cout << "Move: " << move;
			std::cout << "Result gamestate: h=" << result->get_h() << ".\n";
			current_time = std::chrono::steady_clock::now();
			elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
			std::cout << _PURPLE "Calculating move took " << elapsed_time << " ms.\n" _END;
			client.send_move(move);
			if (flags & FLAG_HISTORY) {
				result->print_history(std::cout, true);
			}
#if THREADED
			std::cout << "lets wait for the workers\n";
			threadpool.WaitForWorkers();
			std::cout << "waited for the workers\n";
			outputQ.clear();
#endif
		}
	}
	return (0);
}

unsigned int get_flags(int argc, char **argv) {
	unsigned int flags = 0;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "history") == 0) {
			flags |= FLAG_HISTORY;
		}
	}
	return (flags);
}

int main(int argc, char **argv) {
	unsigned int flags = get_flags(argc, argv);
	return (loop(flags));
}
