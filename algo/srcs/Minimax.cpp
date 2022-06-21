//
// Created by Peer De bakker on 3/22/22.
//

#include "Minimax.hpp"
#include "Gamestate.hpp"
#include <limits>
#include <chrono>
#include "Colours.hpp"

#define OUT_OF_TIME 1
#define HAS_TIME_LEFT 0
std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();;
std::chrono::time_point<std::chrono::steady_clock> current_time = std::chrono::steady_clock::now();;
long long int elapsed_time;

int check_thinking_time_left() {
//	if (elapsed_time >= MAX_THINK_TIME) {
//		return (OUT_OF_TIME);
//	}

	current_time = std::chrono::steady_clock::now();
	elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
	if (elapsed_time >= MAX_THINK_TIME) {
		return (OUT_OF_TIME);
	}
	return (HAS_TIME_LEFT);
}

// Player 1 is the maximizing player
// Player 0 is the minimizing player
Gamestate *minimax(Gamestate *state, int depth, bool maximizing_player) {
	if (check_thinking_time_left() == OUT_OF_TIME) {
		return (nullptr);
	}
	check_thinking_time_left();
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

Gamestate *minimax_ab(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player);

Gamestate	*ab_generate_gamestates(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	state->generate_children();
	if (!state->has_children())
		return (nullptr);
	auto children = state->get_children();

	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (auto child : children) {
			new_state = minimax_ab(child, depth - 1, alpha, beta, false);
			if (new_state == nullptr) {
				return (nullptr);
			}
			auto heuristic_value = new_state->get_h();
			if (heuristic_value > best_state_value) {
				best_state = new_state;
				best_state_value = heuristic_value;
			}
			if (best_state_value >= beta) {
//				std::cout << "Beta cutoff, at " << m_idx + 1 << " / " << next_moves.size() << '\n';
				break ; // β cutoff
			}
			alpha = std::max(alpha, heuristic_value);
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (auto child : children) {
			new_state = minimax_ab(child, depth - 1, alpha, beta, true);
			if (new_state == nullptr) {
				return (nullptr);
			}
			auto heuristic_value = new_state->get_h();
			if (heuristic_value < best_state_value) {
				best_state = new_state;
				best_state_value = heuristic_value;
			}
			if (best_state_value <= alpha) {
//				std::cout << "Alpha cutoff, at " << m_idx + 1 << " / " << next_moves.size() << '\n';
				break ; // α cutoff
			}
		}
	}
	return (best_state);
}

Gamestate	*ab_generate_moves_first(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	Gamestate	*best_state = nullptr;
	int			best_state_value;
	Gamestate	*new_state;

	auto next_moves = state->generate_moves();
	assert(!next_moves.empty());

	if (maximizing_player) {
		best_state_value = std::numeric_limits<int>::min();
		for (size_t m_idx = 0ul; m_idx < next_moves.size(); m_idx++) {
			auto& move = next_moves[m_idx];
			auto child = new Gamestate(*state);
			state->add_child(child);
			child->apply_move(move);
			new_state = minimax_ab(child, depth - 1, alpha, beta, false);
			if (new_state == nullptr) {
				return (nullptr);
			}
			if (new_state->get_h() > best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value >= beta) {
//				std::cout << "Beta cutoff, at " << m_idx + 1 << " / " << next_moves.size() << '\n';
				break ; // β cutoff
			}
			alpha = std::max(alpha, new_state->get_h());
		}
	} else {
		best_state_value = std::numeric_limits<int>::max();
		for (size_t m_idx = 0ul; m_idx < next_moves.size(); m_idx++) {
			auto& move = next_moves[m_idx];
			auto child = new Gamestate(*state);
			state->add_child(child);
			child->apply_move(move);
			new_state = minimax_ab(child, depth - 1, alpha, beta, true);
			if (new_state == nullptr) {
				return (nullptr);
			}
			if (new_state->get_h() < best_state_value) {
				best_state = new_state;
				best_state_value = new_state->get_h();
			}
			if (best_state_value <= alpha) {
//				std::cout << "Alpha cutoff, at " << m_idx + 1 << " / " << next_moves.size() << '\n';
				break ; // α cutoff
			}
			beta = std::min(beta, new_state->get_h());
		}
	}
	state->sort_children();
//	std::cout << "oooh my state now has " << state->get_children().size() << " children\n";
	return (best_state);
}

Gamestate *minimax_ab(Gamestate *state, int depth, int alpha, int beta, bool maximizing_player) {
	if (check_thinking_time_left() == OUT_OF_TIME) {
		return (nullptr);
	}
	if (state->has_winner())
		return (state);
	else if (depth == 0) {
		return (state->calcH());
		// return quiescence search
	}
	return (ab_generate_gamestates(state, depth, alpha, beta, maximizing_player));
//	return (best_state);
}

Gamestate *minimax_alphabeta_start(Gamestate *state, int depth, bool maximizing_player) {
	static const int	intmin = std::numeric_limits<int>::min();
	static const int	intmax = std::numeric_limits<int>::max();

	return (minimax_ab(state, depth, intmin, intmax, maximizing_player));
}

extern unsigned int g_nb;
extern unsigned int g_moves;
extern unsigned int g_applied_moves;

Gamestate	*iterative_deepening(Gamestate *gs, int player) {
	static unsigned int items[2] = {0, 0};
	static unsigned int turns = 0;

	start_time = std::chrono::steady_clock::now();
	current_time = std::chrono::steady_clock::now();
	elapsed_time = 0ll;

	int depth = 1;
	Gamestate	*result = nullptr,
				*tmp;

	g_nb = 0;
	g_moves = 0;
	g_applied_moves = 0;
	while (check_thinking_time_left() == HAS_TIME_LEFT) {
		std::cout << "Start loop, depth: " << depth << ", elapsed time: " << elapsed_time << '\n';
		tmp = minimax_alphabeta_start(gs, depth, bool(player));
		if (tmp)
			result = tmp;
		else {
			std::cout << _RED "TIMEOUT\n" _END;
		}

		if (result->has_winner()) {
			std::cout << "FOUND A WINNER: Player " << result->get_winner() << "\n";
			return (result);
		}

		std::cout << "End of loop, elapsed time: " << elapsed_time << '\n';
		std::cout << "created gamestates: " << g_nb << ", moves: " << g_moves << ", applied moves: " << g_applied_moves << '\n';
		depth++;
	}
	std::cout << "End of turn, elapsed time: " << elapsed_time << '\n';
	std::cout << "created gamestates: " << g_nb << ", moves: " << g_moves << ", applied moves: " << g_applied_moves << '\n';
	items[0] += g_nb;
	items[1] += g_moves;
	turns++;
	std::cout << "Averages: " << items[0] / turns << ", " << items[1] / turns << '\n';
	return (result);
}

