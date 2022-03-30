//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Utils.hpp"

const int middle_idx = 9 * 20 + 9;

void	place_stones2(Gamestate *gs) {
	int indices[] = {middle_idx, middle_idx + 1, middle_idx + 2};
	for (int index : indices) {
		gs->place_stone(index);
	}
}

TEST_CASE("Bitset hash", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	place_stones2(gs);

	auto start_time = std::chrono::steady_clock::now();
//	for (int i = 0; i < 10000; i++) {
//		auto hats = concat(gs->boards[0], gs->boards[1]);
//	}
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "hashing took " << elapsed_time.count() << " ms\n" _END;
	delete gs;
}
