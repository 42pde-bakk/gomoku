//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"

void Gamestate::print_board(std::ostream& o) const {
	for (int i = 0; i < BOARDSIZE; i++) {
		if (this->boards[0][i])
			o << 1;
		else if (this->boards[1][i])
			o << 2;
		else
			o << 0;
		o << ((i + 1) % BOARDLENGTH == 0 ? '\n' : ' ');
	}
}

int Gamestate::get_player() const {
	return (this->turn % 2 + 1);
}

void Gamestate::generate_children() {
	// TODO
}

Gamestate::Gamestate() {

}
