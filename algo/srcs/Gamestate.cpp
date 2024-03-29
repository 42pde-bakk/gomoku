//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include <cassert>
#include <algorithm>
#include <sstream>
#include <chrono>

unsigned int g_nb = 0;
unsigned int g_moves = 0;
unsigned int g_applied_moves = 0;
unsigned int g_max_children = 0;
bool	g_uses_lookuptable = false;

Gamestate::Gamestate() : Heuristic(), lastmove(), parent(nullptr), children() {
}

Gamestate::Gamestate(const Gamestate &x) :
		Heuristic(x), lastmove(),
		parent(&x), children() {
	++g_nb;
}

Gamestate::~Gamestate() {
	for (auto &child: this->children) {
		delete child;
		child = nullptr;
	}
	this->children.clear();
}

bool compareGamestates(const Gamestate *a, const Gamestate *b) { return (*a < *b); }

bool compareGamestatesReverse(const Gamestate *a, const Gamestate *b) { return (*b < *a); }

// https://core.ac.uk/download/pdf/33500946.pdf
void Gamestate::generate_children(unsigned int depth_level) {
	if (!this->children.empty() || this->has_winner()) {
		return;
	}
	if (this->board.none()) {
		const uint16_t idx = 20 * 9 + 9;
		auto *middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return;
	}
	Bitboard empty_neighbours(this->get_empty_neighbours());
	if (empty_neighbours.none()) {
		fprintf(stderr, "Error. No more empty tiles\n");
		assert(!empty_neighbours.none());
	}

	Gamestate *child;
	this->children.reserve(40);
	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		if (!empty_neighbours.bitboard_get(i) || Bitboard::isSeperatingBitIndex(i))
			continue;
		child = new Gamestate(*this);
		if (child->place_stone(i) == false) {
			// invalid move
			delete child;
		} else {
			this->children.emplace_back(child);
		}
	}
	this->sort_children();
	// Only prune the last ones in case the option --max_children was given
	// And when we already tried the first two depth levels at full strength
	if (g_max_children > 0 && depth_level > 2) {
//		size_t children_size = this->children.size();
		if (this->children.size() > g_max_children) {
			while (this->children.size() > g_max_children) {
				delete this->children.back();
				this->children.pop_back();
			}
		}
	}
}

std::vector<Move> Gamestate::generate_moves() const {
	std::vector<Move> next_moves;

	if (this->board.none()) {
		auto idx = 20 * 9 + 9;
		next_moves.emplace_back(idx, this->get_player());
		++g_moves;
		return (next_moves);
	}

	Bitboard empty_neighbours(this->get_empty_neighbours());
	if (empty_neighbours.none()) {
		fprintf(stderr, "Error. no more empty tiles\n");
		exit(1);
	}

	for (unsigned int idx = 0; idx < REALBOARDSIZE; idx++) {
		if (!empty_neighbours.bitboard_get(idx) || Bitboard::isSeperatingBitIndex(idx))
			continue;
		next_moves.emplace_back(idx, this->player);
		++g_moves;
	}
	return (next_moves);
}

void Gamestate::apply_move(const Move &mv) {
	++g_applied_moves;
	this->set(mv.move_idx, this->player);
	if (!this->perform_captures(mv.move_idx)) {
		// check double threes
		// It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair.
		if (this->created_open_threes >= 2) {
			this->set_winner(!this->player);
		}
	}
	this->change_player();
	this->lastmove = std::move(mv);
}

bool Gamestate::place_stone(unsigned int move_idx) {
	assert(move_idx < BOARDSIZE);
	assert(this->tile_is_empty(move_idx));

	this->set(move_idx, this->get_player());
	this->lastmove.move_idx = move_idx;
	this->lastmove.player = player;
	const unsigned int captures_happened = this->perform_captures(move_idx);

	if (this->has_winner()) {
		// no need to calculate heuristic value if we already got a winner by captures!
		return (true);
	}
	this->calcH(move_idx);
	if (!captures_happened) {
		// check double threes
		// "It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair."
		const int created_open_threes =
				parent	? this->values[player][OPEN_THREE] - this->parent->values[player][OPEN_THREE]
						: this->values[player][OPEN_THREE];
		if (created_open_threes >= 2) {
			this->set_winner(!this->player);
			return (false);
		}
	}
	this->change_player();
	return (true);
}

int Gamestate::change_player() {
	this->player = !this->player;
	return (this->player);
}

const Move &Gamestate::get_first_move(const Gamestate *root) const {
	if (this->parent && parent != root)
		return (this->parent->get_first_move(root));
	return (this->lastmove);
}

void Gamestate::clear_children() {
	for (auto child: this->children) {
		delete child;
	}
	this->children.clear();
}

void Gamestate::print_history(std::ostream &o, bool colours) const {
	if (this->parent)
		this->parent->print_history(o, colours);
	this->print_board(o, colours);
	this->print_heuristic(o);
	o << '\n';
}

void Gamestate::add_child(Gamestate *child) {
	this->children.push_back(child);
}

Gamestate *Gamestate::calcH(const unsigned int new_stone_idx) {
	if (g_uses_lookuptable)
		this->set_h_with_lookup(new_stone_idx);
	else
		this->set_h(new_stone_idx);
	this->add_h_for_captures();
	return (this);
}

const Gamestate *Gamestate::get_parent() {
	return (this->parent);
}

bool Gamestate::has_children() const {
	return (!this->children.empty());
}

void Gamestate::sort_children() {
	static compareFunc compareFuncs[] = {
			compareGamestates, compareGamestatesReverse
	};
	assert(!children.empty());
	std::sort(children.begin(), children.end(), compareFuncs[this->get_player()]);
}

Move Gamestate::get_move() const {
	return (this->lastmove);
}

void Gamestate::reset() {
	this->board.reset();
	this->values[0].fill(0);
	this->values[1].fill(0);
	this->h = 0;
	this->captures.fill(0);
	this->winner = 0;
	this->player = 0;
	this->clear_children();
}
