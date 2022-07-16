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
	// g_log = true;
	place_stone(start_idx + 2 * direction); // p1
	std::cerr << *this << std::endl;

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
	this->generate_children();
	for (auto child : children)
		REQUIRE(child->get_captures()[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-E", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;

	int direction = -1;
	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-N", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -20;
	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-S", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 20;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-NE", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -19;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-NW", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = -21;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-SW", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 19;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-SE", "[CaptureTests]") { // p1 captures p0
	const int start_idx = middle_idx;
	int direction = 21;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-0", "[CaptureTests]") { // p1 captures p0
	const int start_idx = 2;
	int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	place_stone(start_idx - direction); // p0
	place_stone(start_idx - 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx - direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-19", "[CaptureTests]") { // p1 captures p0
	const int start_idx = 16;
	int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	place_stone(start_idx + 2 * direction); // p1

	REQUIRE_FALSE(bitboard_get(start_idx));
	REQUIRE_FALSE(bitboard_get(start_idx + direction));
	REQUIRE(this->captures[1] == 2);
}

TEST_CASE_METHOD(Gamestate, "Capture-19-false", "[CaptureTests]") { // p1 captures p0
	const int start_idx = 17;
	int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx - direction); // p1
	place_stone(start_idx + direction); // p0
	// g_log = true;
	place_stone(start_idx + 3 * direction); // p1

	std::cerr << *this << '\n';
	REQUIRE(bitboard_get(start_idx));
	REQUIRE(bitboard_get(start_idx + direction));
}

TEST_CASE("Test_seperating_bit") {
	int sep_bits[] = {19,39,59,79,99,119,139,159,179,199,219,239,259,279,299,319,339,359,379};
	for (auto bit : sep_bits)
		REQUIRE(Bitboard::isSeperatingBitIndex(bit));
	REQUIRE_FALSE(Bitboard::isSeperatingBitIndex(0));
}

TEST_CASE_METHOD(Gamestate, "CanGetCapturedTest East", "[CanGetCapturedTests]") {
	const int start_idx = 16;
	const int direction = 1;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	// A single stone can't be captured
	REQUIRE_FALSE(canGetCaptured(start_idx, direction));
	place_stone(start_idx - direction); //p0

	// Verify that both stones will trigger True in both directions
	REQUIRE(canGetCaptured(start_idx, direction));
	REQUIRE(canGetCaptured(start_idx, -direction));
	REQUIRE(canGetCaptured(start_idx - direction, direction));
	REQUIRE(canGetCaptured(start_idx - direction, -direction));

	// Make sure there are no false positives
	REQUIRE_FALSE(canGetCaptured(start_idx, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx, 20));
	REQUIRE_FALSE(canGetCaptured(start_idx, 21));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 20));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 21));
}

TEST_CASE_METHOD(Gamestate, "CanGetCapturedTest West", "[CanGetCapturedTests]") {
	const int start_idx = 25;
	const int direction = -1;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	// A single stone can't be captured
	REQUIRE_FALSE(canGetCaptured(start_idx, direction));
	place_stone(start_idx - direction); //p0

	// Verify that both stones will trigger True in both directions
	REQUIRE(canGetCaptured(start_idx, direction));
	REQUIRE(canGetCaptured(start_idx, -direction));
	REQUIRE(canGetCaptured(start_idx - direction, direction));
	REQUIRE(canGetCaptured(start_idx - direction, -direction));

	// Make sure there are no false positives
	REQUIRE_FALSE(canGetCaptured(start_idx, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx, 20));
	REQUIRE_FALSE(canGetCaptured(start_idx, 21));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 20));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 21));
}

TEST_CASE_METHOD(Gamestate, "CanGetCapturedTest North", "[CanGetCapturedTests]") {
	const int start_idx = 47;
	const int direction = -20;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	// A single stone can't be captured
	REQUIRE_FALSE(canGetCaptured(start_idx, direction));
	place_stone(start_idx - direction); //p0

	// Verify that both stones will trigger True in both directions
	REQUIRE(canGetCaptured(start_idx, direction));
	REQUIRE(canGetCaptured(start_idx, -direction));
	REQUIRE(canGetCaptured(start_idx - direction, direction));
	REQUIRE(canGetCaptured(start_idx - direction, -direction));

	// Make sure there are no false positives
	REQUIRE_FALSE(canGetCaptured(start_idx, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx, 1));
	REQUIRE_FALSE(canGetCaptured(start_idx, 21));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 1));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 21));
}

TEST_CASE_METHOD(Gamestate, "CanGetCapturedTest South", "[CanGetCapturedTests]") {
	const int start_idx = 47;
	const int direction = 20;

	place_stone(start_idx); // p0
	place_stone(start_idx + direction); // p1
	// A single stone can't be captured
	REQUIRE_FALSE(canGetCaptured(start_idx, direction));
	place_stone(start_idx - direction); //p0

	// Verify that both stones will trigger True in both directions
	REQUIRE(canGetCaptured(start_idx, direction));
	REQUIRE(canGetCaptured(start_idx, -direction));
	REQUIRE(canGetCaptured(start_idx - direction, direction));
	REQUIRE(canGetCaptured(start_idx - direction, -direction));

	// Make sure there are no false positives
	REQUIRE_FALSE(canGetCaptured(start_idx, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx, 1));
	REQUIRE_FALSE(canGetCaptured(start_idx, 21));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 19));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 1));
	REQUIRE_FALSE(canGetCaptured(start_idx - direction, 21));
}
