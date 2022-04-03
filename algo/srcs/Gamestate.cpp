//
// Created by Peer De bakker on 3/22/22.
//

#include "Gamestate.hpp"
#include "Colours.hpp"
#include "Directions.hpp"
#include <unordered_map>
#include <cassert>
#include <algorithm>
#include "Utils.hpp"

bool g_log = false;
std::unordered_map<std::bitset<BOARDSIZE>, int> Gamestate::tt;
std::hash<bitboard> hash_fn;

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
	ss << "tests/log/gamestate_" << idx++;
	fs.open(ss.str(), std::fstream::out | std::fstream::trunc);
	if (!fs.is_open())
		exit(1);
	fs << hash_fn(this->board) << '\n';
	fs << "Heuristic value: " << this->h << '\n';
	print_board(fs, false);
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
	this->set_heuristic();
//	this->write_to_file();
	this->turn++;
	this->change_player();
}

unsigned int Gamestate::h_for_tile(unsigned int start_idx, unsigned int stone_p, unsigned int stone_opp, std::unordered_map<int, unsigned int> &checked_tiles) const {
	static const std::array<int, 4> dirs = setup_dirs();
	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
	unsigned int heur = 0;
	if (g_log)
		dprintf(2, "start_idx: %u, stones: p=%u, opp=%u\n", start_idx, stone_p, stone_opp);
	for (unsigned int d = 0; d < dirs.size(); d++) {
		if (g_log)
			dprintf(2, "dirs[%u] = %d\n", d, dirs[d]);
		if (checked_tiles[start_idx] & (1u << d)) {
			if (g_log)
				dprintf(2, "already checked %u in direction %d\n", start_idx, dirs[d]);
			continue ;
		}

		unsigned int length = 1;
		unsigned int i = start_idx + dirs[d];
//		unsigned int stone_value = this->bitboard_get(i);
		while (i < REALBOARDSIZE && !isSeperatingBitIndex(i) && this->bitboard_get(i) == stone_p) {
			length++;
			checked_tiles[i] |= 1u << d;
			if (g_log)
				dprintf(2, "checked_tiles[%u] now is %d, Unioned this into it: (%u)\n", i, checked_tiles[i], 1u << (d + 1));
			i += dirs[d];
		}
		if (g_log)
			dprintf(2, "start_idx=%u, dir=%d, length=%u\n", start_idx, dirs[d], length);
		if (length > 1) {
			unsigned int open_sides = 0;
			unsigned int prev = start_idx - dirs[d];
			if (prev < REALBOARDSIZE && !isSeperatingBitIndex(prev) && tile_is_empty(prev)) {
				if (g_log)
					dprintf(2, "prev (%u) is empty! So extra value!\n", prev);
				open_sides += 1u;
			}
			if (i < REALBOARDSIZE && !isSeperatingBitIndex(i) && tile_is_empty(i)) {
				if (g_log)
					dprintf(2, "next (%u) is empty! So extra value!\n", i);
				open_sides += 1u;
			}
			unsigned int extra_heur;
			if (length >= 5 && open_sides != 0)
				open_sides += 1;
			extra_heur = open_sides * values[length] / 2;
			if (g_log)
				dprintf(2, "adding extra heuristic value of %u\n", extra_heur);
			heur += extra_heur;
		}
	}
	return (heur);
}

void	Gamestate::set_heuristic() {
	auto hash = hash_fn(this->board);
	if (tt.find(hash) != tt.end()) {
		this->h = tt[hash];
		return ;
	}
	std::unordered_map<int, unsigned int> checked_tiles;
	this->h = 0;

	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		unsigned int stone = this->bitboard_get(i);
		if (!stone)
			continue;
		assert(stone == 1 || stone == 2);
		unsigned int opponent_stone = Gamestate::get_opponent_stone(stone);
		if (stone == 1)
			this->h -= (int)h_for_tile(i, stone, opponent_stone, checked_tiles);
		else
			this->h += (int)h_for_tile(i, stone, opponent_stone, checked_tiles);
	}
	if (g_log)
		dprintf(2, "final heuristic value is %d\n", this->h);
	tt[hash] = this->h;
}

//void Gamestate::update_heuristic(unsigned int move_idx) {
//	static const int values[] = {0, 0, 10, 100, 1000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000, 10000};
//	static const std::array<int, 8>	all_dirs = setup_all_dirs();
//
//	const unsigned int player_id = this->get_player();
//	const unsigned int player_stone = player_id + 1;
//	const unsigned int opp_stone = !player_id + 1;
//	std::array<unsigned int, 8>		lines{};
//	int h_diff = 0;
//
//	(void)opp_stone;
//
//	for (int i = 0; i < 8; i++) {
//		int dir = all_dirs[i];
//		unsigned int neighbour_idx = move_idx + dir;
//		while (neighbour_idx < BOARDSIZE && this->bitboard_get(neighbour_idx) == player_stone) {
//			lines[i] += 1;
//			neighbour_idx += dir;
//		}
//		h_diff -= values[lines[i]];
//	}
//	for (int i = 0; i < 4; i++) {
//		h_diff += (values[lines[i] + lines[i + 4]]);
//	}
//	this->h += (-1 * !player_id) * h_diff;
//}


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

unsigned int Gamestate::get_opponent_stone(unsigned int stone) {
	return (stone ^ 3u);
}

void Gamestate::clear_children() {
	for (auto child : this->children) {
		delete child;
	}
	this->children.clear();
}
