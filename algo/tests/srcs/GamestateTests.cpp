//
// Created by Peer De bakker on 3/30/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Minimax.hpp"

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

TEST_CASE("Gamestate:: test move generation", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	gs->place_stone(2);

	gs->generate_children();
	gs->get_children()[0]->generate_children();
//	std::cout << "placed stone on index 2:\n" << gs << '\n';
//	auto *result = minimax(gs, 2, true);
//	assert(result);
	delete gs;
}

TEST_CASE_METHOD(Gamestate, "Gamestate:: test empty neighbours generation", "[MinimaxTests]") {
	this->place_stone(2);
	Bitboard empty_neighbours(this->get_empty_neighbours());

	REQUIRE(empty_neighbours.count() == 5);
	this->place_stone(1);
	Bitboard empty_neighbours2(this->get_empty_neighbours());

	REQUIRE(empty_neighbours2.count() == 6);
}
