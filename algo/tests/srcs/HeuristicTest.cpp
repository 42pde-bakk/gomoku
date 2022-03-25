//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "Collect", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx); // p0
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_heuristic() == -10);
	dprintf(2, "\n\n\n\n\n\n\nyalalalallala\n");
	place_stone(start_idx + 22); // p1
	REQUIRE(h == -10);
	print_board(std::cerr, false);
}
