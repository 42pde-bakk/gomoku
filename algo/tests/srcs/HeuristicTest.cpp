//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "open 2", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx); // p0
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_heuristic() == -10);
	place_stone(start_idx + 22); // p1
	REQUIRE(h == -10);
}

TEST_CASE_METHOD(Gamestate, "half open 2", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	dprintf(2, "start_idx = %d\n", start_idx);

	place_stone(start_idx); // p0
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_heuristic() == -10);
//	g_log = true;
	place_stone(start_idx - 1); // p1
//	g_log = false;
	print_board(std::cerr, false);
	REQUIRE(h == 5);
}
