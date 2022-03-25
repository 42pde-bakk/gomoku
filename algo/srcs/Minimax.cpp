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

Gamestate *alphabeta(Gamestate *state, int depth, int α, int β, bool maximizing_player) {
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state);

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto& child : state->get_children()) {
			new_state = alphabeta(child, depth - 1, α, β, false);
			if (new_state->h > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->h;
			}
			if (best_state_value >= β)
				break ; // β cutoff
			α = std::max(α, new_state->h);
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = alphabeta(child, depth - 1, α, β,true);
			if (new_state->h < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->h;
			}
			if (best_state_value <= α)
				break ; // α cutoff
			β = std::min(β, new_state->h);
		}
	}
	return (best_state);
}
