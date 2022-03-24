//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Colours.hpp"

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
	}
	return (*this);
}

Gamestate::Gamestate(const Gamestate &x) :
	boards(x.boards), captures(x.captures), moves(x.moves),
	parent(&x), children(),
	turn(x.turn), h(x.h), winner(x.winner) {
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
	return (this->turn % 2);
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

#include <random>
int	get_random_h() {
	static std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(-5000, 5000); // guaranteed unbiased
	int random_integer = uni(rng);
	return (random_integer);
}

void Gamestate::place_stone(int move_idx) {
	int player = this->turn % 2;
//	std::cerr << "move_idx = " << move_idx << '\n';
	assert(this->boards[0][move_idx] == false);
	assert(this->boards[1][move_idx] == false);
	this->boards[player][move_idx] = true;
	this->moves.emplace_back(move_idx, player);
	// TODO: update heuristic value
	this->h = get_random_h();
	this->turn++;
}
