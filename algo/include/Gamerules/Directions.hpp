//
// Created by Peer De bakker on 3/24/22.
//

#ifndef GOMOKUBOT_DIRECTIONS_HPP
#define GOMOKUBOT_DIRECTIONS_HPP

#include <array>

#define SHIFT_N(arg) (arg << BOARDWIDTH)
#define SHIFT_S(arg) (arg >> BOARDWIDTH)
#define SHIFT_W(arg) (arg << 2)
#define SHIFT_E(arg) (arg >> 2)

#define SHIFT_NE(arg) (arg << (BOARDWIDTH - 2))
#define SHIFT_SE(arg) (arg >> (BOARDWIDTH + 2))
#define SHIFT_NW(arg) (arg << (BOARDWIDTH + 2))
#define SHIFT_SW(arg) (arg >> (BOARDWIDTH - 2))

//std::array<int, 8>	all_dirs();
std::array<int, 4> setup_dirs();

std::array<int, 4>	setup_dirs_opposite();
std::array<int, 8> setup_all_dirs();

std::array<int, 8> setup_all_dirs_singular();

#endif //GOMOKUBOT_DIRECTIONS_HPP
