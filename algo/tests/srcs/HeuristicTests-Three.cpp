//
// Created by Peer De bakker on 7/14/22.
//

#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;


TEST_CASE_METHOD(Gamestate, "Open 3 with a hole inbetween E-W", "[Open Three tests]") {
	this->set(middle_idx, 0);
	this->set(middle_idx + WEST, 0);
	this->place_stone(middle_idx + 3 * WEST);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->get_h() == -LineValues[OPEN_THREE]);

	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->set(middle_idx + WEST, 1);
	this->place_stone(middle_idx + 3 * WEST);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->get_h() == LineValues[OPEN_THREE]);
}

TEST_CASE_METHOD(Gamestate, "Open 3 with a hole inbetween N-S", "[Open Three tests]") {
	this->set(middle_idx, 0);
	this->set(middle_idx + SOUTH, 0);
	this->place_stone(middle_idx + 3 * SOUTH);
	REQUIRE(this->get_h() == -LineValues[OPEN_THREE]);

	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->set(middle_idx + SOUTH, 1);
	this->place_stone(middle_idx + 3 * SOUTH);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->get_h() == LineValues[OPEN_THREE]);

}

TEST_CASE_METHOD(Gamestate, "Open 3 with a hole inbetween NW-SE", "[Open Three tests]") {
	this->set(middle_idx, 0);
	this->set(middle_idx + SOUTHEAST, 0);
	this->place_stone(middle_idx + 3 * SOUTHEAST);
	REQUIRE(this->get_h() == -LineValues[OPEN_THREE]);

	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->set(middle_idx + SOUTHEAST, 1);
	this->place_stone(middle_idx + 3 * SOUTHEAST);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->get_h() == LineValues[OPEN_THREE]);
}

TEST_CASE_METHOD(Gamestate, "Open 3 with a hole inbetween NE-SW", "[Open Three tests]") {
	this->set(middle_idx, 0);
	this->set(middle_idx + SOUTHWEST, 0);
	this->place_stone(middle_idx + 3 * SOUTHWEST);
	REQUIRE(this->get_h() == -LineValues[OPEN_THREE]);

	this->reset();
	this->change_player();

	this->set(middle_idx, 1);
	this->set(middle_idx + SOUTHWEST, 1);
	this->place_stone(middle_idx + 3 * SOUTHWEST);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->get_h() == LineValues[OPEN_THREE]);
}

TEST_CASE_METHOD(Gamestate, "Three", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	this->set(start_idx + SOUTHWEST, 0);
	this->place_stone(start_idx + 2 * SOUTHWEST);

	std::cerr << *this;
	this->print_heuristic(std::cerr);
	REQUIRE(get_h() == -LineValues.at(OPEN_THREE));
	this->place_stone(start_idx + NORTHEAST);
	std::cerr << *this;
	this->print_heuristic(std::cerr);
	REQUIRE(set_h(0) == -LineValues.at(HALF_OPEN_THREE));
}

//TEST_CASE_METHOD(Gamestate, "Three but no room to grow to five", "[HeuristicTests]") {
//	const int start_idx = middle_idx;
//
//	this->set(start_idx + WEST, 1);
//	this->set(start_idx, 0);
//	this->set(start_idx + EAST, 0);
//	this->set(start_idx + 4 * EAST, 1);
//	this->place_stone(start_idx + 2 * EAST);
//
//	std::cerr << *this;
//	this->print_heuristic(std::cerr);
//	REQUIRE(get_h() == -LineValues.at(NONE));
//}

TEST_CASE_METHOD(Gamestate, "Blocked Three", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->place_stone(start_idx);
	this->place_stone(start_idx + WEST);
	this->place_stone(start_idx + 2 * WEST);
	this->place_stone(start_idx + 2 + SOUTH);
	this->place_stone(start_idx + 3 * WEST);

	std::cerr << *this;

	REQUIRE(h == -LineValues[HALF_OPEN_TWO]);
}

TEST_CASE_METHOD(Gamestate, "Three at the top wall", "[HeuristicTests]") {
	const int start_idx = 5;
	const int dir = 20;
	this->set(start_idx, 0);
	this->set(start_idx + dir, 0);
	this->set(start_idx + 2 * dir, 0);

	this->set_h(0);
	REQUIRE(this->get_h() == -LineValues.at(HALF_OPEN_THREE));
	this->set(start_idx + 3 * dir, 1);
	this->set_h(0);
	REQUIRE(this->get_h() == 0);
	std::cerr << *this;
	this->print_heuristic(std::cerr);
}

TEST_CASE_METHOD(Gamestate, "Three at the bottom wall", "[HeuristicTests]") {
	const int start_idx = 360;
	const int dir = -20;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);

	set_h(0);
	REQUIRE(get_h() == -LineValues.at(HALF_OPEN_THREE));
	set(start_idx + 3 * dir, 1);
	set_h(0);
	REQUIRE(get_h() == 0);
	std::cerr << *this;
	print_heuristic(std::cerr);
}

TEST_CASE_METHOD(Gamestate, "Three at the left wall", "[HeuristicTests]") {
	const int start_idx = 180;
	const int dir = 1;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);

	set_h(0);
	REQUIRE(get_h() == -LineValues.at(HALF_OPEN_THREE));
	set(start_idx + 3 * dir, 1);
	set_h(0);
	REQUIRE(get_h() == 0);
	std::cerr << *this;
	print_heuristic(std::cerr);
}

TEST_CASE_METHOD(Gamestate, "Three at the right wall", "[HeuristicTests]") {
	const int start_idx = 178;
	const int dir = -1;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);

	set_h(0);
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(get_h() == -LineValues.at(HALF_OPEN_THREE));
	set(start_idx + 3 * dir, 1);
	set_h(0);
	REQUIRE(get_h() == 0);
}

TEST_CASE_METHOD(Gamestate, "Three plus one", "[HeuristicTests]") {
const int start_idx = middle_idx;
const int dir = 1;
set(start_idx, 0);
set(start_idx + dir, 0);
set(start_idx + 2 * dir, 0);
set(start_idx - dir, 1);

	set_h(0);
REQUIRE(get_h() == -LineValues.at(HALF_OPEN_THREE));
set(start_idx + 3 * dir, 1);
	set_h(0);
std::cerr << *this;
print_heuristic(std::cerr);
REQUIRE(get_h() == 0);
}