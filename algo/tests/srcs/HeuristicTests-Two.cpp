//
// Created by Peer De bakker on 7/14/22.
//

#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

// Open two's
TEST_CASE_METHOD(Gamestate, "Open Two W-E", "[HeuristicTests - Open Two]") {
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + EAST);
	REQUIRE(this->get_h() == -LineValues[OPEN_TWO]);
	this->reset();
	this->change_player();
	this->set(middle_idx, 1);
	this->place_stone(middle_idx + EAST);
	REQUIRE(this->get_h() == LineValues[OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Open Two N-S", "[HeuristicTests - Open Two]") {
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTH);
	REQUIRE(this->get_h() == -LineValues[OPEN_TWO]);
	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTH);
	REQUIRE(this->get_h() == LineValues[OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Open Two NW-SE", "[HeuristicTests - Open Two]") {
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTHEAST);
	REQUIRE(this->get_h() == -LineValues[OPEN_TWO]);
	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTHEAST);
	REQUIRE(this->get_h() == LineValues[OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Open Two NE-SW", "[HeuristicTests - Open Two]") {
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTHWEST);
	REQUIRE(this->get_h() == -LineValues[OPEN_TWO]);
	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTHWEST);
	std::cerr << *this;
	REQUIRE(this->get_h() == LineValues[OPEN_TWO]);
}

// Half open two's
TEST_CASE_METHOD(Gamestate, "Half Open Two W-E", "[HeuristicTests - Half Open Two]") {
	this->set(middle_idx + WEST, 1);
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + EAST);
	REQUIRE(this->get_h() == -LineValues[HALF_OPEN_TWO]);
	this->reset();
	this->change_player();
	this->set(middle_idx + 2 * EAST, 0);
	this->set(middle_idx, 1);
	this->place_stone(middle_idx + EAST);
	REQUIRE(this->get_h() == LineValues[HALF_OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Half Open Two N-S", "[HeuristicTests - Half Open Two]") {
	this->set(middle_idx + NORTH, 1);
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTH);
	REQUIRE(this->get_h() == -LineValues[HALF_OPEN_TWO]);
	this->reset();
	this->change_player();

	this->set(middle_idx + 2 * SOUTH, 0);
	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTH);
	REQUIRE(this->get_h() == LineValues[HALF_OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Half Open Two NW-SE", "[HeuristicTests - Half Open Two]") {
	this->set(middle_idx + NORTHWEST, 1);
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTHEAST);
	REQUIRE(this->get_h() == -LineValues[HALF_OPEN_TWO]);
	this->reset();
	this->change_player();

	this->set(middle_idx + 2 * SOUTHEAST, 0);
	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTHEAST);
	REQUIRE(this->get_h() == LineValues[HALF_OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Half Open Two NE-SW", "[HeuristicTests - Half Open Two]") {
	this->set(middle_idx + NORTHEAST, 1);
	this->set(middle_idx, 0);
	this->place_stone(middle_idx + SOUTHWEST);
	REQUIRE(this->get_h() == -LineValues[HALF_OPEN_TWO]);
	std::cerr << *this;
	this->reset();
	this->change_player();

	this->set(middle_idx + 2 * SOUTHWEST, 0);
	this->set(middle_idx, 1);
	this->place_stone(middle_idx + SOUTHWEST);
	std::cerr << *this;
	REQUIRE(this->get_h() == LineValues[HALF_OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "OPEN OPEN_TWO's", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx); // p0
	this->set_h(0);
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 20); // p1
	this->set_h(0);
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 1); // p0
	this->set_h(0);
	REQUIRE(get_h() == -10);
	place_stone(start_idx + 22); // p1
	this->set_h(0);

	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(h == -LineValues.at(NONE));
}
