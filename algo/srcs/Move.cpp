//
// Created by Peer De bakker on 3/22/22.
//

#include "Move.hpp"
#include "Gamestate.hpp"

Move::Move(int idx, int player) : move_idx(idx), player(player) {
}

std::ostream&	operator<<(std::ostream& o, const Move& m) {
	o << "Move (p" << m.player << "), y: " << m.move_idx / BOARDHEIGHT << ", x: " << m.move_idx % BOARDHEIGHT << "\n";
	return (o);
}

