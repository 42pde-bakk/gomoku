//
// Created by Peer De bakker on 3/22/22.
//

#ifndef ALGO_MINIMAX_HPP
#define ALGO_MINIMAX_HPP

#include "Gomoku.hpp"
#include <Gamestate.hpp>

Gamestate * minimax(Gamestate *state, int depth, bool maximizing_player);
Gamestate	alphabeta(Gamestate state, int depth, int α, int β, bool maximizing_player);

Move		get_random_move(Gamestate *state);

#endif //ALGO_MINIMAX_HPP
