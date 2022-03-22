//
// Created by Peer De bakker on 3/22/22.
//

#include "Move.hpp"

Move::Move(int y, int x, int player) : y(y), x(x), player(player) {
}

std::ostream&	operator<<(std::ostream& o, const Move& m) {
	o << "Move (p" << m.player << "), y: " << m.y << ", x: " << m.x << "\n";
	return (o);
}

