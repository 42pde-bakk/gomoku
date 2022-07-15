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

TEST_CASE_METHOD(Gamestate, "DoubleOpenThreeTest Straight Line", "[DoubleOpenThreeTests]") {
	this->set(middle_idx + 2 * WEST, 0);
	this->set(middle_idx + 3 * WEST, 0);

	this->set(middle_idx + EAST, 0);
	this->set(middle_idx + 2 * EAST, 0);
	std::cerr << *this;
	CHECK(this->place_stone(middle_idx) == false);
	std::cerr << *this;
}
