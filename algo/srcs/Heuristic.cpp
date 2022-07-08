//
// Created by Peer De bakker on 4/4/22.
//

#include "Heuristic.hpp"
#include "Directions.hpp"
#include <iostream>
#include <cassert>

std::hash<bitboard> Heuristic::hash_fn;
std::unordered_map<std::bitset<BOARDSIZE>, int> Heuristic::tt;
//std::unordered_map<int, unsigned int> g_checkedTiles;

int Heuristic::get_h() const {
	return (this->h);
}

int Heuristic::get_player() const {
	return (this->player);
}

unsigned int Heuristic::get_length(unsigned int *i, unsigned int stone_p, unsigned int d,  std::array<unsigned int, REALBOARDSIZE>& g_checkedTiles) const {
	static const std::array<int, 4> dirs = setup_dirs();
	unsigned int length = 1;
	unsigned int& idx = *i;

	assert(i == &idx);

	while (idx < REALBOARDSIZE && !isSeperatingBitIndex(idx) && this->bitboard_get(idx) == stone_p) {
		length++;
		g_checkedTiles[idx] |= 1u << d;
//		if (g_log)
//			dprintf(2, "checked_tiles[%u] now is %d, Unioned this into it: (%u)\n", idx, g_checkedTiles[idx], 1u << (d + 1));
		idx += dirs[d];
	}
//	if (g_log)
//		dprintf(2, "dir=%d, length=%u\n", dirs[d], length);
	return (length);
}

LineValue	Heuristic::calc_linevalue(unsigned int length, unsigned int open_sides) {
	static const LineValue values[6][3] = {
			{LineValue::NONE, LineValue::NONE, LineValue::NONE},
			{LineValue::NONE, LineValue::NONE, LineValue::NONE},
			{LineValue::NONE, LineValue::TWO, LineValue::TWO},
			{LineValue::NONE, LineValue::HALF_OPEN_THREE, LineValue::OPEN_THREE},
			{LineValue::NONE, LineValue::HALF_OPEN_FOUR, LineValue::OPEN_FOUR},
			{LineValue::FIVE, LineValue::FIVE, LineValue::FIVE}
	};

	length = std::min(length, 5u);
	return values[length][open_sides];
}

unsigned int Heuristic::count_open_sides(unsigned int prev, unsigned int next) const {
	unsigned int open_sides = 0;
	if (prev < REALBOARDSIZE && !isSeperatingBitIndex(prev) && tile_is_empty(prev)) {
//		if (g_log)
//			dprintf(2, "prev (%u) is empty!\n", prev);
		open_sides += 1u;
	}
	if (next < REALBOARDSIZE && !isSeperatingBitIndex(next) && tile_is_empty(next)) {
//		if (g_log)
//			dprintf(2, "next (%u) is empty!\n", next);
		open_sides += 1u;
	}
	return (open_sides);
}

void Heuristic::tryUpgradeLineVal(LineValue &lv, unsigned int prev, unsigned int next, const int dir, unsigned int stone_p) const {
	unsigned int before_prev = prev - dir;
	unsigned int after_next = next + dir;
	if ((tile_is_empty(prev) && bitboard_get(before_prev) == stone_p) || (tile_is_empty(next) && bitboard_get(after_next) == stone_p)) {
//		std::cerr << "b4_prev: " << bitboard_get(before_prev) << ", af_next: " << bitboard_get(after_next) << '\n';
		lv = static_cast<LineValue>(int(lv) + 1);
//		std::cerr << "upgraded line to " << lv << '\n';
	}
}

