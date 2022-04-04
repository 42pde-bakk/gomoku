//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"

const int middle_idx = 9 * 20 + 9;

TEST_CASE_METHOD(Gamestate, "open 2", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx); // p0
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_heuristic() == -10);
	place_stone(start_idx + 22); // p1
	REQUIRE(h == -10);
}

TEST_CASE_METHOD(Gamestate, "half open 2", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	dprintf(2, "start_idx = %d\n", start_idx);

	place_stone(start_idx); // p0
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 20); // p1
	REQUIRE(get_heuristic() == 0);
	place_stone(start_idx + 1); // p0
	REQUIRE(get_heuristic() == -10);
//	g_log = true;
	place_stone(start_idx - 1); // p1
//	g_log = false;
	print_board(std::cerr, false);
	REQUIRE(h == 5);
}

TEST_CASE_METHOD(Gamestate, "four", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	this->set(start_idx - 21, 0);
	this->set(start_idx - 42, 0);
	this->set_heuristic();
	REQUIRE(this->h == -100);
	this->set(start_idx - 63, 0);
	this->set_heuristic();
	REQUIRE(this->h == -1000);
}

TEST_CASE_METHOD(Gamestate, "five", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	this->set(start_idx - 21, 0);
	this->set(start_idx - 42, 0);
	this->set_heuristic();
	REQUIRE(this->h == -100);
	this->set(start_idx - 63, 0);
	this->set_heuristic();
	REQUIRE(this->h == -1000);
	this->set(start_idx + 21, 0);
	this->set_heuristic();
	REQUIRE(this->h == -2000000);
}

TEST_CASE_METHOD(Gamestate, "2 fours", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
		this->set(start_idx - i * 19, 0);
	}
	this->set_heuristic();
	std::cerr << *this;
	REQUIRE(this->h == -2000);
}

TEST_CASE_METHOD(Gamestate, "BLOCKED BY JAMES", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
	}
	this->set(start_idx - 4 * 21, 1);
	this->set(start_idx + 21, 1);
	this->set_heuristic();
	std::cerr << h << "\n"<< *this << '\n';
	REQUIRE(this->h == 0);
}

TEST_CASE_METHOD(Gamestate, "MinimaxHeuristicReturn", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
	}
	this->set(start_idx - 4 * 21, 1);
	this->set_heuristic();
	std::cerr << h << "\n"<< *this << '\n';
	REQUIRE(this->h == -500);
	this->player = 1;
	Gamestate *result = minimax(this, 1, true);

	for (auto& child : children) {
		std::cerr << "child: " << child->get_heuristic() << "\n" << *child << "\n";
	}

	REQUIRE(result->get_heuristic() == 0);
}

TEST_CASE_METHOD(Gamestate, "Block", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
		this->set(start_idx - i * 19, 0);
	}
	this->set_heuristic();
	REQUIRE(this->h == -2000);
	this->set(105, 1);
	this->set_heuristic();
	std::cerr << h << '\n' << *this << "\n";
	REQUIRE(this->get_heuristic() == -1500);
	this->clear_tile(105);

	this->set(113, 1);
	this->set_heuristic();
	std::cerr << h << '\n' << *this << "\n";
	REQUIRE(this->get_heuristic() == -1500);
	this->clear_tile(113);

	this->set(start_idx + 19, 1);
	this->set_heuristic();
	std::cerr << h << '\n' << *this << "\n";
	REQUIRE(this->get_heuristic() == -1500);
	this->clear_tile(start_idx + 19);

	this->set(start_idx + 21, 1);
	this->set_heuristic();
	std::cerr << h << '\n' << *this << "\n";
	REQUIRE(this->get_heuristic() == -1500);
	this->clear_tile(start_idx + 21);
}
