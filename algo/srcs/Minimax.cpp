//
// Created by Peer De bakker on 3/22/22.
//

#include <Minimax.hpp>

#include "Gamestate.hpp"

Gamestate *minimax(Gamestate *state, int depth, bool maximizing_player) {
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state);

	std::unique_ptr<Gamestate>	uniquePtr;
	Gamestate	*best_state = uniquePtr.get();
	Gamestate	*new_state;

	state->generate_children();
	if (maximizing_player) {
		best_state->set_heuristic(std::numeric_limits<int>::min());
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, false);
			if (new_state > best_state)
				best_state = new_state;
		}
	} else {
		best_state->set_heuristic(std::numeric_limits<int>::max());
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, true);
			if (new_state < best_state)
				best_state = new_state;
		}
	}
	return (best_state);
}

#include <random>
Move get_random_move(Gamestate *state) {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(0, BOARDSIZE); // guaranteed unbiased
	int random_integer = uni(rng);
	int y = random_integer / BOARDLENGTH;
	int	x = random_integer % BOARDLENGTH;
	int player = state->get_player();
	if (state->boards[0][random_integer] || state->boards[1][random_integer])
		return (get_random_move(state)); // RECURSION!!!

	return Move(y, x, player);
}
