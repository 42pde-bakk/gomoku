//
// Created by peerdb on 19-7-22.
//

#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "five", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 20;

	this->set(start_idx, 0);
	this->set(start_idx - dir, 0);
	this->set(start_idx - 2 * dir, 0);
	this->set(start_idx - 3 * dir, 0);
	this->set(start_idx + dir, 0);
	this->set_h(0);

	std::cerr << *this;
	dprintf(2, "h = %d\n", h);
	REQUIRE(this->h == -2000000); // because it is unbreakable
	REQUIRE(this->winner == 1);
}


TEST_CASE_METHOD(Gamestate, "Win by blocked ive", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 20;

	this->set(start_idx + NORTH, 1);
	this->set(start_idx, 0);
	this->set(start_idx + SOUTH, 0);
	this->set(start_idx + 2 * SOUTH, 0);
	this->set(start_idx + 3 * SOUTH, 0);
	this->set(start_idx + 5 * SOUTH, 1);
	this->calcH((unsigned int)-1);

	this->generate_children();
	std::cerr << *this;
	this->print_heuristic(std::cerr);
	std::cerr << "\n\n\n";
	REQUIRE(children[0]->get_move().move_idx == start_idx + 4 * SOUTH);

	this->clear_children();
	// TODO: ensure this always happens
}

TEST_CASE_METHOD(Gamestate, "Bogus", "[HeuristicTests]") {
	this->set(middle_idx, 0);
	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + 3 * EAST, 0);
	this->set(middle_idx + NORTHWEST, 0);
	this->set(middle_idx + SOUTH + 2 * WEST, 0);
	this->set(middle_idx + SOUTH + 2 * EAST, 0);

	this->set(middle_idx + NORTH, 1);
	this->set(middle_idx + 2 * EAST, 1);
	this->set(middle_idx + NORTH + 2 * EAST, 1);
	this->set(middle_idx + NORTH + 3 * EAST, 1);
	this->set(middle_idx + NORTH + 4 * EAST, 1);
	this->set(middle_idx + 2 * NORTH + 2 * EAST, 1);


	this->place_stone(middle_idx + NORTH + 5 * EAST);

//	this->generate_children();
	std::cerr << *this;
	this->print_heuristic(std::cerr);
//	std::cerr << "\n\n\n";
//	for (auto child : children) {
//		std::cerr << *child;
//		child->print_heuristic(std::cerr);
//		std::cerr << "\n";
//	}
	REQUIRE(this->values[1][HALF_OPEN_FOUR] == 1);

}
