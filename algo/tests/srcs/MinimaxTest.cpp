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

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, DEPTH, gs->get_player());
	REQUIRE(minimax_res);
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms. to find " << minimax_res << "\n" _END;
	std::cerr << "Minimax returned:\n" << *minimax_res << "\n";

	delete gs;
}

TEST_CASE("Alphabeta Test", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	place_stones(gs);

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs, DEPTH, gs->get_player());
	REQUIRE(ab_res);
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;
}

TEST_CASE("Compare minimax with AB-pruning", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	auto *gs2 = new Gamestate();

	place_stones(gs);
	place_stones(gs2);
	std::cerr << "original:\n" << *gs << '\n';

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, DEPTH, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms\n" _END;
	minimax_res->print_heuristic(std::cerr);

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs2, DEPTH, gs2->get_player());
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;
	ab_res->print_heuristic(std::cerr);

	std::cerr << "minimax_res:\n" << *minimax_res;
	std::cerr << "ab_res:\n" << *ab_res;
	std::cerr << "threaded: " << THREADED << "\n";
	REQUIRE(*minimax_res == *ab_res);
	delete gs; delete gs2;
}

TEST_CASE("Whoo", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	place_stones(gs);

	iterative_deepening(gs, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time.count() << " ms\n" _END;

	delete gs;
}

TEST_CASE_METHOD(Gamestate, "Faulty", "[MinimaxTests]") {
	place_stone(middle_idx); //p0
	change_player();
	place_stone(middle_idx - 1); //p1
	change_player();
	this->place_stone(middle_idx + 20); //p0
	change_player();
	this->place_stone(middle_idx + 1); //p1
	change_player();
	this->place_stone(middle_idx + 1 * 19); //p0
	change_player();
	this->place_stone(middle_idx - 20); //p1
	change_player();
	this->place_stone(middle_idx + 2 * 19); //p0
	change_player();
	this->place_stone(middle_idx - 20 - 1 * 21); //p1
	change_player();
	this->place_stone(middle_idx + 3 * 19); //p0
//	change_player();
	this->calcH();
	print_heuristic(std::cout);
	std::cerr << *this << "\n\n\n";

//	Gamestate* result = alphabeta(this, 2, this->get_player());
	Gamestate *result = minimax(this, 1, player);
//	Gamestate*	result = iterative_deepening(this, this->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time.count() << " ms\n" _END;
	std::cerr << "endresult = " << *result << "\n\n";
//	std::cerr << "History:\n";
//	result->print_history(std::cerr, false);
	for (auto child : children) {
		print_board(std::cout, false);
		print_heuristic(std::cout);
		std::cout << "\n\n";
	}
}
