//
// Created by Peer De bakker on 3/22/22.
//

#ifndef CLUSTER_GAMESTATE_HPP
#define CLUSTER_GAMESTATE_HPP

#include "Gomoku.hpp"
#include <array>
#include <vector>
#include <Move.hpp>

#define BOARDHEIGHT 19
#define BOARDWIDTH 20 // 1 for a seperating bit
#define BOARDSIZE BOARDHEIGHT * BOARDWIDTH
#define SHIFT_N(arg) (arg << BOARDWIDTH)
#define SHIFT_S(arg) (arg >> BOARDWIDTH)
#define SHIFT_W(arg) (arg << 1)
#define SHIFT_E(arg) (arg >> 1)

#define SHIFT_NE(arg) (arg << (BOARDWIDTH - 1))
#define SHIFT_SE(arg) (arg >> (BOARDWIDTH + 1))
#define SHIFT_NW(arg) (arg << (BOARDWIDTH + 1))
#define SHIFT_SW(arg) (arg >> (BOARDWIDTH - 1))

//using bitboard = std::bitset<BOARDSIZE>;
typedef std::bitset<BOARDSIZE> bitboard;

enum class STONE {
	EMPTY,
	PLAYER1,
	PLAYER2
};

class Client;

class Gamestate {
public:
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

	void generate_children();
	std::vector<Gamestate*>& get_children() { return this->children; }

	bool	operator==(const Gamestate& rhs) const { return (this->h == rhs.h); }
	bool	operator!=(const Gamestate& rhs) const { return !(*this == rhs); }

	bool	operator<(const Gamestate& rhs) const { return (this->h < rhs.h); }
	bool	operator>(const Gamestate& rhs) const { return (rhs < *this); }
	bool	operator<=(const Gamestate& rhs) const { return !(rhs < *this); }
	bool	operator>=(const Gamestate& rhs) const { return !(*this < rhs); }

	void	print_board(std::ostream& o) const;
	[[nodiscard]] int		get_player() const;

protected:
	void	place_stone(int move_idx);
	int		change_player();

	// Captures.cpp
	int perform_captures(int pos);
	int capture_check_dir(int idx, int dir, int p);
};


#endif //CLUSTER_GAMESTATE_HPP
