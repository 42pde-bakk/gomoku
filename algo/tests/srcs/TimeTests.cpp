//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include <iostream>
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Threadpool.hpp"

const int middle_idx = 9 * 20 + 9;


TEST_CASE_METHOD(Gamestate, "Hala madrid", "[TimeTests]") {
	this->place_stone(middle_idx);
	this->place_stone(middle_idx + 1);
	this->place_stone(middle_idx - 1);

	std::cerr << *this;
	generate_children(0);
	std::cerr << "after generate_children\n";
	const size_t kids = children.size();
	std::cerr << "generated " << kids << " kids\n";
	REQUIRE(kids >= 5);
//	threadpool.stop_threads();
}
