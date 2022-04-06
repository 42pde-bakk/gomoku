//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include <limits>
#include <chrono>

std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();;
std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();;
long long int elapsed_time;

void check_time_limit() {
	current_time = std::chrono::steady_clock::now();

	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
	if (elapsed_time >= MAX_THINK_TIME) {
		dprintf(2, "elapsed_time = %llu, LETS THROW THIS BITCH\n", elapsed_time);
		throw std::logic_error("Timeout value reached");
	}
}

// Player 1 is the maximizing player
// Player 0 is the minimizing player
Gamestate *minimax(Gamestate *state, int depth, bool maximizing_player) {
	check_time_limit();
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
			if (new_state->get_h() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = minimax(child, depth - 1, true);
			if (new_state->get_h() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
		}
	}
	return (best_state);
}

Gamestate *internal_alphabeta(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	check_time_limit();
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state);

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
	if (state->get_children().empty())
		return (state);
	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto& child : state->get_children()) {
			new_state = internal_alphabeta(child, depth - 1, alpha, beta, false);
			if (new_state->get_h() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value >= beta)
				break ; // β cutoff
			alpha = std::max(alpha, new_state->get_h());
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = internal_alphabeta(child, depth - 1, alpha, beta, true);
			if (new_state->get_h() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value <= alpha)
				break ; // α cutoff
			beta = std::min(beta, new_state->get_h());
		}
	}
	if (best_state == nullptr) {
		dprintf(2, "depth=%d, alpha=%d, beta=%d, maximizing_player=%d\n", depth, alpha, beta, maximizing_player);
		dprintf(2, "state->children.size() = %zu\n", state->get_children().size());
	}
	return (best_state);
}

Gamestate *alphabeta(Gamestate *state, int depth, bool maximizing_player) {
	static const int	intmin = std::numeric_limits<int>::min();
	static const int	intmax = std::numeric_limits<int>::max();

	return (internal_alphabeta(state, depth, intmin, intmax, maximizing_player));
}

Gamestate	*iterative_deepening(Gamestate *gs, int player) {
	start_time = std::chrono::steady_clock::now();

	int depth = 2;
	Gamestate *result = nullptr;

	try {
		while (true) {
			std::cerr << "Start loop, depth: " << depth << ", elapsed time: " << elapsed_time << '\n';
//			gs->clear_children();
			result = alphabeta(gs, depth, bool(player));

			if (result->has_winner()) {
				std::cerr << "FOUND A WINNER: Player " << result->get_winner() << "\n";
				return (result);
			}

			std::cerr << "End of loop, elapsed time: " << elapsed_time << '\n';
			depth += 2;
			check_time_limit();
		}
	} catch (std::logic_error& e) {
		std::cerr << "Caught: " << e.what() << std::endl;
	}
	return (result);
}

