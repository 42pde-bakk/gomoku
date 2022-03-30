//
// Created by Peer De bakker on 3/29/22.
//

#include <iostream>
#include "Bitboard.hpp"
#include "Colours.hpp"
#include "Directions.hpp"

Bitboard::Bitboard() : board() {
}

Bitboard::Bitboard(const bitboard &b) : board(b) {
}

Bitboard::~Bitboard() = default;

Bitboard::Bitboard(const Bitboard &x) : board(x.board) {
}

Bitboard &Bitboard::operator=(const Bitboard &x) {
	if (this != &x)
		this->board = x.board;
	return (*this);
}

bool Bitboard::operator==(const Bitboard &rhs) const {
	return (this->board == rhs.board);
}

bool Bitboard::operator!=(const Bitboard &rhs) const{
	return !(*this == rhs);
}

void	print_legend(std::ostream& o, bool print_colours) {
	const static char legend[] = "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 S\n";
	if (print_colours)
		o << _YELLOW;
	o << legend;
	if (print_colours)
		o << _END;
}

static void	print_item(std::ostream& o, bool print_colours, unsigned int item) {
	const static char *colours[] = {
			_BLUE _BOLD,
			_RED _BOLD
	};
	if (print_colours)
		o << colours[item - 1];
	o << item;
	if (print_colours)
		o << _END;
}

std::ostream &operator<<(std::ostream &o, const Bitboard &b) {
	for (int i = 0; i < REALBOARDSIZE; i++) {
		print_item(o, false, b.bitboard_get(i));
		o << ((i + 1) % BOARDWIDTH == 0 ? '\n' : ' ');
	}
	return (o);
}

void	Bitboard::print_board(std::ostream& o, bool colours) const {
	for (int i = 0; i < REALBOARDSIZE; i++) {
		print_item(o, colours, bitboard_get(i));
		o << ((i + 1) % BOARDWIDTH == 0 ? '\n' : ' ');
	}
}

unsigned int Bitboard::bitboard_get(unsigned int idx) const {
	const unsigned int real_idx = idx * 2;
	unsigned int ret = this->board[real_idx];

	ret <<= 1u;
	ret |= this->board[real_idx + 1];

	return (ret);
}

bool Bitboard::tile_is_empty(unsigned int idx) const {
	return (this->bitboard_get(idx) == 0);
}

unsigned int Bitboard::at(size_t n) const {
	if (n >= BOARDSIZE)
		throw std::out_of_range("Bitboard::at() => out of range");
	return (this->bitboard_get(n));
}

unsigned int Bitboard::operator[](size_t n) const {
	return (this->bitboard_get(n));
}

void Bitboard::set(unsigned int idx, unsigned int player) {
	assert(idx <= BOARDSIZE / 2);
	assert(player < 2);
	const unsigned int real_idx = idx * 2;
	if (player) {
		this->board[real_idx] = true;
		this->board[real_idx + 1] = false; // probably not needed in production since you can't place a stone on another stone
		std::cerr << "Player2 set bitboard[" << real_idx << "] to true\n";
	} else {
		this->board[real_idx] = false;
		this->board[real_idx + 1] = true;
		std::cerr << "Player 1 set bitboard[" << real_idx+1 << "] to true\n";
	}
}

bitboard	Bitboard::get_empty_neighbours() const {
	bitboard	empty_cells = ~this->board;
	bitboard	neighbours = SHIFT_N(board) | SHIFT_W(board) | SHIFT_S(board) | SHIFT_E(board) \
									| SHIFT_NE(board) | SHIFT_NW(board) | SHIFT_SE(board) | SHIFT_SW(board);
	bitboard	empty_neighbours = neighbours & empty_cells;
	return (empty_neighbours);
}

bool Bitboard::is_seperating_bit(unsigned int idx) {
	return ((idx + 1) % BOARDWIDTH == 0);
}
