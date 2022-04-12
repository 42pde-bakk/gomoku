//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"
#include "Colours.hpp"

const int middle_idx = 9 * 20 + 9;
const int DEPTH = 1;

void	place_stones(Gamestate *gs) {
	int indices[] = {middle_idx, middle_idx + 1, middle_idx + 2};
	for (int index : indices) {
		gs->place_stone(index);
	}
}

TEST_CASE("Basic Minimax test", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	place_stones(gs);

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, DEPTH, gs->get_player());
	REQUIRE(minimax_res);
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms. to find " << minimax_res << "\n" _END;
	std::cerr << "Minimax returned:\n" << *minimax_res << "\n";

	delete gs;
}

TEST_CASE("Alphabeta Test", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	place_stones(gs);

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs, DEPTH, gs->get_player());
	REQUIRE(ab_res);
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;
}

TEST_CASE("Compare minimax with AB-pruning", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	auto *gs2 = new Gamestate();

	place_stones(gs);
	place_stones(gs2);
	std::cerr << "original:\n" << *gs << '\n';

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, DEPTH, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms\n" _END;
	minimax_res->print_heuristic(std::cerr);

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs2, DEPTH, gs2->get_player());
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;
	ab_res->print_heuristic(std::cerr);

	std::cerr << "minimax_res:\n" << *minimax_res;
	std::cerr << "ab_res:\n" << *ab_res;
	std::cerr << "threaded: " << THREADED << "\n";
	REQUIRE(*minimax_res == *ab_res);
	delete gs; delete gs2;
}

TEST_CASE("Whoo", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	place_stones(gs);

	iterative_deepening(gs, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time.count() << " ms\n" _END;

	delete gs;
}
