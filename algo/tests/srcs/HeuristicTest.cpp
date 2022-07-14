//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"

const int middle_idx = 9 * 20 + 9;



TEST_CASE_METHOD(Gamestate, "four", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 21;

	this->set(start_idx, 1);
	this->set(start_idx - 1 * dir, 1);
	this->set(start_idx - 2 * dir, 1);
	this->set(start_idx - 3 * dir, 1);
	this->set_h();
	REQUIRE(this->h == LineValues.at(OPEN_FOUR));

	this->set(start_idx + dir, 0);
	this->set_h();

	REQUIRE(this->h == LineValues.at(HALF_OPEN_FOUR));
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

	std::cerr << *this;
	dprintf(2, "h = %d\n", h);
	REQUIRE(this->h == -2000000); // because it is unbreakable
	REQUIRE(this->winner == 1);
}

TEST_CASE_METHOD(Gamestate, "4-1", "[HeuristicTests]") {
	const int start_idx = middle_idx;
	const int dir = 20;

	this->set(start_idx, 0);
	this->set(start_idx - dir, 0);
	this->set(start_idx - 2 * dir, 0);
	this->set(start_idx - 3 * dir, 0);
	this->set(start_idx + 2 * dir, 0);
	this->set_h();

	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == -LineValues[OPEN_FOUR]); // because it is unbreakable
	REQUIRE(this->winner == 0);
}

TEST_CASE_METHOD(Gamestate, "2 fours", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	this->set(start_idx, 0);
	for (int i = 1; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
		this->set(start_idx - i * 19, 0);
	}
	this->calcH();
	std::cerr << *this;
	print_heuristic(std::cerr);
	REQUIRE(this->h == -LineValues.at(OPEN_FOUR)); // 2 open fours is not more valuable than 1
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

	REQUIRE(this->h == LineValues.at(NONE));
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

	REQUIRE(this->h == LineValues.at(NONE));
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
	REQUIRE(this->h == LineValues.at(HALF_OPEN_FOUR));
	this->player = 0;
	Gamestate *result = minimax(this, 1, false);

	REQUIRE(result->get_h() == LineValues.at(NONE));
}

TEST_CASE_METHOD(Gamestate, "Block", "[HeuristicTests]") {
	const int start_idx = middle_idx;

	for (int i = 0; i < 4; i++) {
		this->set(start_idx - i * 21, 0);
	}
	this->set_h();
	REQUIRE(this->h == -LineValues.at(OPEN_FOUR));
	this->set(105, 1);
	this->set_h();
	REQUIRE(this->get_h() == -LineValues.at(HALF_OPEN_FOUR));
	this->clear_tile(105);

	this->set(113, 1);
	this->set_h();
	REQUIRE(this->get_h() == -LineValues.at(OPEN_FOUR));
	this->clear_tile(113);

	this->set(start_idx + 21, 1);
	this->set_h();
	REQUIRE(this->get_h() == -LineValues.at(HALF_OPEN_FOUR));

	this->set(start_idx - (21 * 4), 1);
	this->set_h();
	std::cerr << *this;
	REQUIRE(this->get_h() == LineValues.at(NONE));
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
	REQUIRE(this->h == LineValues.at(OPEN_FOUR));
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
	REQUIRE(this->h == LineValues.at(OPEN_FOUR));
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
	REQUIRE(this->h == LineValues.at(OPEN_FOUR));
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
	REQUIRE(this->h == LineValues.at(OPEN_FOUR));
}

