//
// Created by Peer De bakker on 4/4/22.
//

#ifndef GOMOKUBOT_HEURISTIC_HPP
#define GOMOKUBOT_HEURISTIC_HPP

#include "Bitboard.hpp"
#include <unordered_map>
#include <array>
#include <cstdint>

// I guess we shouldn't give value to the three O'sL:
// X O O O X

enum LineValue {
	NONE,
	HALF_OPEN_TWO,
	OPEN_TWO,
	HALF_OPEN_THREE,
	OPEN_THREE,
	HALF_OPEN_FOUR,
	OPEN_FOUR,
	FIVE
};
static const uint8_t LINEVALUE_COUNT = 8;
static const std::array<int32_t, LINEVALUE_COUNT> LineValues{
		0,		// NONE
		-9000,	// HALF_OPEN_TWO
		10,		// OPEN_TWO
		100,	// HALF_OPEN_THREE
		4500,	// OPEN_THREE
		4000,	// HALF_OPEN_FOUR
		10000,	// OPEN_FOUR
		100000	// FIVE
};
static const uint32_t CAPTURE_VALUE = 30000;

std::ostream &operator<<(std::ostream &o, const LineValue &lv);

typedef std::array<std::array<uint8_t, LINEVALUE_COUNT>, 2> ValueBoard;

class Heuristic : public Bitboard {
protected:
	ValueBoard values{};
	int32_t h{};
	uint8_t winner{},
			player{},
			created_open_threes{};
	std::array<uint8_t, 2> captures{};

	int set_h(const unsigned int new_stone_idx);
	int set_h_with_lookup(const unsigned int new_stone_idx);

	int add_h_for_captures();

	[[nodiscard]] bool isUnbreakable(unsigned int start_idx, unsigned int end_idx, int dir) const;

	[[nodiscard]] bool canGetCaptured(unsigned int start_idx, int dir) const;

private:
	unsigned int
	get_length(unsigned int &idx, unsigned int stone_p, unsigned int d, bool &empty_space_inbetween) const;

	[[nodiscard]] unsigned int count_open_sides(unsigned int prev, unsigned int next) const;

	[[nodiscard]] static LineValue calc_linevalue(unsigned int length, unsigned int open_sides);

	void calculate_heuristic();

	void
	count_lines(unsigned int start_idx, unsigned int stone_p);

	bool enoughSpaceForFiveInARow(unsigned int idx, int dir, int opp_dir, unsigned int opp_stone) const;

	void loop_over_tiles();

	Heuristic &operator=(const Heuristic &x);

public:
	Heuristic();

	~Heuristic();

	Heuristic(const Heuristic &x);

	[[nodiscard]] int get_h() const;

	[[nodiscard]] bool has_winner() const;

	void	clear_winner();

	void set_winner(unsigned int p_winner);

	[[nodiscard]] uint8_t get_winner() const;

	[[nodiscard]] uint8_t get_player() const;

	[[nodiscard]] uint8_t get_created_open_threes() const;

	void print_heuristic(std::ostream &o) const;

	static unsigned int get_opponent_stone(unsigned int stone);

	std::array<uint8_t, 2> get_captures() const;

	static std::unordered_map<std::bitset<BOARDSIZE>, ValueBoard> tt;
};

std::string LineValueToStr(const LineValue &x);

#endif //GOMOKUBOT_HEURISTIC_HPP