void Heuristic::count_lines(unsigned int start_idx, unsigned int stone_p, std::array<unsigned int, REALBOARDSIZE>& checkedTiles) {
	static const std::array<int, 4> dirs = setup_dirs();
	const unsigned int p = stone_p - 1;

//	if (g_log) dprintf(2, "start_idx: %u, stones: p=%u\n", start_idx, stone_p);
	for (unsigned int d = 0; d < dirs.size(); d++) {
		const int dir = dirs[d];

//		if (g_log) dprintf(2, "dirs[%u] = %d\n", d, dir);
		if (checkedTiles[start_idx] & (1u << d)) {
//			if (g_log) dprintf(2, "already checked %u in direction %d\n", start_idx, dir);
			continue ;
		}
		unsigned int next = start_idx + dir;
		unsigned int length = this->get_length(&next, stone_p, d, checkedTiles);

		if (length < 2) // Might change this to (length <= 2)
			continue;
		unsigned int prev = start_idx - dir;
		unsigned int open_sides = this->count_open_sides(prev, next);


		LineValue linevalue = this->calc_linevalue(length, open_sides);
		if (linevalue < HALF_OPEN_FOUR)
			tryUpgradeLineVal(linevalue, prev, next, dir, stone_p);

		this->values[p][linevalue]++;
		if (linevalue == LineValue::FIVE && this->isUnbreakable(start_idx, next - dir, dir)) {
			this->set_winner(p);
			return ;
		}
	}
}

void Heuristic::loop_over_tiles() {
	std::array<unsigned int, REALBOARDSIZE> checkedTiles{};

	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		unsigned int stone = this->bitboard_get(i);
		if (!stone || stone == std::numeric_limits<unsigned int>::max())
			continue;
		assert(stone == 1 || stone == 2);
		this->count_lines(i, stone, checkedTiles);
	}
}

void Heuristic::calculate_heuristic() {
	static const int	minus[2] = {
			-1, 1
	};
	const unsigned int opp = !player;
	/*
	 * player is the one who just placed their stone
	 * opp is the other one (whose turn it will be after this)
	 */
	if (this->values[player][FIVE]) {
		// if p0, -100000
		// else, +100000
		this->h = (minus[player] * LineValues.at(FIVE));
	}
	else if (this->values[opp][OPEN_FOUR]) {
		this->h = (minus[opp] * LineValues.at(OPEN_FOUR));
	}
	else if (this->values[player][OPEN_FOUR]) {
		this->h = (minus[player] * LineValues.at(OPEN_FOUR));
	} else {
		for (unsigned int i = LineValue::TWO; i <= LineValue::FIVE; ++i) {
			this->h += (this->values[1][i] - this->values[0][i]) * LineValues.at(i);
		}
	}
}

int Heuristic::set_h() {
//	static const int winner_values[3] = {0, -2000000, 2000000};
//	auto hash = hash_fn(this->board);
	this->h = 0;
//	if (tt.find(hash) != tt.end()) {
//		this->h = tt[hash];
//		return (this->h);
//	}
	for (unsigned short int i = 0; i < 2; ++i)
		this->values[i].fill(0);
	this->h = 0;
	this->loop_over_tiles();
	if (this->has_winner())
		return (this->h);
	this->calculate_heuristic();

	if (!this->has_winner()) {
		this->h = std::min(std::max(h, -1900000), 1900000);
	}
//	if (this->has_winner())
//		this->h = winner_values[this->get_winner()];
//	else

//	tt[hash] = this->h;

	return (this->h);
}

int Heuristic::add_h_for_captures() {
	static const int winner_values[2] = {-2000000, 2000000};
	assert(!(captures[0] >= 10 && captures[1] >= 10));
	if (this->has_winner())
		return (this->h);

	if (captures[0] >= 10 || captures[1] >= 10) {
		// winning condition
		auto p = (captures[0] >= 10) ? 0 : 1;
		this->winner = p;
		this->h = winner_values[p];
	}
	else {
		this->h += (captures[1] - captures[0] * 1000);
	}
	return (this->h);
}

