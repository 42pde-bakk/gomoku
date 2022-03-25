//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
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

TEST_CASE_METHOD(Gamestate, "Capture-NE", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -19;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-NW", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -21;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-SW", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 19;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-SE", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 21;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx + direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-0", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = 2;
	int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	place_stone(start_idx - direction); // p0
	place_stone(start_idx - 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx - direction]);
}

TEST_CASE_METHOD(Gamestate, "Capture-19", "[CaptureTests]"){ // p1 captures p0
	const int start_idx = 16;
	int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(boards[0][start_idx]);
	REQUIRE_FALSE(boards[0][start_idx - direction]);
}

TEST_CASE("Test_seperating_bit") {
	int sep_bits[] = {19,39,59,79,99,119,139,159,179,199,219,239,259,279,299,319,339,359,379};
	for (auto bit : sep_bits)
		REQUIRE(is_seperating_bit(bit));
	REQUIRE_FALSE(is_seperating_bit(0));
}
