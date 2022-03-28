//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Directions.hpp"
#include <unordered_map>
#include <cassert>

std::unordered_map<std::bitset<BOARDSIZE * 2>, int> Gamestate::tt;

bool is_seperating_bit(const int idx) {
	return ((idx + 1) % 20 == 0);
}

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

static void	print_legend(std::ostream& o, bool print_colours) {
	static char legend[] = "0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 S\n";
	if (print_colours)
		o << _YELLOW;
	o << legend;
	if (print_colours)
		o << _END;
}

void Gamestate::print_board(std::ostream& o, bool print_colours) const {
	print_legend(o, print_colours);
	for (int i = 0; i < BOARDSIZE; i++) {
		if (this->boards[0][i])
			o << (print_colours ? _BLUE _BOLD "1" _END : "1");
		else if (this->boards[1][i])
			o << (print_colours ? _RED _BOLD "2" _END : "2");
		else
			o << 0;
		o << ((i + 1) % BOARDWIDTH == 0 ? '\n' : ' ');
	}
	print_legend(o, print_colours);
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

bool compareGamestates(Gamestate* a, Gamestate* b) { return (*a < *b); }

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children() {
	bitboard	all_stones = boards[0] | boards[1];
	bitboard	empty_cells = ~all_stones;
	if (all_stones.none()) {
		int idx = 20 * 9 + 9;
		auto	*middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return ;
	}
	bitboard	neighbours = all_stones | SHIFT_N(all_stones) | SHIFT_W(all_stones) | SHIFT_S(all_stones) | SHIFT_E(all_stones) \
										| SHIFT_NE(all_stones) | SHIFT_NW(all_stones) | SHIFT_SE(all_stones) | SHIFT_SW(all_stones);
	bitboard	empty_neighbours = neighbours & empty_cells;
	if (empty_neighbours.none())
		throw std::runtime_error("Error. No more empty tiles");
	for (int i = 0; i < BOARDSIZE; i++) {
		if (!empty_neighbours[i] || i % (BOARDWIDTH - 1) == 0)
			continue;
		auto	*child = new Gamestate(*this);
		child->place_stone(i);
		this->children.emplace_back(child);
		std::sort(children.begin(), children.end(), compareGamestates);
	}
}

Gamestate::Gamestate() { }

void Gamestate::place_stone(int move_idx) {
	assert(move_idx >= 0 && move_idx < BOARDSIZE);
	assert(this->boards[0][move_idx] == false);
	assert(this->boards[1][move_idx] == false);
	this->boards[player][move_idx] = true;
	this->moves.emplace_back(move_idx, player);
	if (!this->perform_captures(move_idx)) {
		// check double threes
		// It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair.
	}
	// TODO: update heuristic value
	this->set_heuristic();
	this->turn++;
	this->change_player();
}

int Gamestate::collect_open_things(int idx, int player_id, std::unordered_map<int, unsigned int>& checked) const {
	static const std::array<int, 4> dirs = setup_dirs();
	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
	int _h = 0;

	for (unsigned int d = 0; d < 4; d++) {
		if (checked[idx] & (1UL << (d + 1)))
			continue;
		int len = 1;
		int i = idx + dirs[d];
		while (i < BOARDSIZE && !is_seperating_bit(i) && boards[player_id][i]) {
			++len;
			checked[i] |= 1UL << (d + 1); // Save that I already checked this tile in this direction
			i += dirs[d];
		}
		if (len > 1) {
			int open_sides = 0;

			// Add extra value if the tiles surrounding this streak are empty
			if (i < BOARDSIZE && !is_seperating_bit(i) && tile_is_empty(i)) {
				open_sides += 1; // opportunity value
			}
			int prev = idx - dirs[d];
			if (prev >= 0 && !is_seperating_bit(prev) && tile_is_empty(prev)) {
				open_sides += 1; // opportunity value
			}
			_h += open_sides * values[len] / 2;
		}
	}
	return (_h);
}

template <size_t N1, size_t N2 >
std::bitset<N1 + N2> concat( const std::bitset<N1>& b1, const std::bitset<N2>& b2 ) {
	std::bitset<N1 + N2> res;
	for (size_t i = 0; i < N1; ++i) {
		res[i] = b1[i];
		res[N1 + i] = b2[i];
	}
	return res;
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
		if (this->boards[player_id][i]) {
			_h += collect_open_things(i, player_id, checked);
		}
	}
	return (_h);
}

int Gamestate::change_player() {
	this->player = !this->player;
	return (this->player);
}

const Move &Gamestate::get_first_move() const {
	return this->moves.front();
}

int Gamestate::get_heuristic() const {
	return (this->h);
}

bool Gamestate::tile_is_empty(int idx) const {
	return (!this->boards[0][idx] && !this->boards[1][idx]);
}
