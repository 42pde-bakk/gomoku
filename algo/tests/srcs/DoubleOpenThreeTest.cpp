//
// Created by Peer de Bakker on 7/8/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "DoubleOpenThreeTest 1", "[DoubleOpenThreeTests]") {
	set(middle_idx + 20, 0);
	set(middle_idx + 40, 0);
	set(middle_idx + 1, 0);
	std::cerr << "this->parent: " << this->parent << "\n";
	this->place_stone(middle_idx + 2);
	std::cerr << *this;
	auto child = new Gamestate(*this);

	child->place_stone(0);
	REQUIRE(child->place_stone(middle_idx) == false);
	std::cerr << "\n\n" << *child << "\n";
	std::cerr << child->get_h() << "\n";
	child->print_heuristic(std::cerr);
	REQUIRE(child->get_h() == 2000000);
}

TEST_CASE_METHOD(Gamestate, "Default DoubleOpenThreeTest", "[DoubleOpenThreeTests]") {
	this->set(middle_idx + 2 * NORTHWEST, 0);
	this->set(middle_idx + 3 * NORTHWEST, 0);

	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + 2 * EAST, 0);
	std::cerr << *this;
	REQUIRE(this->place_stone(middle_idx) == false);
}

TEST_CASE_METHOD(Gamestate, "Default DoubleOpenThreeTest But No Space Inbetween", "[DoubleOpenThreeTests]") {
	this->set(middle_idx + 1 * NORTHWEST, 0);
	this->set(middle_idx + 2 * NORTHWEST, 0);

	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + 2 * EAST, 0);
	std::cerr << *this;
	REQUIRE(this->place_stone(middle_idx) == false);
}

TEST_CASE_METHOD(Gamestate, "Looks like a double open 3, but is an open 4", "[DoubleOpenThreeTests]") {
	this->set(middle_idx + 2 * WEST, 0);
	this->set(middle_idx + 3 * WEST, 0);

	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + 2 * EAST, 0);
	std::cerr << *this;
	CHECK(this->place_stone(middle_idx) == true);
	std::cerr << *this;
}

TEST_CASE_METHOD(Gamestate, "Should win but doesnt", "[DoubleOpenThreeTests]") {
	this->set(7 * REALBOARDWIDTH + 7, 0);
	this->set(8 * REALBOARDWIDTH + 7, 0);
	this->set(9 * REALBOARDWIDTH + 7, 0);

	this->set(8 * REALBOARDWIDTH + 8, 1);
	this->set(8 * REALBOARDWIDTH + 10, 0);
//    this->set(8 * REALBOARDWIDTH + 12, 1);

	this->set(9 * REALBOARDWIDTH + 6, 1);
	this->set(9 * REALBOARDWIDTH + 9, 1);
	this->set(9 * REALBOARDWIDTH + 10, 1);
	this->set(9 * REALBOARDWIDTH + 11, 1);
	this->set(9 * REALBOARDWIDTH + 13, 0);

	this->set(10 * REALBOARDWIDTH + 8, 1);
	this->set(10 * REALBOARDWIDTH + 9, 0);
	this->set(10 * REALBOARDWIDTH + 10, 1);
	this->set(10 * REALBOARDWIDTH + 11, 0);

	this->set(11 * REALBOARDWIDTH + 7, 1);
	this->set(11 * REALBOARDWIDTH + 9, 1);
	this->set(11 * REALBOARDWIDTH + 10, 1);
	this->set(11 * REALBOARDWIDTH + 11, 1);

	this->set(12 * REALBOARDWIDTH + 10, 1);
	this->set(12 * REALBOARDWIDTH + 12, 0);

	this->set(13 * REALBOARDWIDTH + 8, 1);
	this->set(13 * REALBOARDWIDTH + 10, 0);

	this->set(6 * REALBOARDWIDTH + 7, 0);
	this->captures[1] = 4;
	this->calcH(0);
	this->change_player();
	std::cerr << "its player " << (int)this->player << "'s turn\n";
	std::cerr << *this;
	print_heuristic(std::cerr);

	auto clone = new Gamestate(*this);
//	bool ret = clone->place_stone(228);
	CHECK_FALSE(clone->place_stone(228));
	std::cerr << *clone << '\n';
	clone->print_heuristic(std::cerr);
	dprintf(2, "created open threes: %u\n", (unsigned int)clone->get_created_open_threes());
}
