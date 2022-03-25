//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Directions.hpp"
#include <unordered_map>

Gamestate &Gamestate::operator=(const Gamestate& x) {
	for (auto child : this->children)
		delete child;
	if (this != &x) {
		this->boards = x.boards;
		this->boards[0] = x.boards[0];
		this->boards[1] = x.boards[1];
		this->captures = x.captures;
		this->moves = x.moves;
		this->parent = x.parent;
		this->children = x.children;
		this->turn = x.turn;
		this->h = x.h;
		this->winner = x.winner;
		this->player = x.player;
	}
	return (*this);
}

Gamestate::Gamestate(const Gamestate &x) :
	boards(x.boards), captures(x.captures), moves(x.moves),
	parent(&x), children(),
	turn(x.turn), h(x.h), winner(x.winner), player(x.player) {
}

Gamestate::~Gamestate() {
	for (auto child : this->children)
		delete child;
	this->children.clear();
}

void Gamestate::print_board(std::ostream& o) const {
	static char legend[] = "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 S\n";
	o << _YELLOW << legend << _END;
	for (int i = 0; i < BOARDSIZE; i++) {
		if (this->boards[0][i])
			o << _BLUE _BOLD "1" _END;
		else if (this->boards[1][i])
			o << _RED _BOLD "2" _END;
		else
			o << 0;
		o << ((i + 1) % BOARDWIDTH == 0 ? '\n' : ' ');
	}
	o << _YELLOW << legend << "\n" _END;
}

void print_bitboard(bitboard &b, std::ostream &o) {
	static char legend[] = "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 S\n";
	o << _YELLOW << legend << _END;
	for (int i = 0; i < BOARDSIZE; i++) {
		o << b[i];
		o << ((i + 1) % BOARDWIDTH == 0 ? '\n' : ' ');
	}
	o << _YELLOW << legend << "\n" _END;
}


int Gamestate::get_player() const {
	return (this->player);
}

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children() {
	bitboard	all_stones = boards[0] | boards[1];
	bitboard	empty_cells = ~all_stones;
	if (all_stones.none()) {
		int idx = 9 * 9;
		auto	*middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return ;
	}
	bitboard	neighbours = all_stones | SHIFT_N(all_stones) | SHIFT_W(all_stones) | SHIFT_S(all_stones) | SHIFT_E(all_stones) \
										| SHIFT_NE(all_stones) | SHIFT_NW(all_stones) | SHIFT_SE(all_stones) | SHIFT_SW(all_stones);
	bitboard	empty_neighbours = neighbours & empty_cells;
//	std::cerr << "Empty neighbours:" << '\n';
//	print_bitboard(empty_neighbours, std::cerr);
	if (empty_neighbours.none())
		throw std::runtime_error("Error. No more empty tiles");
	for (int i = 0; i < BOARDSIZE; i++) {
		if (!empty_neighbours[i] || i % (BOARDWIDTH - 1) == 0)
			continue;
		auto	*child = new Gamestate(*this);
		child->place_stone(i);
		this->children.emplace_back(child);
	}
}

Gamestate::Gamestate() { }

void Gamestate::place_stone(int move_idx) {
	assert(move_idx >= 0 && move_idx < BOARDSIZE);
	assert(this->boards[0][move_idx] == false);
	assert(this->boards[1][move_idx] == false);
	this->boards[player][move_idx] = true;
	this->moves.emplace_back(move_idx, player);
	this->perform_captures(move_idx);
	// TODO: update heuristic value
	this->set_heuristic();
	this->turn++;
	this->change_player();
}

int collect_open_things(const std::array<bitboard, 2>& boards, int idx, int player_id, std::unordered_map<int, unsigned int>& checked) {
	static const std::array<int, 4> dirs = setup_dirs();
	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
	int h = 0;

	for (unsigned int d = 0; d < 4; d++) {
		if (checked[idx] >> d) // Have I already checked this tile in this direction?
			continue;
		int i = idx;
		int len = 1;
		while (i < BOARDSIZE && (i % 19 != 0) && boards[player_id][i]) {
			++len;
			i += dirs[0];
			checked[i] <<= d; // Save that I already checked this tile in this direction
		}
		if (len > 1) {
			h += values[len];

			// Add extra value if the tiles surrounding this streak are empty
			if (i < BOARDSIZE && (i % 19 == 0))
				h += values[len] / 10; // opportunity value
			int prev = i - dirs[0];
			if (prev >= 0 && (i % 19 == 0))
				h += values[len] / 10; // opportunity value
		}
	}
	return (h);
}

void Gamestate::set_heuristic() {
	int p0_h = this->get_h_value_player(0);
	int p1_h = this->get_h_value_player(1);
	this->h = p1_h - p0_h;
}

int Gamestate::get_h_value_player(int player_id) const {
	std::unordered_map<int, unsigned int> checked;
	int _h = 0;
	for (int i = 0; i < BOARDSIZE; i++) {
		if (this->boards[player_id][i])
			_h += collect_open_things(this->boards, i, player_id, checked);
	}
	return (_h);
}

int Gamestate::change_player() {
	this->player = !this->player;
	return (this->player);
}
