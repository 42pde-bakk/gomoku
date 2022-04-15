//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include <limits>
#include <chrono>
#include "Colours.hpp"

std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();;
std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();;
long long int elapsed_time;

void check_time_limit() {
	current_time = std::chrono::steady_clock::now();

	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
	if (elapsed_time >= MAX_THINK_TIME) {
		{
			std::lock_guard<std::mutex> lock(cerrMutex);
			std::cout << _RED << _BOLD "elapsed_time = " << elapsed_time << ", LETS THROW THIS BITCH\n" _END;
		}
		throw std::logic_error("Timeout value reached");
	}
}

// Player 1 is the maximizing player
// Player 0 is the minimizing player
Gamestate *minimax(Gamestate *state, int depth, bool maximizing_player) {
	check_time_limit();
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state->calcH());

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

// https://en.wikipedia.org/wiki/Quiescence_search
// https://github.com/alextrevithick/ChessAI/blob/master/AlphaBeta.py
//Gamestate*	queiescence_search(Gamestate* state, int depth, int alpha, int beta) {
//	state->calcH();
//	int evaluation = state->get_h();
//
//	// fail-hard beta cutoff
//	if (evaluation >= beta) {
//		// node (move) fails high
//		return
//	}
//}

Gamestate *alphabeta_(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	check_time_limit();
	if (state->has_winner())
		return (state);
	else if (depth == 0) {
		return (state->calcH());
		// return quiescence search
	}

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
//	if (state->get_children().empty())
//		return (state);
	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto& child : state->get_children()) {
			new_state = alphabeta_(child, depth - 1, alpha, beta, false);
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
			new_state = alphabeta_(child, depth - 1, alpha, beta, true);
			if (new_state->get_h() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value <= alpha)
				break ; // α cutoff
			beta = std::min(beta, new_state->get_h());
		}
	}
	return (best_state);
}

Gamestate *alphabeta(Gamestate *state, int depth, bool maximizing_player) {
	static const int	intmin = std::numeric_limits<int>::min();
	static const int	intmax = std::numeric_limits<int>::max();

	return (alphabeta_(state, depth, intmin, intmax, maximizing_player));
}

Gamestate *pvs_(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	check_time_limit();
	if (depth == 0 || state->has_winner()) // Terminal gamestate
		return (state->calcH());

	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
//	if (state->get_children().empty())
//		return (state);
	if (maximizing_player) {
		auto& children = state->get_children();
		best_state = pvs_(children[0], depth - 1, alpha, beta, false);

		for (size_t i = 1; i < state->get_children().size(); ++i) {
			auto* child = state->get_children()[i];
			new_state = pvs_(child, depth - 1, alpha, beta, false);
			if (alpha < new_state->get_h() && new_state->get_h() < beta) {
				//re-search
			}
			if (new_state->get_h() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			alpha = std::max(alpha, new_state->get_h());
			if (best_state_value >= beta)
				break ; // β cutoff
		}
		for (auto& child : state->get_children()) {
			new_state = pvs_(child, depth - 1, alpha, beta, false);



		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto& child : state->get_children()) {
			new_state = pvs_(child, depth - 1, alpha, beta, true);
			if (new_state->get_h() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value <= alpha)
				break ; // α cutoff
			beta = std::min(beta, new_state->get_h());
		}
	}
	return (best_state);
}

Gamestate	*iterative_deepening(Gamestate *gs, int player) {
	start_time = std::chrono::steady_clock::now();
	current_time = std::chrono::steady_clock::now();

	int depth = 1;
	Gamestate *result = nullptr;

	try {
		while (true) {
			std::cout << "Start loop, depth: " << depth << ", elapsed time: " << elapsed_time << '\n';
//			gs->clear_children();
			result = alphabeta(gs, depth, bool(player));

			if (result->has_winner()) {
				std::cout << "FOUND A WINNER: Player " << result->get_winner() << "\n";
				return (result);
			}

			std::cout << "End of loop, elapsed time: " << elapsed_time << '\n';
			depth++;
			check_time_limit();
		}
	} catch (std::logic_error& e) {
		std::cout << "Caught: " << e.what() << std::endl;
	}
	return (result);
}

