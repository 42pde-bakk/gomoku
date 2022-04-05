//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "Two", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx); // p0
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_h() == -10);
	place_stone(start_idx + 22); // p1
//	std::cerr << *this << "\n";
//	print_heuristic(std::cerr);
	REQUIRE(h == -10);
}

TEST_CASE_METHOD(Gamestate, "Three", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	set(start_idx, 0);
	set(start_idx + 19, 0);
	set(start_idx + 19 * 2, 0);

	set_h();
	REQUIRE(get_h() == -5000);
	set(start_idx - 19, 1);
	REQUIRE(set_h() == -1000);
}

TEST_CASE_METHOD(Gamestate, "Three at the top wall", "[HeuristicTests]") {
	const int start_idx = 5;
	const int dir = 20;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);

	set_h();
	REQUIRE(get_h() == -1000);
	set(start_idx + 3 * dir, 1);
	set_h();
	REQUIRE(get_h() == 0);
	std::cerr << *this;
	print_heuristic(std::cerr);
}

TEST_CASE_METHOD(Gamestate, "Three at the bottom wall", "[HeuristicTests]") {
	const int start_idx = 360;
	const int dir = -20;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);

	set_h();
	REQUIRE(get_h() == -1000);
	set(start_idx + 3 * dir, 1);
	set_h();
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

	set_h();
	REQUIRE(get_h() == -1000);
	set(start_idx + 3 * dir, 1);
	set_h();
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

	set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(get_h() == -1000);
	set(start_idx + 3 * dir, 1);
	set_h();
	REQUIRE(get_h() == 0);
}

TEST_CASE_METHOD(Gamestate, "Three plus one", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 1;
	set(start_idx, 0);
	set(start_idx + dir, 0);
	set(start_idx + 2 * dir, 0);
	set(start_idx - dir, 1);

	set_h();
	REQUIRE(get_h() == -1000);
	set(start_idx + 3 * dir, 1);
	set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(get_h() == 0);
}

TEST_CASE_METHOD(Gamestate, "four", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 21;

	this->set(start_idx, 1);
	this->set(start_idx - 1 * dir, 1);
	this->set(start_idx - 2 * dir, 1);
	this->set(start_idx - 3 * dir, 1);
	this->set_h();
	REQUIRE(this->h == 15000);

	this->set(start_idx + dir, 0);
	this->set_h();

	REQUIRE(this->h == 7500);
}

TEST_CASE_METHOD(Gamestate, "five", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 20;

	this->set(start_idx, 0);
	this->set(start_idx - dir, 0);
	this->set(start_idx - 2 * dir, 0);
	this->set(start_idx - 3 * dir, 0);
	this->set(start_idx + dir, 0);
	this->set_h();

	REQUIRE(this->h == -2000000); // because it is unbreakable
	REQUIRE(this->winner == 1);
}

TEST_CASE_METHOD(Gamestate, "2 fours", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
		this->set(start_idx - i * 19, 0);
	}
	this->set_h();
	REQUIRE(this->h == -30000);
}

TEST_CASE_METHOD(Gamestate, "BLOCKED BY JAMES", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
	}
	this->set(start_idx - 4 * 21, 1);
	this->set(start_idx + 21, 1);
	this->set_h();

	REQUIRE(this->h == 0);
}

TEST_CASE_METHOD(Gamestate, "BLOCK4", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 1);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 1);
	}
	this->set(start_idx - 4 * 21, 0);
	this->set(start_idx + 21, 0);
	this->set_h();
	print_board(std::cerr, false);
	print_heuristic(std::cerr);

	REQUIRE(this->h == 0);
}

TEST_CASE_METHOD(Gamestate, "MinimaxHeuristicReturn", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 1);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 1);
	}
	this->set(start_idx - 4 * 21, 0);
	this->set_h();
	print_board(std::cerr, false);
	print_heuristic(std::cerr);
	std::cerr << '\n';
	REQUIRE(this->h == 7500);
	this->player = 0;
	Gamestate *result = minimax(this, 1, false);

	REQUIRE(result->get_h() == 0);
}

TEST_CASE_METHOD(Gamestate, "Block", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
		this->set(start_idx - i * 19, 0);
	}
	this->set_h();
	REQUIRE(this->h == -30000);
	this->set(105, 1);
	this->set_h();
	REQUIRE(this->get_h() == -22500);
	this->clear_tile(105);

	this->set(113, 1);
	this->set_h();
	REQUIRE(this->get_h() == -22500);
	this->clear_tile(113);

	this->set(start_idx + 19, 1);
	this->set_h();
	REQUIRE(this->get_h() == -22500);
	this->clear_tile(start_idx + 19);

	this->set(start_idx + 21, 1);
	this->set_h();
	REQUIRE(this->get_h() == -22500);
	this->clear_tile(start_idx + 21);
}

TEST_CASE_METHOD(Gamestate, "Upgrade", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 1;

	this->set(start_idx, 1);
	this->set(start_idx - 1 * dir, 1);
	this->set(start_idx - 2 * dir, 1);
	this->set(start_idx - 4 * dir, 1);
	this->set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == 7500);
}
