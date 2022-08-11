//
// Created by Peer De bakker on 3/22/22.
//

#include "Move.hpp"
#include "Gamestate.hpp"

Move::Move(uint16_t idx, uint8_t player) : move_idx(idx), player(player) {
}

std::ostream &operator<<(std::ostream &o, const Move &m) {
	const unsigned int y = m.move_idx / REALBOARDWIDTH; // -1 to account for the seperating bit
	const unsigned int x = m.move_idx % REALBOARDWIDTH;
	o << "Move (p" << (unsigned int)m.player << "), y: " << y << ", x: " << x << "\n";
	return (o);
}
