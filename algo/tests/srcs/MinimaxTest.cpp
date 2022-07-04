//
// Created by Peer De bakker on 3/25/22.
//

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"
#include "Colours.hpp"

const int middle_idx = 9 * 20 + 9;
const int DEPTH = 4;

void	place_stones(Gamestate *gs) {
	int indices[] = {middle_idx, middle_idx + 1, middle_idx + 2};
	for (int index : indices) {
		gs->place_stone(index);
	}
}

TEST_CASE("Basic Minimax test", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	place_stones(gs);
	elapsed_time = 0;

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, DEPTH, gs->get_player());
	REQUIRE(minimax_res);
	auto end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Minimax took " << elapsed_time << " ms. to find " << minimax_res << "\n" _END;
	std::cerr << "Minimax returned:\n" << *minimax_res << "\n";

	delete gs;
}

TEST_CASE("Alphabeta Test", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	place_stones(gs);
	elapsed_time = 0;

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = minimax_alphabeta_start(gs, DEPTH, gs->get_player());
	REQUIRE(ab_res);
	auto end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time << " ms\n" _END;
}

TEST_CASE("Compare minimax with AB-pruning", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	auto *gs2 = new Gamestate();

	place_stones(gs);
	place_stones(gs2);
	std::cerr << "original:\n" << *gs << '\n';
	elapsed_time = 0;

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, 3, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Minimax took " << elapsed_time << " ms\n" _END;
	REQUIRE(minimax_res);
	minimax_res->print_heuristic(std::cerr);

	start_time = std::chrono::steady_clock::now();
	elapsed_time = 0;
	Gamestate *ab_res = minimax_alphabeta_start(gs2, 3, gs2->get_player());
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time << " ms\n" _END;
	REQUIRE(ab_res);
	ab_res->print_heuristic(std::cerr);

	std::cerr << "minimax_res:\n" << *minimax_res;
	std::cerr << "ab_res:\n" << *ab_res;
	std::cerr << "threaded: " << THREADED << "\n";
	REQUIRE(*minimax_res == *ab_res);
	delete gs; delete gs2;
}

TEST_CASE("Whoo", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	elapsed_time = 0;

	place_stones(gs);

	iterative_deepening(gs, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time << " ms\n" _END;

	delete gs;
}

TEST_CASE_METHOD(Gamestate, "Faulty", "[MinimaxTests]") {
	place_stone(middle_idx); //p0
	elapsed_time = 0;
	start_time = std::chrono::steady_clock::now();
	change_player();
	place_stone(middle_idx - 1); //p1
	this->place_stone(middle_idx + 20); //p0
	this->place_stone(middle_idx + 1); //p1
	this->place_stone(middle_idx + 1 * 19); //p0
	this->place_stone(middle_idx - 20); //p1
	this->place_stone(middle_idx + 2 * 19); //p0
	this->place_stone(middle_idx - 20 - 1 * 21); //p1
	this->place_stone(middle_idx + 3 * 19); //p0
	this->calcH();
	print_heuristic(std::cout);
	std::cerr << *this << "\n\n\n";

//	Gamestate* result = alphabeta(this, 2, this->get_player());
	Gamestate *result = minimax(this, 1, player);
//	Gamestate*	result = iterative_deepening(this, this->get_player());
	auto end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time << " ms\n" _END;
	REQUIRE(result);
	std::cerr << "endresult = " << *result << "\n\n";
//	std::cerr << "History:\n";
//	result->print_history(std::cerr, false);
	for (auto child : children) {
		print_board(std::cout, false);
		print_heuristic(std::cout);
		std::cout << "\n\n";
	}
}

TEST_CASE_METHOD(Gamestate, "Not fucking blocking", "[MinimaxTests]") {
	elapsed_time = 0;
	start_time = std::chrono::steady_clock::now();
	const unsigned int bottom_right = 378;
	this->set(bottom_right, 0);
	this->set(bottom_right - 1, 0);
	this->set(bottom_right - 40, 0);
	const int anchor = bottom_right - 60 - 2;
	this->set(anchor, 0);
	this->set(anchor - 1, 0);
	this->set(anchor - 2, 0);
	const int new_anchor = anchor - 60;
	this->set(new_anchor, 0);
	this->set(new_anchor - 1, 0);
	this->set(new_anchor - 2, 0);
	this->set(new_anchor - 3, 0);
	this->set(new_anchor + 21, 0);
	this->set(new_anchor + 20 - 3, 0);

	this->set(bottom_right - 21, 1);
	this->set(bottom_right - 40 - 3, 1);
	this->set(bottom_right - 60 - 1, 1);
	const int black_anchor = bottom_right - 80;
//	this->set(black_anchor, 1);
	this->set(black_anchor - 1, 1);
	this->set(black_anchor - 2, 1);
	this->set(black_anchor - 4, 1);
	this->set(black_anchor - 20, 1);
	this->set(black_anchor - 22, 1);
	this->set(black_anchor - 41, 1);
	this->player = 1;
	calcH();

	std::cerr << *this << '\n';
	auto result = minimax(this, 2, this->player);
	REQUIRE(result);
	std::cerr << *result;
	result->print_heuristic(std::cerr);

//	for (auto child : children) {
//		child->print_board(std::cerr, false);
//		child->print_heuristic(std::cerr);
//		std::cerr << '\n';
//	}

}
