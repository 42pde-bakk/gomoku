//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Directions.hpp"
#include <unordered_map>
#include <cassert>
#include "Utils.hpp"

bool g_log = false;
std::unordered_map<std::bitset<BOARDSIZE>, int> Gamestate::tt;

Gamestate &Gamestate::operator=(const Gamestate& x) {
	for (auto child : this->children)
		delete child;
	if (this != &x) {
		this->board = x.board;
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
	Bitboard(x), captures(x.captures), moves(x.moves),
	parent(&x), children(),
	turn(x.turn), h(x.h), winner(x.winner), player(x.player) {
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
	if (this->board.none()) {
		int idx = 20 * 9 + 9;
		auto	*middle = new Gamestate(*this);
		middle->place_stone(idx);
		this->children.emplace_back(middle);
		return ;
	}
	Bitboard	empty_neighbours = this->get_empty_neighbours();
	if (empty_neighbours.none())
		throw std::runtime_error("Error. No more empty tiles");
	for (int i = 0; i < REALBOARDSIZE; i++) {
		if (!empty_neighbours.bitboard_get(i) || Bitboard::isSeperatingBitIndex(i))
			continue;
		auto	*child = new Gamestate(*this);
		child->place_stone(i);
		this->children.emplace_back(child);
	}
	std::sort(children.begin(), children.end(), compareFuncs[this->get_player()]);
}

Gamestate::Gamestate() { }
void Gamestate::place_stone(int move_idx) {
	assert(move_idx >= 0 && move_idx < BOARDSIZE);
	assert (this->tile_is_empty(move_idx));
	this->set(move_idx, this->get_player());
	this->moves.emplace_back(move_idx, player);
	if (!this->perform_captures(move_idx)) {
		// check double threes
		// It is important to note that it is not forbidden to introduce
		// a double-three by capturing a pair.
	}
	// TODO: update heuristic value
	this->update_heuristic(move_idx);
	this->turn++;
	this->change_player();
}

//int Gamestate::collect_open_things(unsigned int start_idx, unsigned int player_id, std::unordered_map<int, unsigned int>& checked) const {
//	static const std::array<int, 4> dirs = setup_dirs();
//	(void)player_id;
//	unsigned int player_value = player_id + 1;
////	unsigned int opponent_value = !player_id + 1;
//	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
//	int _h = 0;
//
//	for (unsigned int d = 0; d < 4; d++) {
//		if (checked[start_idx] & (1u << (d + 1)))
//			continue;
//		int len = 1;
//		unsigned int i = start_idx + dirs[d];
//		unsigned int res = this->bitboard_get(i);
//		while (i < BOARDSIZE && !isSeperatingBitIndex(i) && res == player_value) { // && boards[player_id][i]) {
//			++len;
//			checked[i] |= 1UL << (d + 1); // Save that I already checked this tile in this direction
//			i += dirs[d];
//		}
//		if (len > 1) {
//			int open_sides = 0;
//
//			// Add extra value if the tiles surrounding this streak are empty
//			if (i < BOARDSIZE && !isSeperatingBitIndex(i) && tile_is_empty(i)) {
//				open_sides += 1; // opportunity value
//			}
//			int prev = start_idx - dirs[d];
//			if (prev >= 0 && !isSeperatingBitIndex(prev) && tile_is_empty(prev)) {
//				open_sides += 1; // opportunity value
//			}
//			_h += open_sides * values[len] / 2;
//		}
//	}
//	return (_h);
//}

void Gamestate::update_heuristic(unsigned int move_idx) {
	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
	static const std::array<int, 8>	all_dirs = setup_all_dirs();

	const unsigned int player_id = this->get_player();
	const unsigned int player_stone = player_id + 1;
	const unsigned int opp_stone = !player_id + 1;
	std::array<unsigned int, 8>		lines{};
	int h_diff = 0;

	(void)opp_stone;

	for (int i = 0; i < 8; i++) {
		int dir = all_dirs[i];
		unsigned int neighbour_idx = move_idx + dir;
		while (neighbour_idx < BOARDSIZE && this->bitboard_get(neighbour_idx) == player_stone) {
			lines[i] += 1;
			neighbour_idx += dir;
		}
		h_diff -= values[lines[i]];
	}
	for (int i = 0; i < 4; i++) {
		h_diff += (values[lines[i] + lines[i + 4]]);
	}
	this->h += (-1 * !player_id) * h_diff;
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
