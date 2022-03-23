//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_MOVE_HPP
#define CLUSTER_MOVE_HPP

#include "Gomoku.hpp"
#include <iostream>

class Move {
public:
	int		y,
			x,
			player;

	// Allows the operator<< to access the private vars
	friend std::ostream&	operator<<(std::ostream& o, const Move& m);
	Move(int y, int x, int player);
};


#endif //CLUSTER_MOVE_HPP
