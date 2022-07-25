//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include <iostream>
#include "../catch.hpp"
#include "Bitboard.hpp"
#include "Colours.hpp"

//const int middle_idx = 9 * 20 + 9;


TEST_CASE("Bitboard relational operators", "[BitboardTests]") {
	Bitboard bb;
	Bitboard bb2;

	REQUIRE(bb == bb2);
	REQUIRE_FALSE(bb != bb2);

	bb.set(9, 1);
	bb2.set(9, 1);
	REQUIRE(bb == bb2);

	bb.set(3, 0);
	bb2.set(3, 1);
	bb.print_board(std::cerr, false);
	REQUIRE(bb.bitboard_get(3) == 1);
	REQUIRE(bb2.bitboard_get(3) == 2);
	REQUIRE_FALSE(bb == bb2);
}

TEST_CASE("Bitboard empty neighbours", "[BitboardTests]") {
	unsigned int idx = 5;
	unsigned int p = 1; // 0 or 1
	Bitboard bb;
	bb.set(idx, p);
	bb.print_board(std::cerr, false);
	std::cerr << "\n\n";

	Bitboard bb2;
	bb2.set(idx - 1, p);
	bb2.set(idx + 1, p);
	bb2.set(idx + 20 - 1, p);
	bb2.set(idx + 20, p);
	bb2.set(idx + 20 + 1, p);
	Bitboard empty_neighbours(bb.get_empty_neighbours());
	REQUIRE(empty_neighbours == bb2);
	std::cerr << empty_neighbours;
}

TEST_CASE("Bitboard empty neighbours2", "[BitboardTests]") {
	unsigned int idx = 9 * 20 + 9;
	unsigned int p = 1; // 0 or 1
	Bitboard bb;
	bb.set(idx, p);
	bb.print_board(std::cerr, false);
	std::cerr << "\n\n";

	Bitboard bb2;
	bb2.set(idx - 1, p);
	bb2.set(idx + 1, p);
	bb2.set(idx + 20 - 1, p);
	bb2.set(idx + 20, p);
	bb2.set(idx + 20 + 1, p);
	bb2.set(idx - 20 - 1, p);
	bb2.set(idx - 20, p);
	bb2.set(idx - 20 + 1, p);
	Bitboard empty_neighbours(bb.get_empty_neighbours());
	REQUIRE(empty_neighbours == bb2);
	std::cerr << empty_neighbours;
}
