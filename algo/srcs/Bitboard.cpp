//
// Created by Peer De bakker on 3/29/22.
//

#include <iostream>
#include "Bitboard.hpp"
#include "Colours.hpp"
#include "Directions.hpp"
#include <cassert>

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
			_WHITE _BOLD,
			_BLUE _BOLD,
			_RED _BOLD,
			_GREEN _BOLD // 3 is used only when bitshifting to discover neighbours
	};
	if (item == std::numeric_limits<unsigned int>::max())
		item = 0;
	if (print_colours)
		o << colours[item];
	o << item;
	if (print_colours)
		o << _END;
}

std::ostream &operator<<(std::ostream &o, const Bitboard &b) {
	print_legend(o, false);
	for (int i = 0; i < REALBOARDSIZE; i++) {
		print_item(o, false, b.bitboard_get(i));
		if (Bitboard::isSeperatingBitIndex(i))
			o << '\n';
		else
			o << ' ';
	}
	print_legend(o, false);
	return (o);
}

void	Bitboard::print_board(std::ostream& o, bool colours) const {
	print_legend(o, colours);
	for (int i = 0; i < REALBOARDSIZE; i++) {
		print_item(o, colours, bitboard_get(i));
		if (Bitboard::isSeperatingBitIndex(i))
			o << '\n';
		else
			o << ' ';
	}
	print_legend(o, colours);
}

unsigned int Bitboard::bitboard_get(unsigned int idx) const {
	if (!isvalid_tile(idx))
		return (-1);
//	if (idx >= REALBOARDSIZE)
//		return (-1);
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
	assert(!isSeperatingBitIndex(idx));
	const unsigned int real_idx = idx * 2;
	this->board[real_idx] = player;
	this->board[real_idx + 1] = !player;
}

void Bitboard::clear_tile(unsigned int idx) {
	const unsigned int real_idx = idx * 2;
	this->board[real_idx] = false;
	this->board[real_idx + 1] = false;
}


bitboard	Bitboard::get_empty_neighbours() const {
	Bitboard	empty_cells(~this->board);
	for (unsigned int n = 0; n < REALBOARDSIZE; n++) {
		if (!this->tile_is_empty(n))
			empty_cells.clear_tile(n);
	}
	bitboard	neighbours = SHIFT_N(board) | SHIFT_W(board) | SHIFT_S(board) | SHIFT_E(board) \
							| SHIFT_NE(board) | SHIFT_NW(board) | SHIFT_SE(board) | SHIFT_SW(board);
	return (neighbours & empty_cells.board);
}

bool Bitboard::isSeperatingBitIndex(unsigned int idx) {
	return ((idx + 1) % REALBOARDWIDTH == 0);
}

bool Bitboard::none() const {
	return (this->board.none());
}

const bitboard &Bitboard::get() const {
	return (this->board);
}

unsigned int Bitboard::count() const {
	unsigned int res = 0;
	unsigned int tmp;
	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		tmp = this->bitboard_get(i);
		res += (tmp > 0 && tmp <= 3);
	}
	return (res);
}

bool Bitboard::isvalid_tile(unsigned int idx) {
	return (idx < REALBOARDSIZE && !isSeperatingBitIndex(idx));
}
