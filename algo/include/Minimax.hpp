//
// Created by Peer De bakker on 3/22/22.
//

#ifndef ALGO_MINIMAX_HPP
#define ALGO_MINIMAX_HPP

#include "Gomoku.hpp"
#include <Gamestate.hpp>
#include <chrono>
#include <exception>

#define MAX_THINK_TIME 490

Gamestate	*minimax(Gamestate *state, int depth, bool maximizing_player);
Gamestate	*alphabeta(Gamestate *state, int depth, bool maximizing_player);
Gamestate	*iterative_deepening(Gamestate *gs, int player);


extern std::chrono::time_point<std::chrono::steady_clock> start_time;
extern std::chrono::time_point<std::chrono::steady_clock> current_time;

#endif //ALGO_MINIMAX_HPP
