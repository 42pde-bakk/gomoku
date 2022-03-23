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

static void print_bitboard(bitboard &b, std::ostream &o) {
	for (int i = 0; i < BOARDSIZE; i++) {
		o << b[i];
		o << ((i + 1) % BOARDLENGTH == 0 ? '\n' : ' ');
	}
}


int Gamestate::get_player() const {
	return (this->turn % 2);
}

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children() {
	bitboard	all_stones = boards[0] | boards[1];
	bitboard	empty_cells = ~all_stones;
	bitboard	neighbours = all_stones | SHIFT_N(all_stones) | SHIFT_W(all_stones) | SHIFT_S(all_stones) | SHIFT_E(all_stones) \
			| SHIFT_NE(all_stones) | SHIFT_NW(all_stones) | SHIFT_SE(all_stones) | SHIFT_SW(all_stones);
	bitboard	empty_neighbours = neighbours & empty_cells;
	std::cerr << "Empty neighbours:\n";
	print_bitboard(empty_neighbours, std::cerr);

}

Gamestate::Gamestate() {

}
