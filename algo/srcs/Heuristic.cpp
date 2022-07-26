//
// Created by Peer De bakker on 4/4/22.
//

#include "Heuristic.hpp"
#include "Directions.hpp"
#include <iostream>
#include <cassert>
#include <limits>

std::hash<bitboard> Heuristic::hash_fn;
std::unordered_map<std::bitset<BOARDSIZE>, int> Heuristic::tt;
std::array<unsigned int, REALBOARDSIZE> g_checkedTiles;
std::array<unsigned int, REALBOARDSIZE> g_checkedTiles2; // for across the water
static unsigned int	g_newly_placed_stone_idx;
static bool			g_contains_newly_placed_stone;

int Heuristic::get_h() const {
	return (this->h);
}

uint8_t Heuristic::get_player() const {
	return (this->player);
}

unsigned int
Heuristic::get_length(unsigned int &idx, unsigned int stone_p, unsigned int d, bool &empty_space_inbetween) const {
	static const std::array<int, 4> dirs = setup_dirs();
	unsigned int lengths[2] = {1,0};
	const unsigned int opp_stone = get_opponent_stone(stone_p);
	unsigned int stone_value;
//	unsigned int empty_tile_count = 0;
	const unsigned int start_idx = idx;

	while (isvalid_tile(idx)) {
		stone_value = this->bitboard_get(idx);
		if (stone_value == opp_stone) {
			break ;
		} else if (stone_value == stone_p) {
			lengths[empty_space_inbetween]++;
			if (!empty_space_inbetween) {
				g_checkedTiles[idx] |= 1u << d;
			} else {
				g_checkedTiles2[idx] |= 1u << d;
			}
			if (idx == g_newly_placed_stone_idx) {
				g_contains_newly_placed_stone = true;
			}
		} else {
			if (empty_space_inbetween)
				break ;
			empty_space_inbetween = true;
		}
		idx += dirs[d];
	}
	if (g_checkedTiles2[start_idx] & (1u << d) && lengths[1] == 0) {
		return (0);
	}
	while (this->bitboard_get(idx - dirs[d]) != stone_p) {
		// so I can afterwards easily find the next tile
		idx -= dirs[d];
	}
	const unsigned int total_length = lengths[0] + lengths[1];
	if (total_length < 5)
		return (total_length);
	return (std::max(lengths[0], lengths[1]));
}

LineValue Heuristic::calc_linevalue(unsigned int length, unsigned int open_sides) {
	static const LineValue values[6][3] = {
			{LineValue::NONE, LineValue::NONE,            LineValue::NONE}, // zero-length
			{LineValue::NONE, LineValue::NONE,            LineValue::NONE}, // 1-length
			{LineValue::NONE, LineValue::HALF_OPEN_TWO,   LineValue::OPEN_TWO},
			{LineValue::NONE, LineValue::HALF_OPEN_THREE, LineValue::OPEN_THREE},
			{LineValue::NONE, LineValue::HALF_OPEN_FOUR,  LineValue::OPEN_FOUR},
			{LineValue::FIVE, LineValue::FIVE,            LineValue::FIVE}
	};

	length = std::min(length, 5u);
	return values[length][open_sides];
}

unsigned int Heuristic::count_open_sides(unsigned int prev, unsigned int next) const {
	unsigned int open_sides = 0;
	if (isvalid_tile(prev) && tile_is_empty(prev)) {
		++open_sides;
	}
	if (isvalid_tile(next) && tile_is_empty(next)) {
		++open_sides;
	}
	return (open_sides);
}

void Heuristic::tryUpgradeLineVal(LineValue &lv, unsigned int prev, unsigned int next, const int dir,
								  unsigned int stone_p) const {
	unsigned int before_prev = prev - dir;
	unsigned int after_next = next + dir;
	if ((tile_is_empty(prev) && bitboard_get(before_prev) == stone_p) ||
		(tile_is_empty(next) && bitboard_get(after_next) == stone_p)) {
		lv = static_cast<LineValue>(int(lv) + 1);
	}
}

void Heuristic::count_lines(unsigned int start_idx, unsigned int stone_p) {
	static const std::array<int, 4> dirs = setup_dirs();
	static const std::array<int, 4>	opp_dirs = setup_dirs_opposite();
	const unsigned int p = stone_p - 1;
	const unsigned int opp_stone = this->get_opponent_stone(stone_p);

	for (unsigned int d = 0; d < dirs.size(); d++) {
		const int dir = dirs[d];
		const int opp_dir = opp_dirs[d];

		if (g_checkedTiles[start_idx] & (1u << d)) {
			continue;
		}
//		if (!this->enoughSpaceForFiveInARow(start_idx, dir, opp_dirs[d], get_opponent_stone(stone_p))) {
//			continue ;
//		}
		unsigned int	next = start_idx + dir;
		bool	empty_space_inbetween = false;
		g_contains_newly_placed_stone = false;
		unsigned int	length = this->get_length(next, stone_p, d, empty_space_inbetween);

		if (length < 2) // Might change this to (length <= 2)
			continue;
		unsigned int prev = start_idx - dir;
		unsigned int open_sides = this->count_open_sides(prev, next);
		// TODO: check that there is enough space for it to grow into a 5 (but would capturable pieces become tricky?)

		if (!this->enoughSpaceForFiveInARow(start_idx, dir, opp_dir, opp_stone)) {
			continue ;
		}

		if (length == 4 && open_sides == 0 && empty_space_inbetween) {
			++open_sides;
		}

		LineValue linevalue = this->calc_linevalue(length, open_sides);
		if (g_contains_newly_placed_stone && linevalue == OPEN_THREE) {
			++this->created_open_threes;
		}

		this->values[p][linevalue]++;
	}
}

