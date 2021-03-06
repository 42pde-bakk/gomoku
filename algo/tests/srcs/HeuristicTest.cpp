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
	this->set_h();
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 20); // p1
	this->set_h();
	REQUIRE(get_h() == 0);
	place_stone(start_idx + 1); // p0
	this->set_h();
	REQUIRE(get_h() == -10);
	place_stone(start_idx + 22); // p1
	this->set_h();
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

TEST_CASE_METHOD(Gamestate, "Blocked Three", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	place_stone(start_idx);
	place_stone(start_idx + 1);
	place_stone(start_idx + 2);
	place_stone(start_idx + 2 + 20);
	place_stone(start_idx + 3);

	std::cerr << *this;

	REQUIRE(h == 0);
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
	REQUIRE(this->h == -15000); // 2 open fours is not more valuable than 1
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

	for (int i = 0; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
	}
	this->set_h();
	REQUIRE(this->h == -15000);
	this->set(105, 1);
	this->set_h();
	REQUIRE(this->get_h() == -7500);
	this->clear_tile(105);

	this->set(113, 1);
	this->set_h();
	REQUIRE(this->get_h() == -15000);
	this->clear_tile(113);

	this->set(start_idx + 21, 1);
	this->set_h();
	REQUIRE(this->get_h() == -7500);

	this->set(start_idx - (21 * 4), 1);
	this->set_h();
	std::cerr << *this;
	REQUIRE(this->get_h() == 0);
}

TEST_CASE_METHOD(Gamestate, "Upgrade 3-1 W-E", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 1;

	this->set(start_idx, 1);
	this->set(start_idx + 1 * dir, 1);
	this->set(start_idx + 2 * dir, 1);
	this->set(start_idx + 4 * dir, 1);
	this->set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == 7500);
}

TEST_CASE_METHOD(Gamestate, "Upgrade 1-3 W-E", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = -1;

	this->set(start_idx, 1);
	this->set(start_idx + 1 * dir, 1);
	this->set(start_idx + 2 * dir, 1);
	this->set(start_idx + 4 * dir, 1);
	this->set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == 7500);
}

TEST_CASE_METHOD(Gamestate, "Upgrade 3-1 N-S", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 20;

	this->set(start_idx, 1);
	this->set(start_idx + 1 * dir, 1);
	this->set(start_idx + 2 * dir, 1);
	this->set(start_idx + 4 * dir, 1);
	this->set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == 7500);
}

TEST_CASE_METHOD(Gamestate, "Upgrade 1-3 N-S", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = -20;

	this->set(start_idx, 1);
	this->set(start_idx + 1 * dir, 1);
	this->set(start_idx + 2 * dir, 1);
	this->set(start_idx + 4 * dir, 1);
	this->set_h();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == 7500);
}

TEST_CASE_METHOD(Gamestate, "Mistake", "[HeuristicTests]") {
	this->captures[0] = 4;
	set(89, 1);
	set(91, 0);
	set(108, 0);
	set(111, 0);
	set(129, 1);
	set(131, 0);
	set(147, 0);
	set(148, 1);
	set(149, 1);
	set(150, 1);
	set(151, 0);
	set(152, 0);
	set(170, 0);
	set(171, 1);
	set(190, 0);
	set(191, 0);
	set(210, 0);
	set(230, 0);
	set(250, 1);
	this->player = 1;
	set_h();
	std::cerr << *this << '\n';

	generate_children();
	for (auto& child : children) {
		std::cerr << "Child:\n" << *child;
		child->print_heuristic(std::cerr);
	}

}
