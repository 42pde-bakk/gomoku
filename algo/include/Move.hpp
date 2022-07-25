//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_MOVE_HPP
#define GOMOKUBOT_MOVE_HPP

#include "Gomoku.hpp"
#include <iostream>
//#include <cstdint>

class Move {
public:
	uint16_t move_idx;
	uint8_t player;

	Move() = default;

	Move(uint16_t idx, uint8_t player);

	// Allows the operator<< to access the private vars
	friend std::ostream &operator<<(std::ostream &o, const Move &m);
};


#endif //GOMOKUBOT_MOVE_HPP
