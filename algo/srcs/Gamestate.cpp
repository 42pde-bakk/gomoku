//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"

void Gamestate::print_board(std::ostream& o) const {
	for (int i = 0; i < BOARDSIZE; i++) {
		o << this->board[i];
		o << ((i + 1) % BOARDLENGTH == 0 ? '\n' : ' ');
	}
}

int Gamestate::get_player() const {
	return (this->turn % 2 + 1);
}