TEST_CASE_METHOD(Gamestate, "Should block but doesn't", "[HeuristicTests]") {
	this->set(5 * 20 + 7, 0);
	this->set(6 * 20 + 8, 1);
	this->set(7 * 20 + 9, 1);
	this->set(8 * 20 + 10, 1);
	this->set(9 * 20 + 11, 1);
	this->set(7 * 20 + 10, 0);
	this->set(7 * 20 + 11, 0);
	this->set(7 * 20 + 12, 0);
	this->set(7 * 20 + 13, 0); // my move
	this->set(8 * 20 + 9, 0);
	this->set(9 * 20 + 8, 1);
	this->set(9 * 20 + 9, 1);
	this->set(10 * 20 + 8, 1);
	this->set(10 * 20 + 10, 0);
	this->set(10 * 20 + 12, 0);
	this->calcH();
	this->change_player();

	this->generate_children();
	REQUIRE(children[0]->get_move().move_idx == 154);

	this->clear_children();
	elapsed_time = 0;
	start_time = std::chrono::steady_clock::now();
	auto result = minimax_alphabeta_start(this, 3, this->player);
	REQUIRE(result);
	std::cerr << *result;
	std::cerr << result->get_h() << "\n\n\n\n";
	REQUIRE(result->get_first_move(this).move_idx == 154);
}

TEST_CASE_METHOD(Gamestate, "Should block but doesn't - 2", "[HeuristicTests]") {
	this->set(6 * 20 + 7, 0);
	this->set(7 * 20 + 8, 0);
	this->set(8 * 20 + 8, 0);
	this->set(9 * 20 + 8, 0);
	this->set(9 * 20 + 7, 0);
	this->set(10 * 20 + 6, 1);
	this->set(10 * 20 + 7, 1);
	this->set(10 * 20 + 8, 1);

	this->set(6 * 20 + 8, 0);
	this->calcH();
	this->change_player();

	std::cerr << *this;
	print_heuristic(std::cerr);
	this->generate_children();
	auto lastmove = children[0]->get_move();
	std::cerr << children[0]->get_move();
	std::cerr << *children[0];
	REQUIRE(lastmove.move_idx == 5 * 20 + 8);

	this->clear_children();
	elapsed_time = 0;
	start_time = std::chrono::steady_clock::now();
	auto result = minimax_alphabeta_start(this, 4, this->player);
	REQUIRE(result);
	std::cerr << *result;
	std::cerr << result->get_h() << "\n\n\n\n";
	REQUIRE(result->get_first_move(this).move_idx == 5 * 20 + 8);
}

TEST_CASE_METHOD(Gamestate, "Should win but doesnt", "[HeuristicTests]") {
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
    this->calcH();
    this->change_player();
    std::cerr << *this;

    this->generate_children();
    std::cerr << "\n\n\n";
    for (auto child : children) {
        std::cerr << "move_idx: " << child->get_move().move_idx << "\n";
        std::cerr << *child;
        child->print_heuristic(std::cerr);
    }

	REQUIRE(this->children[0]->get_move().move_idx == 228);
    this->clear_children();
    elapsed_time = 0;
    start_time = std::chrono::steady_clock::now();
//    auto result = minimax_alphabeta_start(this, 4, this->player);
    auto result = iterative_deepening(this, this->player);
    REQUIRE(result);
    std::cerr << "\n\n";
    std::cerr << *result;
    std::cerr << result->get_h() << "\n\n\n\nHISTORY:\n";
    REQUIRE(result->get_first_move(this).move_idx == 228);

    result->print_history(std::cerr, false);
}

TEST_CASE_METHOD(Gamestate, "Should block open 3", "[HeuristicTests]") {
    this->set(8 * REALBOARDWIDTH + 9, 0);
    this->set(7 * REALBOARDWIDTH + 9, 0);
    this->set(9 * REALBOARDWIDTH + 8, 0);
    this->set(9 * REALBOARDWIDTH + 7, 0);
    this->set(9 * REALBOARDWIDTH + 6, 0);

    this->set(9 * REALBOARDWIDTH + 9, 1);
    this->set(7 * REALBOARDWIDTH + 8, 1);
    this->set(7 * REALBOARDWIDTH + 7, 1);
    this->set(7 * REALBOARDWIDTH + 6, 1);
    this->set(6 * REALBOARDWIDTH + 6, 1);

    calcH();
    this->place_stone(8 * REALBOARDWIDTH + 8);
    std::cerr << *this << "\n\n\n";

    this->generate_children();
    for (auto child : children) {
        std::cerr << *child << "\n";
        child->print_heuristic(std::cerr);
        std::cerr << "\n\n";
    }
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
