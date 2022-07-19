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
