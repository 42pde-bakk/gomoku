//
// Created by Peer De bakker on 3/30/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Minimax.hpp"
#include "Threadpool.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE("Gamestate:: sorting_children", "[GamestateTests]") {
	auto *gs = new Gamestate();

	gs->set(middle_idx, 0);
	gs->place_stone(middle_idx + 1);
//	gs->set(middle_idx + 1, 0);
//	gs->place_stone(middle_idx);
//	gs->place_stone(0);
//	gs->place_stone(middle_idx + 1);
//	std::cerr << Bitboard(gs->get_empty_neighbours()) << "\n";
	std::cerr << *gs << "\n";
	gs->generate_children();
	auto children = gs->get_children();
	auto move = children[0]->get_move();

	for (int i = 0; i < children.size(); i++) {
		std::cerr << "[ " << i << " ]\n";
		std::cerr << "Move: " << children[i]->get_move();
		std::cerr << "heuristic: " << children[i]->get_h() << '\n';
		std::cerr << *children[i] << "\n";
	}

//	REQUIRE(move.move_idx == middle_idx - 1 || move.move_idx == middle_idx + 2);

	std::cerr << "heyooo\n";

	delete gs;
}

TEST_CASE("Gamestate:: generate children", "[GamestateTests]") {
	auto *gs = new Gamestate();

	gs->place_stone(0);
	gs->place_stone(middle_idx);
	std::cerr << Bitboard(gs->get_empty_neighbours()) << "\n";
	gs->generate_children();

	assert(gs->get_children().size() == 11);

	std::cerr << "heyooo\n";

	delete gs;
}

TEST_CASE("Gamestate:: test move generation", "[GamestateTests]") {
	auto *gs = new Gamestate();

	gs->place_stone(2);

	gs->generate_children();
	gs->get_children()[0]->generate_children();
//	std::cout << "placed stone on index 2:\n" << gs << '\n';
//	auto *result = minimax(gs, 2, true);
//	assert(result);
	delete gs;
}

TEST_CASE_METHOD(Gamestate, "Gamestate:: test empty neighbours generation", "[GamestateTests]") {
	this->place_stone(2);
	Bitboard empty_neighbours(this->get_empty_neighbours());

	REQUIRE(empty_neighbours.count() == 5);
	this->place_stone(1);
	Bitboard empty_neighbours2(this->get_empty_neighbours());

	REQUIRE(empty_neighbours2.count() == 6);
}

TEST_CASE("Gamestate::get_opponent_stone()", "[GamestateTests]") {
	REQUIRE(Gamestate::get_opponent_stone(1) == 2);
	REQUIRE(Gamestate::get_opponent_stone(2) == 1);
	REQUIRE(Gamestate::get_opponent_stone(0) == 3);
	REQUIRE(Gamestate::get_opponent_stone(3) == 0);
}

TEST_CASE_METHOD(Gamestate, "Gamestate::isUnbreakable", "[GamestateTests]") {
	const int start = 15;
	const int dir = 19;
	this->place_stone(start); // p0
	this->place_stone(start + 20); // p1
	this->place_stone(start + dir); // p0
	this->place_stone(start + 40); // p1
	REQUIRE(this->canGetCaptured(start + 20, 20));
	REQUIRE(this->canGetCaptured(start + 40, 20));
	REQUIRE(this->canGetCaptured(start + 20, -20));
	REQUIRE(this->canGetCaptured(start + 40, -20));

//	std::cerr << *this << "\n";
	this->place_stone(start + 2 * dir); // p0
	this->place_stone(start + 60); // p1
	this->place_stone(start + 3 * dir); // p0
	this->place_stone(start - 1); // p1
	this->place_stone(start + 4 * dir); // p0
	this->place_stone(start + 80); // p1
//
	std::cerr << *this << "\n";
	REQUIRE(isUnbreakable(start, start + 4 * dir, dir));
	this->place_stone(start + 39); // p0
	this->place_stone(start - 2); // p1
	std::cerr << *this << "\n";
	REQUIRE_FALSE(isUnbreakable(start, start + 4 * dir, dir));
}
