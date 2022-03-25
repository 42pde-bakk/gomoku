//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "Capture-W", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;

	int direction = 1;
	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-E", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;

	int direction = -1;
	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-N", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -20;
	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-S", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 20;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}