std::string	LineValueToStr(const LineValue& x) {
	switch (x) {
		case LineValue::TWO:
			return "TWO";
		case LineValue::HALF_OPEN_THREE:
			return "HALF_OPEN_THREE";
		case LineValue::OPEN_THREE:
			return "OPEN_THREE";
		case LineValue::HALF_OPEN_FOUR:
			return "HALF_OPEN_FOUR";
		case LineValue::OPEN_FOUR:
			return "OPEN_FOUR";
		case LineValue::FIVE:
			return "FIVE";
		default:
			break ;
	}
	return ("NONE");
}

void Heuristic::print_heuristic(std::ostream &o) const {
	static const LineValue linevalues[] = {
			LineValue::TWO, LineValue::HALF_OPEN_THREE, LineValue::OPEN_THREE,
			LineValue::HALF_OPEN_FOUR, LineValue::OPEN_FOUR, LineValue::FIVE
	};
	o << "Heuristic value: " << this->get_h() << "\n";
	for (unsigned int p = 0; p < 2; p++) {
		o << "Player " << p + 1 << ":\n";
		for (auto & i : linevalues) {
			o << '\t' << LineValueToStr(i) << ": " << this->values[p].at(i) << "\n";
		}
		o << "\tCaptures: " << this->captures[p] << '\n';
	}
}

bool Heuristic::has_winner() const {
	return (this->winner != 0);
}

void Heuristic::set_winner(unsigned int p_winner) {
	static const int winner_values[2] = {-2000000, 2000000};

	assert(p_winner == 0 || p_winner == 1); //TODO: REMOVE
	this->winner = p_winner + 1;
	this->h = winner_values[p_winner];
}

int Heuristic::get_winner() const {
	return (this->winner);
}

Heuristic::Heuristic() { }

Heuristic::Heuristic(const Heuristic &x)
	: Bitboard(x), values(), h(x.h), winner(x.winner), depth(x.depth), player(x.player), captures(x.captures) {
	values[0].fill(0);
	values[1].fill(0);
}

Heuristic::~Heuristic() {
}

bool Heuristic::canGetCaptured(unsigned int start_idx, int dir) const {
	const unsigned int stone_p = this->bitboard_get(start_idx);
	const int dir_opp = -dir;
	unsigned int length = 1;
	unsigned int idx = start_idx + dir;
	while (idx < REALBOARDSIZE && !isSeperatingBitIndex(idx) && this->bitboard_get(idx) == stone_p) {
		++length;
		idx += dir;
	}
	unsigned int back_idx = start_idx + dir_opp;
	while (back_idx < REALBOARDSIZE && !isSeperatingBitIndex(back_idx) && this->bitboard_get(back_idx) == stone_p) {
		++length;
		back_idx += dir_opp;
	}
	if (length != 2 || idx >= REALBOARDSIZE || isSeperatingBitIndex(idx) || back_idx >= REALBOARDSIZE || isSeperatingBitIndex(back_idx))
		return (false);
	return (tile_is_empty(idx) ^ tile_is_empty(back_idx));
//	return ((int)tile_is_empty(idx) + (int)tile_is_empty(back_idx) == 1);
}

bool Heuristic::isUnbreakable(unsigned int start_idx, unsigned int end_idx, int dir) const {
	static const std::array<int, 4> dirs = setup_dirs();
	unsigned int unbroken_length = 0;

	for (; start_idx <= end_idx; start_idx += dir) {
		for (int d = 0; d < 4; d++) {
			if (d == dir)
				continue ;
			if (canGetCaptured(start_idx, dirs[d]))
				return (false);
		}
		++unbroken_length;
		if (unbroken_length == 5)
			break ;
	}
	return (unbroken_length == 5);
}

unsigned int Heuristic::get_opponent_stone(const unsigned int stone) {
	return (stone ^ 3u);
}

std::array<int, 2> Heuristic::get_captures() const {
	return (this->captures);
}

std::ostream &operator<<(std::ostream &o, const LineValue &lv) {
	o << LineValueToStr(lv);
	return (o);
}
