//
// Created by Peer De bakker on 3/22/22.
//

#include "Gomoku.hpp"
#include "Minimax.hpp"
#include "Gamestate.hpp"
#include "IO/Server.hpp"
#include "IO/Client.hpp"
#include "Colours.hpp"
#include <getopt.h>
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

static void	print_usage() {
	fprintf(stderr, "usage: ./gomokubot [options]\n");
	fprintf(stderr, "Available options:\n\t%s\n\t%s\n\t%s=[%s]\n\t%s\n",
			"--history", "--lookuptable", "--max_children", "amount", "--help (or -h)");
}

static unsigned int get_flags(int argc, char **argv) {
	unsigned int flags = 0;
	int opt;
	long int arg_val;
	const struct option long_options[] = {
			{ "help", no_argument, NULL, 'h'},
			{ "history", no_argument, NULL, 'H'},
			{ "lookuptable", no_argument, NULL, 'l'},
			{ "max_children", required_argument, NULL, 'c'},
	};

	while ((opt = getopt_long(argc, argv, "hHLlm:", long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':
				print_usage();
				exit(EXIT_SUCCESS);
			case 'H':
				flags |= FLAG_HISTORY;
				fprintf(stderr, "HISTORY!\n");
				break ;
			case 'L':
			case 'l':
				flags |= FLAG_LOOKUPTABLE;
				g_uses_lookuptable = true;
				fprintf(stderr, "Lookuptable!\n");
				break ;
			case 'm':
			case 'c':
				flags |= FLAG_MAX_CHILDREN;
				arg_val = std::strtol(optarg, NULL, 10);
				if (arg_val <= 0) {
					fprintf(stderr, "Bad value for --max_children: %s\n", optarg);
					return (-1);
				}
				g_max_children = static_cast<unsigned int>(arg_val);
				break ;
			case '?':
				return (-1);
		}
	}
	return (flags);
}

int main(int argc, char **argv) {
	unsigned int flags = get_flags(argc, argv);

	if (flags == (unsigned int)-1) {
		return (1);
	}

	return (loop(flags));
}
