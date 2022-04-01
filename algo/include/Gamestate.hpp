//
// Created by Peer De bakker on 3/22/22.
//

#ifndef GOMOKUBOT_GAMESTATE_HPP
#define GOMOKUBOT_GAMESTATE_HPP

# include "Gomoku.hpp"
# include <array>
# include <vector>
# include <Move.hpp>
# include <unordered_map>
# include <bitset>
# include "Bitboard.hpp"



class Client;

class Gamestate : public Bitboard {
protected:
	static std::unordered_map<std::bitset<BOARDSIZE>, int> tt;
	std::array<int, 2>	captures{};
	std::vector<Move>	moves;
	const Gamestate	*parent{};
	std::vector<Gamestate*> children;
	int turn{},
		h{},
		winner{},
		player{};

	friend Client;
public:
	Gamestate();
	Gamestate& operator=(const Gamestate& x);
	Gamestate(const Gamestate& x);
	~Gamestate();

	[[nodiscard]] bool	has_winner() const { return (this->winner != 0); }
//	void	update_heuristic(unsigned int move_idx);
	void	set_heuristic();
	[[nodiscard]] int		get_heuristic() const;
	static unsigned int get_opponent_stone(unsigned int stone);

	void generate_children();
	std::vector<Gamestate*>& get_children() { return this->children; }

	bool	operator==(const Gamestate& rhs) const { return (this->board == rhs.board); }
	bool	operator!=(const Gamestate& rhs) const { return !(*this == rhs); }

	bool	operator<(const Gamestate& rhs) const { return (this->h < rhs.h); }
	bool	operator>(const Gamestate& rhs) const { return (rhs < *this); }
	bool	operator<=(const Gamestate& rhs) const { return !(rhs < *this); }
	bool	operator>=(const Gamestate& rhs) const { return !(*this < rhs); }

	[[nodiscard]] int		get_player() const;

	[[nodiscard]] const Move& get_first_move() const;

	void	place_stone(int move_idx);
	void	clear_children();
protected:
	int		change_player();

	// Captures.cpp
	unsigned int perform_captures(int pos);
	unsigned int capture_check_dir(int idx, int dir);

	unsigned int	h_for_tile(unsigned int start_idx, unsigned int stone_p, unsigned int stone_opp, std::unordered_map<int, unsigned int>& checked_tiles) const;
};

typedef bool (*compareFunc)(const Gamestate* a, const Gamestate* b);

#endif //GOMOKUBOT_GAMESTATE_HPP
