//
// Created by Peer De bakker on 3/24/22.
//

#ifndef GOMOKUBOT_DIRECTIONS_HPP
#define GOMOKUBOT_DIRECTIONS_HPP

#include <array>

#define SHIFT_N(arg) (arg << BOARDWIDTH)
#define SHIFT_S(arg) (arg >> BOARDWIDTH)
#define SHIFT_W(arg) (arg << 1)
#define SHIFT_E(arg) (arg >> 1)

#define SHIFT_NE(arg) (arg << (BOARDWIDTH - 1))
#define SHIFT_SE(arg) (arg >> (BOARDWIDTH + 1))
#define SHIFT_NW(arg) (arg << (BOARDWIDTH + 1))
#define SHIFT_SW(arg) (arg >> (BOARDWIDTH - 1))

std::array<int, 8>	all_dirs();
std::array<int, 4>	setup_dirs();
std::array<int, 4>	setup_dirs_opposite();

#endif //GOMOKUBOT_DIRECTIONS_HPP
