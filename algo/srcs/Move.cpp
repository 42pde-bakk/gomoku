//
// Created by Peer De bakker on 3/22/22.
//

#include "Move.hpp"
#include "Gamestate.hpp"

Move::Move(int idx, int player, bool isTactical) : move_idx(idx), player(player), tacticalMove(isTactical) {
}

std::ostream&	operator<<(std::ostream& o, const Move& m) {
	const int y = m.move_idx / REALBOARDWIDTH; // -1 to account for the seperating bit
	const int x = m.move_idx % REALBOARDWIDTH;
	o << "Move (p" << m.player << "), y: " << y << ", x: " << x << "\n";
	return (o);
}
