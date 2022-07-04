//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_MOVE_HPP
#define GOMOKUBOT_MOVE_HPP

#include "Gomoku.hpp"
#include <iostream>

class Move {
public:
	int	move_idx,
		player;
	bool tacticalMove;

	Move() = default;
	Move(int idx, int player, bool isTactical = false);

	// Allows the operator<< to access the private vars
	friend std::ostream&	operator<<(std::ostream& o, const Move& m);
};


#endif //GOMOKUBOT_MOVE_HPP
