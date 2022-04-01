//
// Created by Peer De bakker on 3/22/22.
//

#include <Minimax.hpp>

#include "Gamestate.hpp"
#include <limits>
#include <chrono>

std::chrono::time_point<std::chrono::steady_clock> start_time;
std::chrono::time_point<std::chrono::steady_clock> current_time;
long long int elapsed_time;

long long int times_up() {
	current_time = std::chrono::steady_clock::now();

	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
	return (elapsed_time >= 450);
}

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
			if (new_state->get_heuristic() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_heuristic();
			}
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, true);
			if (new_state->get_heuristic() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_heuristic();
			}
		}
	}
	return (best_state);
}

Gamestate *alphabeta(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state);

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto& child : state->get_children()) {
			new_state = alphabeta(child, depth - 1, alpha, beta, false);
			if (new_state->get_heuristic() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_heuristic();
			}
			if (best_state_value >= beta)
				break ; // β cutoff
			alpha = std::max(alpha, new_state->get_heuristic());
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = alphabeta(child, depth - 1, alpha, beta, true);
			if (new_state->get_heuristic() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_heuristic();
			}
			if (best_state_value <= alpha)
				break ; // α cutoff
			beta = std::min(beta, new_state->get_heuristic());
		}
	}
	return (best_state);
}

Gamestate	*iterative_deepening(Gamestate *gs, int player) {
	start_time = std::chrono::steady_clock::now();
	int	intmax = std::numeric_limits<int>::max(),
		intmin = std::numeric_limits<int>::min();
	int depth = 1;
	Gamestate *result = nullptr;


	while (!times_up() && depth < 3) {
		std::cerr << "Start loop, depth: " << depth << ", elapsed time: " << elapsed_time << '\n';
		gs->clear_children();
		result = alphabeta(gs, depth, intmin, intmax, bool(player));

//		if (result->get_heuristic() >= winCutoff) {
//			return (result);
//		}
		std::cerr << "End of loop, elapsed time: " << elapsed_time << '\n';
		depth++;
	}
	return (result);
}
