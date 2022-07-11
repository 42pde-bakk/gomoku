//
// Created by Peer De bakker on 3/22/22.
//

#ifndef ALGO_MINIMAX_HPP
#define ALGO_MINIMAX_HPP

#include "Gomoku.hpp"
#include <Gamestate.hpp>
#include <chrono>
#include <exception>

#ifndef TIMEOUT_VALUE
# define TIMEOUT_VALUE 490
#endif

Gamestate	*minimax(Gamestate *state, int depth, bool maximizing_player);
Gamestate	*minimax_alphabeta_start(Gamestate *state, int depth, bool maximizing_player);
Gamestate	*iterative_deepening(Gamestate *gs, int player);


extern std::chrono::time_point<std::chrono::steady_clock> start_time;
extern std::chrono::time_point<std::chrono::steady_clock> current_time;
extern long long int elapsed_time;

#endif //ALGO_MINIMAX_HPP
