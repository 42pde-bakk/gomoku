//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include <cassert>
#include <algorithm>

bool g_log = false;
bool g_log_heur = false;

Gamestate::Gamestate(const Gamestate &x) :
		Heuristic(x), moves(x.moves),
		parent(&x), children(),
		depth(x.depth), player(x.player) {
}

Gamestate::~Gamestate() {
	for (auto child : this->children)
		delete child;
	this->children.clear();
}

int Gamestate::get_player() const {
	return (this->player);
}

bool compareGamestates(const Gamestate* a, const Gamestate* b) { return (*a < *b); }
bool compareGamestatesReverse(const Gamestate* a, const Gamestate* b) { return (*b < *a); }

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children() {
	static compareFunc compareFuncs[] = {
		compareGamestates, compareGamestatesReverse
	};
	if (!this->children.empty() || this->has_winner()) {
		return ;
	}
	if (this->board.none()) {
		int idx = 20 * 9 + 9;
		auto	*middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return ;
	}
	Bitboard	empty_neighbours(this->get_empty_neighbours());
	if (empty_neighbours.none()) {
		throw std::runtime_error("Error. No more empty tiles");
	}
	for (int i = 0; i < REALBOARDSIZE; i++) {
		if (!empty_neighbours.bitboard_get(i) || Bitboard::isSeperatingBitIndex(i))
			continue;
		auto	*child = new Gamestate(*this);
		child->place_stone(i);
		this->children.emplace_back(child);
	}
	std::sort(children.begin(), children.end(), compareFuncs[this->get_player()]);
}

#include <fstream>
#include <sstream>
void	Gamestate::write_to_file() const {
	static int idx = 1;
	std::fstream fs;
	std::stringstream ss;
//	ss << "tests/log/gamestate_" << idx++;
	ss << "/Users/pde-bakk/PycharmProjects/gomoku/algo/tests/log/gamestate_" << idx++;
	fs.open(ss.str(), std::fstream::out | std::fstream::trunc);
	if (!fs.is_open())
		throw std::runtime_error("Couldn't write to logfile");
	fs << Heuristic::hash_fn(this->board) << '\n';
	fs << "Heuristic value: " << this->h << '\n';
	print_board(fs, false);
	print_heuristic(fs);
}

Gamestate::Gamestate() { }

void Gamestate::place_stone(int move_idx) {
	assert(move_idx >= 0 && move_idx < BOARDSIZE);
	assert (this->tile_is_empty(move_idx));
//	static size_t whoo = 1;
//
//	dprintf(1, "whoo = %zu\n", whoo);
//	whoo++;

	this->set(move_idx, this->get_player());
	this->moves.emplace_back(move_idx, player);
	if (!this->perform_captures(move_idx)) {
		// check double threes
		// It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair.
	}
	// TODO: update heuristic value
	this->set_h();
	this->add_h_for_captures();
	if (g_log_heur)
		this->write_to_file();
	this->depth++;
	this->change_player();
}

int Gamestate::change_player() {
	this->player = !this->player;
	return (this->player);
}

const Move &Gamestate::get_first_move() const {
	return this->moves.front();
}

unsigned int Gamestate::get_opponent_stone(unsigned int stone) {
	return (stone ^ 3u);
}

void Gamestate::clear_children() {
	for (auto child : this->children) {
		delete child;
	}
	this->children.clear();
}

void Gamestate::print_history(std::ostream& o) const {
	if (this->parent)
		this->parent->print_history(o);
	this->print_board(o, true);
	o << '\n';
}
