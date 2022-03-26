//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_GAMESTATE_HPP
#define CLUSTER_GAMESTATE_HPP

# include "Gomoku.hpp"
# include <array>
# include <vector>
# include <Move.hpp>
# include <unordered_map>
# include <bitset>

# define BOARDHEIGHT 19
# define BOARDWIDTH 20 // 1 for a seperating bit
# define BOARDSIZE BOARDHEIGHT * BOARDWIDTH

typedef std::bitset<BOARDSIZE> bitboard;

class Client;

class Gamestate {
protected:
	std::array<bitboard, 2>	boards;
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
	[[nodiscard]] int		get_h_value_player(int player_id) const;
	void	set_heuristic();
	[[nodiscard]] int		get_heuristic() const;

	void generate_children();
	std::vector<Gamestate*>& get_children() { return this->children; }

	bool	operator==(const Gamestate& rhs) const { return (this->h == rhs.h); }
	bool	operator!=(const Gamestate& rhs) const { return !(*this == rhs); }

	bool	operator<(const Gamestate& rhs) const { return (this->h < rhs.h); }
	bool	operator>(const Gamestate& rhs) const { return (rhs < *this); }
	bool	operator<=(const Gamestate& rhs) const { return !(rhs < *this); }
	bool	operator>=(const Gamestate& rhs) const { return !(*this < rhs); }

	void	print_board(std::ostream& o, bool print_colours) const;
	[[nodiscard]] int		get_player() const;

	[[nodiscard]] const Move& get_first_move() const;

protected:
	void	place_stone(int move_idx);
	int		change_player();

	// Captures.cpp
	unsigned int perform_captures(int pos);
	unsigned int capture_check_dir(int idx, int dir, int p);
	[[nodiscard]] bool		tile_is_empty(int idx) const;
	int collect_open_things(int idx, int player_id, std::unordered_map<int, unsigned int>& checked) const;
};

bool	is_seperating_bit(int idx);

#endif //CLUSTER_GAMESTATE_HPP
