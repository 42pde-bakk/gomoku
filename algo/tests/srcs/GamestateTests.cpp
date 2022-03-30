//
// Created by Peer De bakker on 3/30/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE("Gamestate:: generate children", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	gs->place_stone(0);
	gs->place_stone(middle_idx);
	std::cerr << Bitboard(gs->get_empty_neighbours()) << "\n";
	gs->generate_children();

	assert(gs->get_children().size() == 11);

	delete gs;
}
