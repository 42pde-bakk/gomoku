//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"
#include "Colours.hpp"

const int middle_idx = 9 * 20 + 9;

void	place_stones(Gamestate *gs) {
	int indices[] = {middle_idx, middle_idx + 1, middle_idx + 2};
	for (int index : indices) {
		gs->place_stone(index);
	}
}

TEST_CASE("Alphabeta pruning", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	auto *gs2 = new Gamestate();

	place_stones(gs);
	place_stones(gs2);

	auto start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, 4, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms\n" _END;

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs2, 4, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), gs->get_player());
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;

	REQUIRE(minimax_res == ab_res);

	delete gs; delete gs2;
}
