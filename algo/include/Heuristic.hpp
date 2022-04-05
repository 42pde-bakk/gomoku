//
// Created by Peer De bakker on 4/4/22.
//

#ifndef GOMOKUBOT_HEURISTIC_HPP
#define GOMOKUBOT_HEURISTIC_HPP
#include "Bitboard.hpp"
#include <unordered_map>
#include <array>

// I guess we shouldn't give value to the three O'sL:
// X O O O X

enum LineValue {
	NONE = 0,
	TWO = 1,
	HALF_OPEN_THREE = 2,
	OPEN_THREE = 3,
	HALF_OPEN_FOUR = 4,
	OPEN_FOUR = 5,
	FIVE = 6
};
std::ostream&	operator<<(std::ostream& o, const LineValue& lv);

class Heuristic : public Bitboard {
protected:
	static std::hash<bitboard> hash_fn;
	std::array<std::array<int, 7>, 2> values{};
	int h{};
	int winner{};
	std::array<int, 2>	captures{};

	int set_h();
	int add_h_for_captures();
	[[nodiscard]] bool isUnbreakable(unsigned int start_idx, unsigned int end_idx, int dir) const;
	[[nodiscard]] bool canGetCaptured(unsigned int start_idx, int dir) const;

private:
	static std::unordered_map<std::bitset<BOARDSIZE>, int> tt;

	unsigned int get_length(unsigned int *idx, unsigned int stone_p, unsigned int d) const;
	[[nodiscard]] unsigned int count_open_sides(unsigned int prev, unsigned int next) const;
	[[nodiscard]] static LineValue	calc_linevalue(unsigned int length, unsigned int open_sides);
	void tryUpgradeLineVal(LineValue &lv, unsigned int prev, unsigned int next, const int dir, unsigned int stone_p) const;
	void	calculate_heuristic();
	void	count_lines(unsigned int start_idx, unsigned int stone_p);
	void	loop_over_tiles();
	Heuristic& operator=(const Heuristic& x);

public:
	Heuristic();
	~Heuristic();
	Heuristic(const Heuristic& x);

	[[nodiscard]] int get_h() const;
	[[nodiscard]] bool	has_winner() const;
	[[nodiscard]] int	get_winner() const;

	void	print_heuristic(std::ostream& o) const;
};


#endif //GOMOKUBOT_HEURISTIC_HPP
