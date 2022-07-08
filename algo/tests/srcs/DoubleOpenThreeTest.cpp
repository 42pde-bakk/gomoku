//
// Created by Peer de Bakker on 7/8/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "DoubleOpenThreeTest 1","[DoubleOpenThreeTests") {
	set(middle_idx + 20, 0);
	set(middle_idx + 40, 0);
	set(middle_idx + 1, 0);
	this->place_stone(middle_idx + 2);

}