//
// Created by Peer De bakker on 3/22/22.
//

#include <Minimax.hpp>

#include "Gamestate.hpp"

// Player 1 is the maximizing player
// Player 0 is the minimizing player
Gamestate *minimax(Gamestate *state, int depth, bool maximizing_player) {
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state);

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, false);
			if (new_state->h > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->h;
			}
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, true);
			if (new_state->h < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->h;
			}
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
	int player = state->get_player();
	if (state->boards[0][random_integer] || state->boards[1][random_integer])
		return (get_random_move(state)); // RECURSION!!!

	return Move(random_integer, player);
}
