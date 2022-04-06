//
// Created by Peer De bakker on 3/25/22.
//

//#define CATCH_CONFIG_MAIN
#include "../catch.hpp"
#include "Gamestate.hpp"
#include "Minimax.hpp"
#include "Colours.hpp"

const int middle_idx = 9 * 20 + 9;

void	place_stones(Gamestate *gs) {
	int indices[] = {middle_idx, middle_idx + 1, middle_idx + 2};
	for (int index : indices) {
		gs->place_stone(index);
	}
}

TEST_CASE("Alphabeta pruning", "[MinimaxTests]") {
	auto *gs = new Gamestate();
	auto *gs2 = new Gamestate();

	place_stones(gs);
	place_stones(gs2);

	start_time = std::chrono::steady_clock::now();
	Gamestate *minimax_res = minimax(gs, 3, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Minimax took " << elapsed_time.count() << " ms\n" _END;

	start_time = std::chrono::steady_clock::now();
	Gamestate *ab_res = alphabeta(gs2, 3, gs->get_player());
	end_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Alpha-Beta pruning took " << elapsed_time.count() << " ms\n" _END;

	REQUIRE(*minimax_res == *ab_res);

	delete gs; delete gs2;
}

TEST_CASE("Whoo", "[MinimaxTests]") {
	auto *gs = new Gamestate();

	place_stones(gs);

	Gamestate *ab_res = iterative_deepening(gs, gs->get_player());
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cerr << _PURPLE "Iterative deepening pruning took " << elapsed_time.count() << " ms\n" _END;

	delete gs;
}

TEST_CASE_METHOD(Gamestate, "Mistake2", "[HeuristicTests]") {
	set(88, 0);
	set(107, 0);
	set(110, 0);
	set(125, 0);
	set(126, 0);
	set(127, 1);
	set(128, 1);
	set(129, 1);
	set(130, 0);
	set(131, 1);
	set(145, 1);
	set(146, 1);
	set(148, 1);
	this->player = 0;
	place_stone(167); // p0
	place_stone(148); // p1
	set_h();
	std::cerr << *this << '\n';
	g_log_heur = true;

	auto* result = iterative_deepening(this, this->get_player());
	std::cerr << *result;
	result->print_heuristic(std::cerr);
//	REQUIRE(result->get_first_move().move_idx == 147);
	REQUIRE(true);
	delete result;
}