void Heuristic::loop_over_tiles() {
	g_checkedTiles.fill(0);
	g_checkedTiles2.fill(0);
	for (unsigned int i = 0; i < REALBOARDSIZE; i++) {
		unsigned int stone = this->bitboard_get(i);
		if (!stone || stone == std::numeric_limits<unsigned int>::max())
			continue;
		assert(stone == 1 || stone == 2);
		this->count_lines(i, stone);
	}
}

void Heuristic::calculate_heuristic() {
	static const int minus[2] = {
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
		this->h = (minus[player] * LineValues[FIVE]);
		this->set_winner(player);
	} else if (this->values[opp][OPEN_FOUR]) {
		this->h = (minus[opp] * LineValues[OPEN_FOUR]);
	} else if (this->values[opp][HALF_OPEN_FOUR]) {
		this->h = (minus[opp] * LineValues[HALF_OPEN_FOUR]);
	} else if (this->values[player][OPEN_FOUR]) {
		this->h = (minus[player] * LineValues[OPEN_FOUR]);
	} else {
		for (unsigned int i = LineValue::HALF_OPEN_TWO; i <= LineValue::FIVE; ++i) {
			this->h += (this->values[1][i] - this->values[0][i]) * LineValues[i];
		}
	}
}

int Heuristic::set_h(const unsigned int new_stone_idx) {
//	static const int winner_values[3] = {0, -2000000, 2000000};
//	auto hash = hash_fn(this->board);
	g_newly_placed_stone_idx = new_stone_idx;
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
	} else {
		this->h += (captures[1] - captures[0]) * CAPTURE_VALUE;
	}
	return (this->h);
}

std::string LineValueToStr(const LineValue &x) {
	switch (x) {
		case LineValue::HALF_OPEN_TWO:
			return "HALF_OPEN_TWO";
		case LineValue::OPEN_TWO:
			return "OPEN_TWO";
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
			break;
	}
	return ("NONE");
}

void Heuristic::print_heuristic(std::ostream &o) const {
	static const LineValue linevalues[] = {
			LineValue::HALF_OPEN_TWO, LineValue::OPEN_TWO, LineValue::HALF_OPEN_THREE, LineValue::OPEN_THREE,
			LineValue::HALF_OPEN_FOUR, LineValue::OPEN_FOUR, LineValue::FIVE
	};
	o << "Heuristic value: " << this->get_h() << "\n";
	for (unsigned int p = 0; p < 2; p++) {
		o << "Player " << p + 1 << ":\n";
		for (auto &i: linevalues) {
			o << '\t' << LineValueToStr(i) << ": " << (int) this->values[p][i] << "\n";
		}
		o << "\tCaptures: " << (int) this->captures[p] << '\n';
	}
}

bool Heuristic::has_winner() const {
	return (this->winner != 0);
}

void Heuristic::set_winner(unsigned int p_winner) {
	static const int winner_values[2] = {-2000000, 2000000};

	assert(p_winner == 0 || p_winner == 1);
	this->winner = p_winner + 1;
	this->h = winner_values[p_winner];
}

uint8_t Heuristic::get_winner() const {
	return (this->winner);
}

Heuristic::Heuristic() {}

Heuristic::Heuristic(const Heuristic &x)
		: Bitboard(x), values(), h(x.h), winner(x.winner), player(x.player), captures(x.captures) {
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
	if (length != 2 || idx >= REALBOARDSIZE || isSeperatingBitIndex(idx) || back_idx >= REALBOARDSIZE ||
		isSeperatingBitIndex(back_idx))
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
				continue;
			if (canGetCaptured(start_idx, dirs[d]))
				return (false);
		}
		++unbroken_length;
		if (unbroken_length == 5)
			break;
	}
	return (unbroken_length == 5);
}

unsigned int Heuristic::get_opponent_stone(const unsigned int stone) {
	return (stone ^ 3u);
}

std::array<uint8_t, 2> Heuristic::get_captures() const {
	return (this->captures);
}

bool Heuristic::enoughSpaceForFiveInARow(unsigned int idx, int dir, int opp_dir, unsigned int opp_stone) const {
	unsigned int tmp = idx + dir;
	unsigned int count = 1;

	// TODO: should I add checkedTiles here as well?

	while (isvalid_tile(tmp) && bitboard_get(tmp) != opp_stone) {
		count++;
		tmp += dir;
	}
	if (count >= 5)
		return (true);
	tmp = idx + opp_dir;
	while (isvalid_tile(tmp) && bitboard_get(tmp) != opp_stone) {
		count++;
		tmp += opp_dir;
	}
	return (count >= 5);
}

uint8_t Heuristic::get_created_open_threes() const {
	return (this->created_open_threes);
}

std::ostream &operator<<(std::ostream &o, const LineValue &lv) {
	o << LineValueToStr(lv);
	return (o);
}
