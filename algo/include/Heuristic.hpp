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
#include <map>
enum LineValue {
	NONE = 0,
    HALF_OPEN_TWO = 1,
	TWO = 2,
	HALF_OPEN_THREE = 3,
	OPEN_THREE = 4,
	HALF_OPEN_FOUR = 5,
	OPEN_FOUR = 6,
	FIVE = 7
};
static const std::array<uint32_t, 8> LineValues {
		0,		// NONE
		5,		// HALF_OPEN_TWO
		10,		// TWO
		1000,	// HALF_OPEN_THREE
		5000,	// OPEN_THREE
		7500,	// HALF_OPEN_FOUR
		5000,	// OPEN_FOUR
		100000	// FIVE
};

std::ostream&	operator<<(std::ostream& o, const LineValue& lv);

class Heuristic : public Bitboard {
protected:
	static std::hash<bitboard> hash_fn;
	static std::unordered_map<std::bitset<BOARDSIZE>, int> tt;

	std::array<std::array<uint8_t, 8>, 2> values{};
	int32_t h{};
	uint8_t winner{},
            player{};
	std::array<uint8_t, 2>	captures{};

	int set_h();
	int add_h_for_captures();
	[[nodiscard]] bool isUnbreakable(unsigned int start_idx, unsigned int end_idx, int dir) const;
	[[nodiscard]] bool canGetCaptured(unsigned int start_idx, int dir) const;

private:
	unsigned int get_length(unsigned int *idx, unsigned int stone_p, unsigned int d, std::array<unsigned int, REALBOARDSIZE>& g_checkedTiles) const;
	[[nodiscard]] unsigned int count_open_sides(unsigned int prev, unsigned int next) const;
	[[nodiscard]] static LineValue	calc_linevalue(unsigned int length, unsigned int open_sides);
	void	tryUpgradeLineVal(LineValue &lv, unsigned int prev, unsigned int next, int dir, unsigned int stone_p) const;
	void	calculate_heuristic();
	void	count_lines(unsigned int start_idx, unsigned int stone_p, std::array<unsigned int, REALBOARDSIZE>& checkedTiles);
	void	loop_over_tiles();
	Heuristic& operator=(const Heuristic& x);

public:
	Heuristic();
	~Heuristic();
	Heuristic(const Heuristic& x);

	[[nodiscard]] int	get_h() const;
	[[nodiscard]] bool	has_winner() const;
	void set_winner(unsigned int p_winner);
	[[nodiscard]] uint8_t get_winner() const;
	[[nodiscard]] uint8_t get_player() const;

	void	print_heuristic(std::ostream& o) const;
	static unsigned int get_opponent_stone(unsigned int stone);

	std::array<uint8_t, 2> get_captures() const;
};

#endif //GOMOKUBOT_HEURISTIC_HPP
